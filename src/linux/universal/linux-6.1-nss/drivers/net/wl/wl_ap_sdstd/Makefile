#
# Makefile for the Broadcom wl driver
#
# Copyright 2007, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.2 2008-08-11 23:05:54 $
#

SRCBASE_OFFSET := ../../../../brcm

ifeq ($(CONFIG_WL_AP_SDSTD),)
CONFIG_WL_AP_SDSTD := "wlconfig_lx_router_ap_sdstd"
endif
CONFIG_WL_CONF = $(CONFIG_WL_AP_SDSTD)

wl_suffix := ap_sdstd

TARGET := wl_$(wl_suffix)
WLCONF_H := ../wlconf.h
WLCONF_O := ../wlconf.o

ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
include $(src)/../wl_generic.mk
else
# Normal makefile
KERNELDIR := $(BASEDIR)/components/opensource/linux/linux-2.6.36
all:
	$(MAKE) -C $(KERNELDIR) M=`pwd`
clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` clean
include ../wl_generic.mk
endif
