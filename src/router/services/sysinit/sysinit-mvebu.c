/*
 * sysinit-pb42.c
 *
 * Copyright (C) 2006 - 2024 Sebastian Gottschall <s.gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id:
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <ddnvram.h>
#include <shutils.h>
#include <utils.h>

#define SIOCGMIIREG 0x8948 /* Read MII PHY register.  */
#define SIOCSMIIREG 0x8949 /* Write MII PHY register.  */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/mii.h>
#include "devices/wireless.c"

void start_sysinit(void)
{
	char buf[PATH_MAX];
	struct stat tmp_stat;
	time_t tm = 0;
	FILE *fp;

	if (!nvram_matchi("disable_watchdog", 1)) {
		insmod("orion_wdt");
	}
	/*
	 * Setup console 
	 */

	klogctl(8, NULL, nvram_geti("console_loglevel"));
	int brand = getRouterBrand();

	/* 
	 * 
	 */

	insmod("i2c-core");
	insmod("i2c-dev");
	insmod("i2c-mv64xxx");
	insmod("regmap-core");
	insmod("regmap-i2c");
	insmod("regmap-spi");
	//insmod("input-core");
	//insmod("button-hotplug");
	//insmod("gpio-button-hotplug");
	insmod("leds-tlc591xx");
	insmod("leds-pca963x");

	insmod("hwmon");
	insmod("thermal_sys");
	insmod("gpio-fan");
	insmod("pwm-fan");
	insmod("armada_thermal");
	insmod("tmp421");

	insmod("rtc-armada38x"); // for WRT1200AC / WRT1900ACv2 only
	insmod("mii");
	// crypto drivers
	insmod("libdes");
	insmod("des_generic");
	insmod("marvell-cesa"); // tested on WRT1900AC v1 so far
	insmod("cryptodev");

	insmod("mmc_core");
	insmod("mmc_block");
	insmod("sdhci");
	insmod("sdhci-pltfm");
	insmod("sdhci-pxav3");
	insmod("mvsdio");

	int s;
	struct ifreq ifr;
	char *recovery = getUEnv("auto_recovery");
	if (recovery && strcmp(recovery, "yes"))
		eval("ubootenv", "set", "auto_recovery", "yes");

	/*
	 * Set a sane date 
	 */
	stime(&tm);
	nvram_set("wl0_ifname", "wlan0");
	nvram_set("wl1_ifname", "wlan1");

	writeint("/sys/class/hwmon/hwmon0/pwm1", 0);
	char line[256];
	char *mac;
	if ((fp = fopen("/dev/mtdblock3", "r"))) {
		while (fgets(line, sizeof(line), fp) != NULL) {
			if (strstr(line, "hw_mac_addr")) {
				strtok_r(line, "=", &mac);
				//fprintf(stderr, "Found mac: %s\n", mac);
				nvram_set("et0macaddr", mac);
				nvram_set("et0macaddr_safe", mac);
				set_hwaddr("eth0", mac);
				set_hwaddr("lan1", mac);
				set_hwaddr("lan2", mac);
				set_hwaddr("lan3", mac);
				set_hwaddr("lan4", mac);
				MAC_ADD(mac);
				set_hwaddr("wan", mac);
			}
		}
		fclose(fp);
	}
	eval("ifconfig", "eth0", "up");
	eval("ifconfig", "eth1", "up");

	/*     if (brand == ROUTER_WRT_1900AC) {
	   set_smp_affinity(27, 2);
	   set_smp_affinity(42, 2);
	   } else {
	   set_smp_affinity(36, 2);
	   set_smp_affinity(46, 2);
	   } */
	set_gpio(3, 0); //disable sata led as initial value
	set_gpio(4, 0); //disable usb 1 led as initial value
	set_gpio(5, 0); //disable usb 2 led as initial value
	set_gpio(8, 0); //disable usb ss led as initial value

	if (brand == ROUTER_WRT_1900AC) {
		writestr("/sys/class/leds/mamba\\:white\\:esata/trigger", "disk-activity");
	}
	if (brand == ROUTER_WRT_1200AC) {
		writestr("/sys/class/leds/caiman\\:white\\:sata/trigger", "disk-activity");
	}
	if (brand == ROUTER_WRT_1900ACV2) {
		writestr("/sys/class/leds/cobra\\:white\\:sata/trigger", "disk-activity");
	}
	if (brand == ROUTER_WRT_1900ACS) {
		writestr("/sys/class/leds/shelby\\:white\\:sata/trigger", "disk-activity");
	}
	if (brand == ROUTER_WRT_3200ACM) {
		writestr("/sys/class/leds/rango\\:white\\:sata/trigger", "disk-activity");
	}
	if (brand == ROUTER_WRT_32X) {
		writestr("/sys/class/leds/venom\\:blue\\:sata/trigger", "disk-activity");
	}
	if (nvram_geti("nvram_ver") < 11 || nvram_match("wan_ifname2", "eth0") || nvram_match("wan_ifname2", "eth1") ||
	    nvram_match("wan_ifname", "eth1") || nvram_match("wan_ifname", "eth0")) {
		nvram_set("nvram_ver", "11");
		nvram_set("wan_ifname2", "wan");
		nvram_set("wan_ifname", "wan");
		nvram_set("wan_ifnames", "wan");
		nvram_set("wan_default", "wan");
		nvram_set("wan_iface", "wan");
	}

	nvram_unset("sw_wancpuport");
	nvram_unset("sw_lancpuport");
	nvram_unset("sw_wan");
	nvram_unset("sw_lan1");
	nvram_unset("sw_lan2");
	nvram_unset("sw_lan3");
	nvram_unset("sw_lan4");

	return;
}

int check_cfe_nv(void)
{
	nvram_seti("portprio_support", 0);
	return 0;
}

int check_pmon_nv(void)
{
	return 0;
}

void sys_overclocking(void)
{
}

char *enable_dtag_vlan(int enable)
{
	return "wan";
}

char *set_wan_state(int state)
{
	return NULL;
}

void start_devinit_arch(void)
{
}

void start_resetbc(void)
{
	if (!nvram_match("nobcreset", "1"))
		eval("mtd", "resetbc", "s_env");
}
void start_wifi_drivers(void)
{
	int brand = getRouterBrand();
	if (!load_mac80211()) {
		insmod("mwlwifi");
		insmod("mwifiex");
		insmod("mwifiex_sdio.ko");
		eval("modprobe", "bluetooth");
		eval("modprobe", "btmrvl");
		eval("modprobe", "btmrvl_sdio");
		if (brand == ROUTER_WRT_3200ACM)
			wait_for_wifi(3);
		else
			wait_for_wifi(2);
	}
}
void start_arch_defaults(void)
{
}
