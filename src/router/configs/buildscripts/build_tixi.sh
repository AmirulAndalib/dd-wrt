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
cp .config_tixi .config
#echo "CONFIG_RT3352=y" >> .config
make -f Makefile.rt2880 kernel clean all install
mkdir -p ~/GruppenLW/releases/$DATE/TIXI
cd ../../../
#cp rt2880/src/router/mipsel-uclibc/aligned.uimage ~/GruppenLW/releases/$DATE/WHR-G300N/firmware.bin
cp rt2880/src/router/mipsel-uclibc/rt2880-webflash.bin ~/GruppenLW/releases/$DATE/TIXI/RT3352-webflash.bin
cp rt2880/src/router/mipsel-uclibc/root.uimage ~/GruppenLW/releases/$DATE/TIXI/factory-to-ddwrt.bin
#cp rt2880/src/router/mipsel-uclibc/whrg300n-firmware.tftp ~/GruppenLW/releases/$DATE/WHR-G300N/whrg300n-firmware.tftp


