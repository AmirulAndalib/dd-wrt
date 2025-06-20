/*
 * sysinit-wr741.c
 *
 * Copyright (C) 2009 - 2024 Sebastian Gottschall <s.gottschall@dd-wrt.com>
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
#include "devices/ethtools.c"

void start_sysinit(void)
{
	time_t tm = 0;

	/*
	 * Setup console 
	 */

	klogctl(8, NULL, nvram_geti("console_loglevel"));

	/*
	 * network drivers 
	 */
	fprintf(stderr, "load ATH Ethernet Driver\n");
	system("insmod ag71xx || insmod ag7240_mod");
	insmod("ledtrig-netdev");

	FILE *fp = fopen("/dev/mtdblock/0", "rb");
	char mac[32];
	if (fp) {
		unsigned char buf2[256];
		fseek(fp, 0x1fc00, SEEK_SET);
		fread(buf2, 256, 1, fp);
		fclose(fp);
		unsigned int copy[256];
		int i;
		for (i = 0; i < 256; i++)
			copy[i] = buf2[i] & 0xff;
		sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", copy[0] & 0xff, copy[1] & 0xff, copy[2] & 0xff, copy[3] & 0xff,
			copy[4] & 0xff, copy[5] & 0xff);
		fprintf(stderr, "configure eth0 to %s\n", mac);
		MAC_SUB(mac);
		set_hwaddr("eth0", mac);
		MAC_ADD(mac);
		MAC_ADD(mac);
		fprintf(stderr, "configure eth1 to %s\n", mac);
		set_hwaddr("eth1", mac);
#ifndef HAVE_ATH9K
		MAC_SUB(mac);
#endif
	}

	eval("ifconfig", "eth0", "up");
	eval("ifconfig", "eth1", "up");

#ifdef HAVE_SWCONFIG

#ifdef HAVE_WDR2543

	eval("swconfig", "dev", "switch0", "set", "reset", "1");
	eval("swconfig", "dev", "switch0", "set", "enable_vlan", "1");
	eval("swconfig", "dev", "switch0", "vlan", "1", "set", "ports", "1 2 3 4 9t");
	eval("swconfig", "dev", "switch0", "vlan", "2", "set", "ports", "0 9t");
	eval("swconfig", "dev", "switch0", "set", "apply");

	eval("vconfig", "set_name_type", "VLAN_PLUS_VID_NO_PAD");
	eval("vconfig", "add", "eth0", "1");
	eval("vconfig", "add", "eth0", "2");

	nvram_seti("sw_cpuport", 9);
	nvram_seti("sw_wan", 0);
	nvram_seti("sw_lan1", 1);
	nvram_seti("sw_lan2", 2);
	nvram_seti("sw_lan3", 3);
	nvram_seti("sw_lan4", 4);

#else
	eval("swconfig", "dev", "eth1", "set", "reset", "1");
	eval("swconfig", "dev", "eth1", "set", "enable_vlan", "1");
	eval("swconfig", "dev", "eth1", "vlan", "1", "set", "ports", "0 1 2 3 4");
	eval("swconfig", "dev", "eth1", "set", "apply");

	nvram_set("switchphy", "eth1");
	nvram_seti("sw_cpuport", 0);
	nvram_seti("sw_wan", -1);
	nvram_seti("sw_lan1", 1);
	nvram_seti("sw_lan2", 2);
	nvram_seti("sw_lan3", 3);
	nvram_seti("sw_lan4", 4);

	setEthLED(17, "eth0");
	setSwitchLED(13, 0x2);
	setSwitchLED(14, 0x4);
	setSwitchLED(15, 0x8);
	setSwitchLED(16, 0x10);
#endif

#endif
	nvram_default_geti("port0vlans", 2);
	nvram_default_geti("port1vlans", 1);
	nvram_default_geti("port2vlans", 1);
	nvram_default_geti("port3vlans", 1);
	nvram_default_geti("port4vlans", 1);
	nvram_default_get("port5vlans", "1 2 16000");

	struct ifreq ifr;
	int s;

	char macaddr[32];
	if (get_hwaddr("eth0", macaddr)) {
		nvram_set("et0macaddr", macaddr);
		nvram_set("et0macaddr_safe", macaddr);
	}

	detect_wireless_devices(RADIO_ALL);
#ifndef HAVE_ATH9K
	fprintf(stderr, "configure wifi0 to %s\n", mac);
	set_hwaddr("wifi0", mac);
#endif
	//enable wlan led (card gpio based)
#ifndef HAVE_WR810N
#if defined(HAVE_WR841v7) || defined(HAVE_WR842) || defined(HAVE_MR3420)
	setWirelessLedPhy0(0);
#else
	setWirelessLedPhy0(1);
#endif
#endif
	/*
	 * Set a sane date 
	 */
	stime(&tm);
	nvram_set("wl0_ifname", "wlan0");

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
	return "eth0";
}

char *set_wan_state(int state)
{
	return NULL;
}

void start_devinit_arch(void)
{
}
void start_wifi_drivers(void)
{
}
void start_arch_defaults(void)
{
}
