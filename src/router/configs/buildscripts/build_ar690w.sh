#!/bin/sh
OLDPATH=$PATH
DATE=$(date +%m-%d-%Y)
DATE+="-r"
DATE+=$(svnversion -n rt2880/src/router/httpd)
export PATH=/xfs/toolchains/toolchain-mipsel_24kc_gcc-13.1.0_musl/bin:$OLDPATH
#export PATH=/xfs/toolchains/toolchain-mipsel_mips32_gcc-8.2.0_musl/bin:$OLDPATH
cd rt2880/src/router
[ -n "$DO_UPDATE" ] && svn update
cd opt/etc/config
[ -n "$DO_UPDATE" ] && svn update
cd ../../../
cp .config_ar690w .config
make -f Makefile.rt2880 kernel clean all install
mkdir -p ~/GruppenLW/releases/$DATE/airlink101-ar690w
cd ../../../
#cp rt2880/src/router/mipsel-uclibc/aligned.uimage ~/GruppenLW/releases/$DATE/WHR-G300N/firmware.bin
cp rt2880/src/router/mipsel-uclibc/raw-webflash.bin ~/GruppenLW/releases/$DATE/airlink101-ar690w/ar690w-ddwrt-webflash.bin
cp rt2880/src/router/mipsel-uclibc/ar690w-web.bin ~/GruppenLW/releases/$DATE/airlink101-ar690w/airlink-ar690w-factory-webflash.bin
#cp rt2880/src/router/mipsel-uclibc/whrg300n-firmware.tftp ~/GruppenLW/releases/$DATE/WHR-G300N/whrg300n-firmware.tftp


