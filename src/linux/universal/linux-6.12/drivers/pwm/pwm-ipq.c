// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
/*
 * Copyright (c) 2016-2017, 2020 The Linux Foundation. All rights reserved.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/math64.h>
#include <linux/of_device.h>
#include <linux/bitfield.h>
#include <linux/units.h>

/* The frequency range supported is 1 Hz to clock rate */
#define IPQ_PWM_MAX_PERIOD_NS	((u64)NSEC_PER_SEC)

/*
 * The max value specified for each field is based on the number of bits
 * in the pwm control register for that field
 */
#define IPQ_PWM_MAX_DIV		0xFFFF

/*
 * Two 32-bit registers for each PWM: REG0, and REG1.
 * Base offset for PWM #i is at 8 * #i.
 */
#define IPQ_PWM_REG0			0
#define IPQ_PWM_REG0_PWM_DIV		GENMASK(15, 0)
#define IPQ_PWM_REG0_HI_DURATION	GENMASK(31, 16)

#define IPQ_PWM_REG1			4
#define IPQ_PWM_REG1_PRE_DIV		GENMASK(15, 0)
/*
 * Enable bit is set to enable output toggling in pwm device.
 * Update bit is set to reflect the changed divider and high duration
 * values in register.
 */
#define IPQ_PWM_REG1_UPDATE		BIT(30)
#define IPQ_PWM_REG1_ENABLE		BIT(31)

struct ipq_pwm_chip {
	struct clk *clk;
	void __iomem *mem;
};

static struct ipq_pwm_chip *ipq_pwm_from_chip(struct pwm_chip *chip)
{
	return pwmchip_get_drvdata(chip);
}

static unsigned int ipq_pwm_reg_read(struct pwm_device *pwm, unsigned int reg)
{
	struct ipq_pwm_chip *ipq_chip = ipq_pwm_from_chip(pwm->chip);
	unsigned int off = 8 * pwm->hwpwm + reg;

	return readl(ipq_chip->mem + off);
}

static void ipq_pwm_reg_write(struct pwm_device *pwm, unsigned int reg,
			      unsigned int val)
{
	struct ipq_pwm_chip *ipq_chip = ipq_pwm_from_chip(pwm->chip);
	unsigned int off = 8 * pwm->hwpwm + reg;

	writel(val, ipq_chip->mem + off);
}

static void config_div_and_duty(struct pwm_device *pwm, unsigned int pre_div,
				unsigned int pwm_div, unsigned long rate, u64 duty_ns,
				bool enable)
{
	unsigned long hi_dur;
	unsigned long val = 0;

	/*
	 * high duration = pwm duty * (pwm div + 1)
	 * pwm duty = duty_ns / period_ns
	 */
	hi_dur = div64_u64(duty_ns * rate, (pre_div + 1) * NSEC_PER_SEC);

	val = FIELD_PREP(IPQ_PWM_REG0_HI_DURATION, hi_dur) |
		FIELD_PREP(IPQ_PWM_REG0_PWM_DIV, pwm_div);
	ipq_pwm_reg_write(pwm, IPQ_PWM_REG0, val);

	val = FIELD_PREP(IPQ_PWM_REG1_PRE_DIV, pre_div);
	ipq_pwm_reg_write(pwm, IPQ_PWM_REG1, val);

	/* PWM enable toggle needs a separate write to REG1 */
	val |= IPQ_PWM_REG1_UPDATE;
	if (enable)
		val |= IPQ_PWM_REG1_ENABLE;
	ipq_pwm_reg_write(pwm, IPQ_PWM_REG1, val);
}

