/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 *
 * Copyright (c) 2022-2023 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/kconfig.h>
#include <linux/version.h>
#include <linux/phy.h>
#include <linux/kthread.h>

#include "hsl.h"
#include "ssdk_plat.h"
#include "hsl_phy.h"
#include "qcaphy_common.h"
#include "hsl_ptp.h"
#include "qca808x_phy.h"

#define QCA808X_PHY_DRIVER_NAME	"QCA808X ethernet"
#define QCA808X_SSDK_PHY_DRIVER_NAME	"QCA808X SSDK ethernet"

#if defined(IN_LINUX_STD_PTP)
#include <linux/ptp_clock_kernel.h>
typedef struct {
	/* ptp filter class */
	a_int32_t ptp_type;
	/* ptp frame type */
	a_int32_t pkt_type;
} qca808x_ptp_cb;

typedef struct {
	a_uint8_t reserved0;
	a_uint8_t reserved1;
	a_uint8_t msg_type;
	a_uint16_t seqid;
	a_uint32_t reserved2;
	a_int64_t correction;
} qca808x_embeded_ts;

struct qca808x_ptp_info {
	a_int32_t hwts_tx_type;
	a_int32_t hwts_rx_type;
	struct qca808x_ptp_clock *clock;
	struct delayed_work tx_ts_work;
	struct delayed_work rx_ts_work;
	/* work for writing ingress time to register */
	struct delayed_work ingress_trig_work;
	a_int32_t ingress_time;
	struct sk_buff_head tx_queue;
	struct sk_buff_head rx_queue;
	qca808x_embeded_ts embeded_ts;
};
#endif

struct qca808x_phy_info {
	struct list_head list;
	a_uint32_t dev_id;
	/* phy real address,it is the mdio addr or the i2c slave addr */
	a_uint32_t phy_addr;
	/* the address of phy device, it is a fake addr for the i2c accessed phy */
	a_uint32_t phydev_addr;
#if defined(IN_LINUX_STD_PTP)
	a_int32_t speed;
	a_uint16_t clock_mode;
	a_uint16_t step_mode;
	/* work for gps sencond sync */
	struct delayed_work ts_schedule_work;
	a_bool_t gps_seconds_sync_en;
#endif
};

typedef struct {
	struct phy_device *phydev;
	struct qca808x_phy_info *phy_info;
#if defined(IN_LINUX_STD_PTP)
	struct qca808x_ptp_info ptp_info;
	/*the statistics array records the counter of
	 * rx and tx ptp event packet */
	hsl_ptp_event_pkt_stat_t ptp_event_stat[FAL_TX_DIRECTION + 1];
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,1,0))
	struct mii_timestamper mii_ts;
#endif
#endif
} qca808x_priv;

#if defined(IN_LINUX_STD_PTP)
struct qca808x_ptp_clock{
	struct ptp_clock_info caps;
	struct ptp_clock *ptp_clock;
	struct mutex tsreg_lock;
	qca808x_priv *priv;
};

struct qca808x_phy_info* qca808x_phy_info_get(a_uint32_t phy_addr);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6,1,0))
void qca808x_ptp_change_notify(struct mii_timestamper *mii_ts, struct phy_device *phydev);
int qca808x_hwtstamp(struct mii_timestamper *mii_ts, struct ifreq *ifr);
bool qca808x_rxtstamp(struct mii_timestamper *mii_ts, struct sk_buff *skb, int type);
void qca808x_txtstamp(struct mii_timestamper *mii_ts, struct sk_buff *skb, int type);
int qca808x_ts_info(struct mii_timestamper *mii_ts, struct ethtool_ts_info *info);
#else
void qca808x_ptp_change_notify(struct phy_device *phydev);
int qca808x_hwtstamp(struct phy_device *phydev, struct ifreq *ifr);
bool qca808x_rxtstamp(struct phy_device *phydev, struct sk_buff *skb, int type);
void qca808x_txtstamp(struct phy_device *phydev, struct sk_buff *skb, int type);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0))
int qca808x_ts_info(struct phy_device *phydev, struct ethtool_ts_info *info);
#endif
#endif
sw_error_t qca808x_ptp_config_init(struct phy_device *phydev);
int qca808x_ptp_init(qca808x_priv *priv);
void qca808x_ptp_deinit(qca808x_priv *priv);
int qca808x_ptp_hook_init(void);
void qca808x_ptp_hook_cleanup(void);
#endif

void qca808x_phydev_init(a_uint32_t dev_id, a_uint32_t port_id);
void qca808x_phydev_deinit(a_uint32_t dev_id, a_uint32_t port_id);
a_int32_t qca808x_phy_driver_register(void);
void qca808x_phy_driver_unregister(void);
int qca808x_phy_probe(struct phy_device *phydev);
void qca808x_phy_remove(struct phy_device *phydev);
