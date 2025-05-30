#!/bin/sh
# Copyright (C) 2006-2012 OpenWrt.org
set -e -x
if [ $# -ne 5 ] && [ $# -ne 7 ]; then
    echo "SYNTAX: $0 <file> <kernel size> <kernel directory> <rootfs size> <rootfs image> [<align>]"
    exit 1
fi

OUTPUT="$1"
KERNELSIZE="$2"
KERNELDIR="$3"
KERNELPARTTYPE=${KERNELPARTTYPE:-83}
ROOTFSSIZE="$4"
ROOTFSIMAGE="$5"
ROOTFSPARTTYPE=${ROOTFSPARTTYPE:-83}
ALIGN="$6"
PART3FSSIZE="$7"
PART3FSPARTTYPE=${PART3FSPARTTYPE:-83}
rm -f "$OUTPUT"

head=16
sect=63

# create partition table
set $(ptgen -o "$OUTPUT" -h $head -s $sect ${GUID:+-g} -t "${KERNELPARTTYPE}" -p "${KERNELSIZE}m${PARTOFFSET:+@$PARTOFFSET}" -t "${ROOTFSPARTTYPE}" -p "${ROOTFSSIZE}m" -t "${PART3FSPARTTYPE}" -p "${PART3FSSIZE}m" ${ALIGN:+-l $ALIGN} ${SIGNATURE:+-S 0x$SIGNATURE} ${GUID:+-G $GUID})

KERNELOFFSET="$(($1 / 512))"
KERNELSIZE="$2"
ROOTFSOFFSET="$(($3 / 512))"
ROOTFSSIZE="$(($4 / 512))"
PART3FSOFFSET=$(($5 / 512))
PART3FSSIZE=$(($6 / 512))

# Using mcopy -s ... is using READDIR(3) to iterate through the directory
# entries, hence they end up in the FAT filesystem in traversal order which
# breaks reproducibility.
# Implement recursive copy with reproducible order.
dos_dircopy() {
  local entry
  local baseentry
  for entry in "$1"/* ; do
    if [ -f "$entry" ]; then
      mcopy -i "$OUTPUT.kernel" "$entry" ::"$2"
    elif [ -d "$entry" ]; then
      baseentry="$(basename "$entry")"
      mmd -i "$OUTPUT.kernel" ::"$2""$baseentry"
      dos_dircopy "$entry" "$2""$baseentry"/
    fi
  done
}

[ -n "$PADDING" ] && dd if=/dev/zero of="$OUTPUT" bs=512 seek="$PART3FSOFFSET" conv=notrunc count="$PART3FSSIZE"
dd if="$ROOTFSIMAGE" of="$OUTPUT" bs=512 seek="$ROOTFSOFFSET" conv=notrunc

if [ -n "$GUID" ]; then
    [ -n "$PADDING" ] && dd if=/dev/zero of="$OUTPUT" bs=512 seek="$((ROOTFSOFFSET + ROOTFSSIZE + PART3FSSIZE))" conv=notrunc count="$sect"
    echo "test"
    mkfs.fat --invariant -n kernel -C "$OUTPUT.kernel" -S 512 "$((KERNELSIZE / 1024))"
    echo "test"
    LC_ALL=C dos_dircopy "$KERNELDIR" /
    echo "test"
else
    make_ext4fs -J -L kernel -l "$KERNELSIZE" ${SOURCE_DATE_EPOCH:+-T ${SOURCE_DATE_EPOCH}} "$OUTPUT.kernel" "$KERNELDIR"
fi
dd if="$OUTPUT.kernel" of="$OUTPUT" bs=512 seek="$KERNELOFFSET" conv=notrunc
rm -f "$OUTPUT.kernel"
