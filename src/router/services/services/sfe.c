/*
 * sfe.c
 *
 * Copyright (C) 2019 - 2024 Sebastian Gottschall <s.gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
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
#ifdef HAVE_SFE
#include <stdlib.h>
#include <ddnvram.h>
#include <shutils.h>
#include <utils.h>
#include <wlutils.h>
#include <syslog.h>
#include <signal.h>
#include <services.h>

void start_sfe(void)
{
	if (nvram_match("sfe", "1")) {
		eval("modprobe", "ipv6");
		insmod("shortcut-fe");
		insmod("shortcut-fe-ipv6");
		insmod("fast-classifier");
		writeproc("/proc/ctf", "0");
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv4_stop");
		sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv6_stop");
		sysprintf("echo 1 > /sys/kernel/debug/ecm/ecm_db/defunct_all");
		rmmod("ecm");
		rmmod("hyfi-bridging");
		rmmod("qca-nss-sfe");
#endif
		sysprintf("echo 1 > /sys/fast_classifier/skip_to_bridge_ingress");
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
		sysprintf("echo 0 > /proc/sys/dev/nss/general/redirect");
#endif
		dd_loginfo("sfe", "shortcut (SFE) forwarding engine successfully started");
	} else if (nvram_match("sfe", "2")) {
		rmmod("fast-classifier");
		rmmod("shortcut-fe-ipv6");
		rmmod("shortcut-fe");
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv4_stop");
		sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv6_stop");
		sysprintf("echo 1 > /sys/kernel/debug/ecm/ecm_db/defunct_all");
		rmmod("ecm");
		rmmod("hyfi-bridging");
		rmmod("qca-nss-sfe");
#endif
		writeproc("/proc/ctf", "1");
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
		sysprintf("echo 0 > /proc/sys/dev/nss/general/redirect");
#endif
		dd_loginfo("ctf", "fast path (CTF) forwarding successfully started");
	} else if (nvram_match("sfe", "3") && has_nss) { // ecm nss
		rmmod("fast-classifier");
		rmmod("shortcut-fe-ipv6");
		rmmod("shortcut-fe");
		writeproc("/proc/ctf", "0");
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		eval("insmod", "qca-nss-sfe");
		insmod("hyfi-bridging");
		eval("insmod", "ecm", "front_end_selection=1");
#endif
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
		sysprintf("echo 1 > /proc/sys/dev/nss/general/redirect");
#endif
		dd_loginfo("ecm-nss", "Enhanced Connection Manager (ECM) forwarding engine successfully started");
	} else if (nvram_match("sfe", "4") && has_nss) { // ecm sfe
		rmmod("fast-classifier");
		rmmod("shortcut-fe-ipv6");
		rmmod("shortcut-fe");
		writeproc("/proc/ctf", "0");
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		eval("insmod", "qca-nss-sfe");
		insmod("hyfi-bridging");
		eval("insmod", "ecm", "front_end_selection=2");
#endif
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
		sysprintf("echo 1 > /proc/sys/dev/nss/general/redirect");
#endif
		dd_loginfo("ecm-nss", "shortcut (NSS-SFE) forwarding engine successfully started");
	} else if (nvram_match("sfe", "5") && has_nss) { // ecm sfe & nss
		rmmod("fast-classifier");
		rmmod("shortcut-fe-ipv6");
		rmmod("shortcut-fe");
		writeproc("/proc/ctf", "0");
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		eval("insmod", "qca-nss-sfe");
		insmod("hyfi-bridging");
		eval("insmod", "ecm", "front_end_selection=4");
#endif
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
		sysprintf("echo 1 > /proc/sys/dev/nss/general/redirect");
#endif
		dd_loginfo("ecm-nss", "Enhanced Connection Manager (ECM+NSS-SFE) forwarding engine successfully started");
	} else {
		rmmod("fast-classifier");
		rmmod("shortcut-fe-ipv6");
		rmmod("shortcut-fe");

#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv4_stop");
		sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv6_stop");
		sysprintf("echo 1 > /sys/kernel/debug/ecm/ecm_db/defunct_all");
#endif
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
		sysprintf("echo 0 > /proc/sys/dev/nss/general/redirect");
#endif
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
		rmmod("ecm");
		rmmod("hyfi-bridging");
		rmmod("qca-nss-sfe");
#endif
		writeproc("/proc/ctf", "0");
	}

	return;
}

void stop_sfe(void)
{
	rmmod("fast-classifier");
	rmmod("shortcut-fe-ipv6");
	rmmod("shortcut-fe");
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
	sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv4_stop");
	sysprintf("echo 1 > /sys/kernel/debug/ecm/front_end_ipv6_stop");
	sysprintf("echo 1 > /sys/kernel/debug/ecm/ecm_db/defunct_all");
#endif
#if !defined(HAVE_IPQ6018) && defined(HAVE_IPQ806X)
	sysprintf("echo 0 > /proc/sys/dev/nss/general/redirect");
#endif
#if defined(HAVE_IPQ6018) || defined(HAVE_IPQ806X)
	rmmod("ecm");
	rmmod("hyfi-bridging");
	rmmod("qca-nss-sfe");
#endif
	writeproc("/proc/ctf", "0");
	dd_loginfo("sfe", "shortcut forwarding engine successfully stopped");
	return;
}
#endif
