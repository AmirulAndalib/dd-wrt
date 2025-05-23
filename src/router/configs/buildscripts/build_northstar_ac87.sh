#!/bin/sh
OLDPATH=$PATH
DATE=$(date +%m-%d-%Y)
DATE+="-r"
DATE+=$(svnversion -n northstar/src/router/httpd)
#export PATH=/xfs/toolchains/toolchain-laguna-new/bin:$OLDPATH

export PATH=/xfs/toolchains/toolchain-arm_cortex-a9_gcc-13.1.0_musl_eabi/bin:$OLDPATH
cd northstar/src/router
[ -n "$DO_UPDATE" ] && svn update
#cp .config_laguna-small .config
mkdir -p ~/GruppenLW/releases/$DATE/asus-rt-ac87u



cp .config_northstar .config
echo "CONFIG_SMP=y" >> .config
echo "CONFIG_QTN=y" >> .config
echo "CONFIG_SOFTETHER=y" >> .config
echo "CONFIG_IRQBALANCE=y" >> .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config
echo "CONFIG_RAID=y" >> .config
echo "CONFIG_SMBD=y" >> .config
echo "CONFIG_SMARTDNS=y" >> .config
echo "CONFIG_HTOP=y" >> .config
echo "CONFIG_IPSET=y" >> .config
echo "CONFIG_MDNS=y" >> .config
echo "CONFIG_MDNS_UTILS=y" >> .config
#echo "CONFIG_KERNELLTO=y" >> .config
#echo "CONFIG_KERNELLTO_CP_CLONE=y" >> .config
make -f Makefile.northstar kernel clean all install
#mkdir -p ~/GruppenLW/releases/$DATE/northstar
cd ../../../
#cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/$DATE/northstar
cp northstar/src/router/arm-uclibc/asus_rt-ac87u-firmware.trx ~/GruppenLW/releases/$DATE/asus-rt-ac87u

