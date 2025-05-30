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
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac56u
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac68u
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac1900p
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac88u
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac5300
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac3200
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac3100
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-n18u
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6700
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r7000
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6300v2
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6250
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6400
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6400v2
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r8000
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r7000P
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r8500
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-ex6200
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-ac1450
mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-1750dhp
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wxr-1900dhp
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-1166dhp
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-900dhp
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-600dhp2
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-reva
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-revb
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-revc
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir860l-a1
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir880l
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6900
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6300
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6350
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6400
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6700
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6500v2
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-811DRU
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-812DRUv2
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-818DRU
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-828DRU
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v1
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v2
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v3
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v4
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c1900
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c8v1
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir890l
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir895l
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir885l


#(cat .config_northstar | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
(cat .config_northstar | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_BRANDING=y\nCONFIG_TMK=y\nCONFIG_REGISTER=y\nCONFIG_STRACE=y\n" )>.config
echo "CONFIG_SMP=y" >> .config
echo "CONFIG_SOFTETHER=y" >> .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config

make -f Makefile.northstar kernel clean all install
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar
cd ../../../
#cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar
#cp northstar/src/router/arm-uclibc/asus_rt-ac56u-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac56u
#
#cp northstar/src/router/arm-uclibc/asus_rt-ac68u-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac68u
#
#cp northstar/src/router/arm-uclibc/asus_rt-ac68u-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac1900p/asus_rt-ac1900p-firmware.trx
#
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-1750dhp/buffalo-wzr-1750dhp-webflash.bin
cp northstar/src/router/arm-uclibc/buffalo-1750.encold ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-1750dhp/factory-to-dd-wrt.bin

exit 
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wxr-1900dhp/buffalo-wxr-1900dhp-webflash.bin
cp northstar/src/router/arm-uclibc/buffalo-1900.encold ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wxr-1900dhp/factory-to-dd-wrt.bin

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-1166dhp/buffalo-wzr-1166dhp-webflash.bin
cp northstar/src/router/arm-uclibc/buffalo-1166.encold ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-1166dhp/factory-to-dd-wrt.bin


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r7000/netgear-r7000-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R7000.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r7000/factory-to-dd-wrt.chk

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6700/netgear-r6700-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R6700.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6700/factory-to-dd-wrt.chk

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6300v2/netgear-r6300v2-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R6300V2.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6300v2/factory-to-dd-wrt.chk

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6250/netgear-r6250-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R6250.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6250/factory-to-dd-wrt.chk

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-ac1450/netgear-ac1450-webflash.bin
cp northstar/src/router/arm-uclibc/K3_AC1450.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-ac1450/factory-to-dd-wrt.chk


cp northstar/src/router/arm-uclibc/webflash-dir868.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-reva/dir868a-webflash.bin
cp northstar/src/router/arm-uclibc/webflash-dir868b.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-revb/dir868b-webflash.bin
cp northstar/src/router/arm-uclibc/webflash-dir868c.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-revc/dir868c-webflash.bin
cp northstar/src/router/arm-uclibc/webflash-dir860.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir860l-a1/dir860-webflash.bin

cp northstar/src/router/arm-uclibc/webflash-dir880.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir880l/dir880-webflash.bin

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6900/linksys-ea6900-webflash.bin
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6300/linksys-ea6300-webflash.bin
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6350/linksys-ea6350-webflash.bin
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6400/linksys-ea6400-webflash.bin
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6500v2/linksys-ea6500v2-webflash.bin
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/linksys-ea6700/linksys-ea6700-webflash.bin


cd northstar/src/router

(cat .config_northstar | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
echo "CONFIG_SMP=y" >> .config
echo "CONFIG_SOFTETHER=y" >> .config
echo "CONFIG_NVRAM_128K=y" >> .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config

make -f Makefile.northstar kernel clean all install
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar
cd ../../../
#cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar



cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6400/netgear-r6400-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R6400.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6400/factory-to-dd-wrt.chk
cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6400v2/netgear-r6400v2-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R6400V2.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r6400v2/factory-to-dd-wrt.chk


cd northstar/src/router
(cat .config_northstar_mini | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
make -f Makefile.northstar kernel clean all install
cd ../../../
cp northstar/src/router/arm-uclibc/web-dir868.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-reva/factory-to-ddwrt.bin
cp northstar/src/router/arm-uclibc/web-dir868b.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-revb/factory-to-ddwrt.bin
cp northstar/src/router/arm-uclibc/web-dir868c.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir868l-revc/factory-to-ddwrt.bin
cp northstar/src/router/arm-uclibc/web-dir860.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir860l-a1/factory-to-ddwrt.bin
cp northstar/src/router/arm-uclibc/web-dir880.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir880l/factory-to-ddwrt.bin

cp northstar/src/router/arm-uclibc/tnet812.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-812DRUv2/trendnet-812dru-webflash.bin


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-ex6200/netgear-ex6200-webflash.bin
cp northstar/src/router/arm-uclibc/K3_EX6200.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-ex6200/factory-to-dd-wrt.chk


cd northstar/src/router
(cat .config_northstar_16m | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
sed -i 's/\CONFIG_PRIVOXY=y/# CONFIG_PRIVOXY is not set/g' .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config



make -f Makefile.northstar kernel clean all install
cd ../../../

cp northstar/src/router/arm-uclibc/tnet818.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-818DRU/trendnet-818dru-webflash.bin

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v1/archer-c9v1-webflash.bin
cp northstar/src/router/arm-uclibc/archerc9.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v1/factory-to-ddwrt.bin


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v2/archer-c9v2-webflash.bin
cp northstar/src/router/arm-uclibc/archerc9v2.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v2/factory-to-ddwrt.bin


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v3/archer-c9v3-webflash.bin
cp northstar/src/router/arm-uclibc/tplink-ARCHER-C9-v3-firmware.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v3/factory-to-ddwrt.bin

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v4/archer-c9v4-webflash.bin
cp northstar/src/router/arm-uclibc/tplink-ARCHER-C9-v3-firmware.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c9v4/factory-to-ddwrt.bin


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c1900/archer-c1900-webflash.bin
cp northstar/src/router/arm-uclibc/archerc1900.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c1900/factory-to-ddwrt.bin

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c8v1/archer-c8-webflash.bin
cp northstar/src/router/arm-uclibc/archerc8.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/tplink-archer-c8v1/factory-to-ddwrt.bin


cd northstar/src/router
(cat .config_northstar_mini | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
echo "CONFIG_NORTHSTAR_NOSMP=y" >> .config
make -f Makefile.northstar kernel clean all install
cd ../../../
cp northstar/src/router/arm-uclibc/tnet811.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-811DRU/trendnet-811dru-webflash.bin


cd northstar/src/router

(cat .config_northstar | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config

echo "CONFIG_NORTHSTAR_NOSMP=y" >> .config
echo "CONFIG_SOFTETHER=y" >> .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config


make -f Makefile.northstar kernel clean all install
cd ../../../


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-900dhp/buffalo-wzr-900dhp-webflash.bin
cp northstar/src/router/arm-uclibc/buffalo-900.encold ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-900dhp/factory-to-dd-wrt.bin
cp northstar/src/router/arm-uclibc/buffalo-900.enc ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-900dhp/factory-to-dd-wrt-new.bin

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-600dhp2/buffalo-wzr-600dhp2-webflash.bin
cp northstar/src/router/arm-uclibc/buffalo-600.encold ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-600dhp2/factory-to-dd-wrt.bin
cp northstar/src/router/arm-uclibc/buffalo-600.enc ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/buffalo-wzr-600dhp2/factory-to-dd-wrt-new.bin

cp northstar/src/router/arm-uclibc/asus_rt-n18u-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-n18u
                                

cd northstar/src/router

(cat .config_northstar | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
echo "CONFIG_SMP=y" >> .config
echo "CONFIG_DHDAP=y" >> .config
echo "CONFIG_SOFTETHER=y" >> .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config

make -f Makefile.northstar kernel clean all install
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar
cd ../../../
#cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar

cp northstar/src/router/arm-uclibc/webflash-dir890.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir890l/dir890-webflash.bin
cp northstar/src/router/arm-uclibc/web-dir890.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir890l/factory-to-ddwrt.bin

cp northstar/src/router/arm-uclibc/webflash-dir895.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir895l/dir895-webflash.bin
cp northstar/src/router/arm-uclibc/web-dir895.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir895l/factory-to-ddwrt.bin

cp northstar/src/router/arm-uclibc/webflash-dir885.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir885l/dir885-webflash.bin
cp northstar/src/router/arm-uclibc/web-dir885.img ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/dlink-dir885l/factory-to-ddwrt.bin

cp northstar/src/router/arm-uclibc/asus_rt-ac88u-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac88u
cp northstar/src/router/arm-uclibc/asus_rt-ac5300-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac5300
cp northstar/src/router/arm-uclibc/asus_rt-ac3200-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac3200
cp northstar/src/router/arm-uclibc/asus_rt-ac3100-firmware.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/asus-rt-ac3100

cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r8000/netgear-r8000-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R8000.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r8000/factory-to-dd-wrt.chk


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r7000P/netgear-r8000-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R7000P.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r7000P/factory-to-dd-wrt.chk


cd northstar/src/router

(cat .config_northstar | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
echo "CONFIG_SMP=y" >> .config
echo "CONFIG_DHDAP=y" >> .config
echo "CONFIG_NVRAM_128K=y" >> .config
echo "CONFIG_SOFTETHER=y" >> .config
echo "CONFIG_SPEEDCHECKER=y" >> .config
echo "CONFIG_WIREGUARD=y" >> .config

make -f Makefile.northstar kernel clean all install
#mkdir -p ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar
cd ../../../
#cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/northstar


cp northstar/src/router/arm-uclibc/northstar-firmware-squashfs.bin ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r8500/netgear-r8500-webflash.bin
cp northstar/src/router/arm-uclibc/K3_R8500.chk ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/netgear-r8500/factory-to-dd-wrt.chk






cd northstar/src/router
(cat .config_northstar_16m | grep -v "^CONFIG_SQUID\|^CONFIG_WIKAR\|^CONFIG_KROMO\|^CONFIG_XIRIAN\|^CONFIG_BRAINSLAYER\|^CONFIG_NOCAT\|CONFIG_MC\|CONFIG_SNORT" ; printf "CONFIG_TMK=y\nCONFIG_NOTRIAL=y\nCONFIG_REGISTER=y\n" )>.config
echo "CONFIG_SMP=y" >> .config
echo "CONFIG_DHDAP=y" >> .config
sed -i 's/\CONFIG_SNORT=y/# CONFIG_SNORT is not set/g' .config
sed -i 's/\CONFIG_PHP=y/# CONFIG_PHP is not set/g' .config
sed -i 's/\CONFIG_ASTERISK=y/# CONFIG_ASTERISK is not set/g' .config
sed -i 's/\CONFIG_WEBSERVER=y/# CONFIG_WEBSERVER is not set/g' .config
sed -i 's/\CONFIG_PRIVOXY=y/# CONFIG_PRIVOXY is not set/g' .config
sed -i 's/\CONFIG_TRANSMISSION=y/# CONFIG_TRANSMISSION is not set/g' .config
make -f Makefile.northstar kernel clean all install
cd ../../../
cp northstar/src/router/arm-uclibc/tnet828.trx ~/GruppenLW/releases/CUSTOMER/$DATE/kmt/trendnet-828DRU/trendnet-828dru-webflash.bin
