#!/bin/sh
OLDPATH=$PATH
DATE=$(date +%m-%d-%Y)
DATE+="-r"
DATE+=$(svnversion -n ks8695/src/router/httpd)
export PATH=/xfs/toolchains/toolchain-arm_arm922t_gcc-13.2.0_musl_eabi/bin:$OLDPATH
#export PATH=/xfs/toolchains/toolchain-arm_gcc4.2.3/bin:$OLDPATH
cd ks8695/src/router
[ -n "$DO_UPDATE" ] && svn update
cp .config_openrisc_erc .config
echo "CONFIG_WPA3=y" >> .config
make -f Makefile.openrisc kernel clean all install
mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/erc/Alekto_OpenRisc
cd ../../../
#cp ar531x/src/router/mips-uclibc/root.fs ~/GruppenLW/releases/$DATE/ubnt-ls5
#cp ar531x/src/router/mips-uclibc/vmlinux.bin.l7 ~/GruppenLW/releases/$DATE/ubnt-ls5/kernel
cp ks8695/src/router/arm-uclibc/disc.img ~/GruppenLW/releases/CUSTOMER/$DATE/erc/Alekto_OpenRisc/disc_cf.img
cp ks8695/src/router/arm-uclibc/disc_sd.img ~/GruppenLW/releases/CUSTOMER/$DATE/erc/Alekto_OpenRisc/disc_sd.img
cd ~/GruppenLW/releases/CUSTOMER/$DATE/erc/Alekto_OpenRisc 
zip -9 disc_sd.zip disc_sd.img
zip -9 disc_cf.zip disc_cf.img
rm -rf disc_sd.img disc_cf.img
echo /GruppenLW/releases/CUSTOMER/$DATE/erc/Alekto_OpenRisc
