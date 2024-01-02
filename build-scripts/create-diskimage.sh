#!/bin/bash
# HELPER SCRIPT FOR CMAKE
#
# Script to generate the initial diskimage, and partition it
# It checks if it exists, and if so, doesn't generate it
# Expect invocation create-diskimage.sh $(diskimage) $(rootsrcdir) $(loopback)
#
#
# 2023 Diogo Gomes

set -e

diskimage=$1

if [ -f "$diskimage" ]; then
    echo "Diskimage ${diskimage} detected, skipping generation"
else
    echo "Diskimage ${diskimage} not detected, generating it..."
    echo "dd:"
    dd if=/dev/zero of=${diskimage} bs=1048576 count=12
    echo "Super user permissions are needed for this step"
    echo "sfdisk:"
    sudo -A sfdisk ${diskimage} < ${2}/config/diskimage.sfdisk
    echo "losetup:"
    sudo -A losetup -o $((2048*512)) $3 ${diskimage}
    echo "mkfs.fat:"
    sudo -A mkfs.fat -F 32 $3
    sudo -A losetup -d $3
    sudo -A losetup -o $((4096*512)) $3 ${diskimage}
    echo "mkfs.fat:"
    sudo -A mkfs.fat -F 32 $3
    sudo -A losetup -d $3
    echo "Diskimage ${diskimage} generated!"
fi