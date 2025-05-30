ifeq ($(CONFIG_MIPS),y)
ifeq ($(CONFIG_CPU_MIPS32_R1),y)
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel_r1/include -I$(srctree)/brcm/mipsel_r1/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel_r1/include -I$(srctree)/brcm/mipsel_r1/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel_r1/common/include
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel_r1/common/include
else
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel/include -I$(srctree)/brcm/mipsel/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel/include -I$(srctree)/brcm/mipsel/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_CFLAGS+=-I$(srctree)/brcm/mipsel/common/include
EXTRA_AFLAGS+=-I$(srctree)/brcm/mipsel/common/include
endif
else
EXTRA_CFLAGS+=-I$(srctree)/brcm/arm/include -I$(srctree)/brcm/arm/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_AFLAGS+=-I$(srctree)/brcm/arm/include -I$(srctree)/brcm/arm/shared/bcmwifi/include -DBCMDRIVER -Dlinux
EXTRA_CFLAGS+=-I$(srctree)/brcm/arm/common/include
EXTRA_AFLAGS+=-I$(srctree)/brcm/arm/common/include
endif
#
# Makefile for USB Host Controller Drivers
#

# tell define_trace.h where to find the xhci trace header
CFLAGS_xhci-trace.o := -I$(src)

fhci-y := fhci-hcd.o fhci-hub.o fhci-q.o
fhci-y += fhci-mem.o fhci-tds.o fhci-sched.o

fhci-$(CONFIG_FHCI_DEBUG) += fhci-dbg.o

xhci-hcd-y := xhci.o xhci-mem.o
xhci-hcd-y += xhci-ring.o xhci-hub.o xhci-dbg.o
xhci-hcd-y += xhci-trace.o

xhci-plat-hcd-y := xhci-plat.o
ifneq ($(CONFIG_USB_XHCI_MVEBU), )
	xhci-plat-hcd-y		+= xhci-mvebu.o
endif
ifneq ($(CONFIG_USB_XHCI_RCAR), )
	xhci-plat-hcd-y		+= xhci-rcar.o
endif

obj-$(CONFIG_USB_WHCI_HCD)	+= whci/

ifneq ($(CONFIG_USB), )
	obj-$(CONFIG_PCI)	+= pci-quirks.o
endif

obj-$(CONFIG_USB_ADM5120_HCD)	+= adm5120-hcd.o
obj-$(CONFIG_USB_EHCI_HCD)	+= ehci-hcd.o
obj-$(CONFIG_USB_EHCI_PCI)	+= ehci-pci.o
obj-$(CONFIG_USB_EHCI_HCD_PLATFORM)	+= ehci-platform.o
obj-$(CONFIG_USB_EHCI_MXC)	+= ehci-mxc.o
obj-$(CONFIG_USB_EHCI_HCD_OMAP)	+= ehci-omap.o
obj-$(CONFIG_USB_EHCI_HCD_ORION)	+= ehci-orion.o
obj-$(CONFIG_USB_EHCI_HCD_SPEAR)	+= ehci-spear.o
obj-$(CONFIG_USB_EHCI_HCD_STI)	+= ehci-st.o
obj-$(CONFIG_USB_EHCI_EXYNOS)	+= ehci-exynos.o
obj-$(CONFIG_USB_EHCI_HCD_AT91) += ehci-atmel.o
obj-$(CONFIG_USB_EHCI_MSM)	+= ehci-msm.o
obj-$(CONFIG_USB_EHCI_TEGRA)	+= ehci-tegra.o
obj-$(CONFIG_USB_W90X900_EHCI)	+= ehci-w90x900.o

obj-$(CONFIG_USB_OXU210HP_HCD)	+= oxu210hp-hcd.o
obj-$(CONFIG_USB_ISP116X_HCD)	+= isp116x-hcd.o
obj-$(CONFIG_USB_ISP1362_HCD)	+= isp1362-hcd.o

obj-$(CONFIG_USB_OHCI_HCD)	+= ohci-hcd.o
obj-$(CONFIG_USB_OHCI_HCD_PCI)	+= ohci-pci.o
obj-$(CONFIG_USB_OHCI_HCD_PLATFORM)	+= ohci-platform.o
obj-$(CONFIG_USB_OHCI_EXYNOS)	+= ohci-exynos.o
obj-$(CONFIG_USB_OHCI_HCD_OMAP1)	+= ohci-omap.o
obj-$(CONFIG_USB_OHCI_HCD_OMAP3)	+= ohci-omap3.o
obj-$(CONFIG_USB_OHCI_HCD_SPEAR)	+= ohci-spear.o
obj-$(CONFIG_USB_OHCI_HCD_STI)	+= ohci-st.o
obj-$(CONFIG_USB_OHCI_HCD_AT91)	+= ohci-at91.o
obj-$(CONFIG_USB_OHCI_HCD_S3C2410)	+= ohci-s3c2410.o
obj-$(CONFIG_USB_OHCI_HCD_LPC32XX)	+= ohci-nxp.o
obj-$(CONFIG_USB_OHCI_HCD_PXA27X)	+= ohci-pxa27x.o

obj-$(CONFIG_USB_UHCI_HCD)	+= uhci-hcd.o
obj-$(CONFIG_USB_FHCI_HCD)	+= fhci.o
obj-$(CONFIG_USB_XHCI_HCD)	+= xhci-hcd.o
obj-$(CONFIG_USB_XHCI_PCI)	+= xhci-pci.o
obj-$(CONFIG_USB_XHCI_PLATFORM) += xhci-plat-hcd.o
obj-$(CONFIG_USB_SL811_HCD)	+= sl811-hcd.o
obj-$(CONFIG_USB_SL811_CS)	+= sl811_cs.o
obj-$(CONFIG_USB_U132_HCD)	+= u132-hcd.o
obj-$(CONFIG_USB_R8A66597_HCD)	+= r8a66597-hcd.o
obj-$(CONFIG_USB_HWA_HCD)	+= hwa-hc.o
obj-$(CONFIG_USB_IMX21_HCD)	+= imx21-hcd.o
obj-$(CONFIG_USB_FSL_MPH_DR_OF)	+= fsl-mph-dr-of.o
obj-$(CONFIG_USB_EHCI_FSL)	+= ehci-fsl.o
obj-$(CONFIG_USB_HCD_BCMA)	+= bcma-hcd.o
obj-$(CONFIG_USB_HCD_SSB)	+= ssb-hcd.o
obj-$(CONFIG_USB_FOTG210_HCD)	+= fotg210-hcd.o
obj-$(CONFIG_USB_MAX3421_HCD)	+= max3421-hcd.o