static int ipq_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
			 const struct pwm_state *state)
{
	struct ipq_pwm_chip *ipq_chip = ipq_pwm_from_chip(chip);
	unsigned int pre_div, pwm_div, best_pre_div, best_pwm_div;
	unsigned long rate = clk_get_rate(ipq_chip->clk);
	u64 period_ns, duty_ns, period_rate;
	u64 min_diff;

	if (state->polarity != PWM_POLARITY_NORMAL)
		return -EINVAL;

	if (state->period < DIV64_U64_ROUND_UP(NSEC_PER_SEC, rate))
		return -ERANGE;

	period_ns = min(state->period, IPQ_PWM_MAX_PERIOD_NS);
	duty_ns = min(state->duty_cycle, period_ns);

	/*
	 * period_ns is 1G or less. As long as rate is less than 16 GHz,
	 * period_rate does not overflow. Make that explicit.
	 */
	if ((unsigned long long)rate > 16ULL * GIGA)
		return -EINVAL;
	period_rate = period_ns * rate;
	best_pre_div = IPQ_PWM_MAX_DIV;
	best_pwm_div = IPQ_PWM_MAX_DIV;
	/*
	 * We don't need to consider pre_div values smaller than
	 *
	 *                              period_rate
	 *  pre_div_min := ------------------------------------
	 *                 NSEC_PER_SEC * (IPQ_PWM_MAX_DIV + 1)
	 *
	 * because pre_div = pre_div_min results in a better
	 * approximation.
	 */
	pre_div = div64_u64(period_rate,
			    (u64)NSEC_PER_SEC * (IPQ_PWM_MAX_DIV + 1));
	min_diff = period_rate;

	for (; pre_div <= IPQ_PWM_MAX_DIV; pre_div++) {
		u64 remainder;

		pwm_div = div64_u64_rem(period_rate,
					(u64)NSEC_PER_SEC * (pre_div + 1), &remainder);
		/* pwm_div is unsigned; the check below catches underflow */
		pwm_div--;

		/*
		 * Swapping values for pre_div and pwm_div produces the same
		 * period length. So we can skip all settings with pre_div >
		 * pwm_div which results in bigger constraints for selecting
		 * the duty_cycle than with the two values swapped.
		 */
		if (pre_div > pwm_div)
			break;

		/*
		 * Make sure we can do 100% duty cycle where
		 * hi_dur == pwm_div + 1
		 */
		if (pwm_div > IPQ_PWM_MAX_DIV - 1)
			continue;

		if (remainder < min_diff) {
			best_pre_div = pre_div;
			best_pwm_div = pwm_div;
			min_diff = remainder;

			if (min_diff == 0) /* bingo */
				break;
		}
	}

	/* config divider values for the closest possible frequency */
	config_div_and_duty(pwm, best_pre_div, best_pwm_div,
			    rate, duty_ns, state->enabled);

	return 0;
}

static int ipq_pwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
			     struct pwm_state *state)
{
	struct ipq_pwm_chip *ipq_chip = ipq_pwm_from_chip(chip);
	unsigned long rate = clk_get_rate(ipq_chip->clk);
	unsigned int pre_div, pwm_div, hi_dur;
	u64 effective_div, hi_div;
	u32 reg0, reg1;

	reg0 = ipq_pwm_reg_read(pwm, IPQ_PWM_REG0);
	reg1 = ipq_pwm_reg_read(pwm, IPQ_PWM_REG1);

	state->polarity = PWM_POLARITY_NORMAL;
	state->enabled = reg1 & IPQ_PWM_REG1_ENABLE;

	pwm_div = FIELD_GET(IPQ_PWM_REG0_PWM_DIV, reg0);
	hi_dur = FIELD_GET(IPQ_PWM_REG0_HI_DURATION, reg0);
	pre_div = FIELD_GET(IPQ_PWM_REG1_PRE_DIV, reg1);

	/* No overflow here, both pre_div and pwm_div <= 0xffff */
	effective_div = (u64)(pre_div + 1) * (pwm_div + 1);
	state->period = DIV64_U64_ROUND_UP(effective_div * NSEC_PER_SEC, rate);

	hi_div = hi_dur * (pre_div + 1);
	state->duty_cycle = DIV64_U64_ROUND_UP(hi_div * NSEC_PER_SEC, rate);

	return 0;
}

static const struct pwm_ops ipq_pwm_ops = {
	.apply = ipq_pwm_apply,
	.get_state = ipq_pwm_get_state,
};

static int ipq_pwm_probe(struct platform_device *pdev)
{
	struct pwm_chip *chip;
	struct ipq_pwm_chip *pwm;
	struct device *dev = &pdev->dev;
	int ret;

	chip = devm_pwmchip_alloc(dev, 4, sizeof(*pwm));
	if (IS_ERR(chip))
		return PTR_ERR(chip);
	pwm = ipq_pwm_from_chip(chip);

	pwm->mem = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(pwm->mem))
		return dev_err_probe(dev, PTR_ERR(pwm->mem),
				"regs map failed");

	pwm->clk = devm_clk_get_enabled(dev, NULL);
	if (IS_ERR(pwm->clk))
		return dev_err_probe(dev, PTR_ERR(pwm->clk),
				"failed to get clock");

	chip->ops = &ipq_pwm_ops;

	ret = devm_pwmchip_add(dev, chip);
	if (ret < 0)
		dev_err_probe(dev, ret, "devm_pwmchip_add() failed\n");

	return ret;
}

static const struct of_device_id pwm_ipq_dt_match[] = {
	{ .compatible = "qcom,ipq6018-pwm", },
	{}
};
MODULE_DEVICE_TABLE(of, pwm_ipq_dt_match);

static struct platform_driver ipq_pwm_driver = {
	.driver = {
		.name = "ipq-pwm",
		.of_match_table = pwm_ipq_dt_match,
	},
	.probe = ipq_pwm_probe,
};

module_platform_driver(ipq_pwm_driver);

MODULE_LICENSE("Dual BSD/GPL");
