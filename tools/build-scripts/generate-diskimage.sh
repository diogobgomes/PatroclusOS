#!/bin/bash
# HELPER SCRIPT FOR CMAKE
#
# Script to generate the final diskimage. Mounts the dirs, copies the files over,
# and injects the fat info
# Expect invocation generate-diskimage.sh $(diskimage) $(diskimage_loopback) \
#        $(rootsrcdir) $(bootloader_mount_dir) $(kernel_mount_dir) $(stage0) \
#        $(stage1) $(kernel) ${python}
#
#
# 2023 Diogo Gomes

set -e
# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap cleanup ERR

cleanup() {
    error_code=$?
    echo "\"${last_command}\" command failed with exit code $?. Aborting!"
    exit $((error_code))
}

# Get current user, to pass on to subscript
current_user=$(whoami)

diskimage=$1
diskimage_loopback=$2
rootsrcdir=$3
bootloader_mount_dir=$4
kernel_mount_dir=$5
stage0=$6
stage1=$7
kernel=$8
python=$9

# First off, create/mount the directories
#mkdir -p ${bootloader_mount_dir}
#mkdir -p ${kernel_mount_dir}
#if ! mountpoint -q ${bootloader_mount_dir} && ! mountpoint -q ${kernel_mount_dir}; then
#    echo "Filesystems not mounted, attempting to mount them"
#    if ! sudo ${rootsrcdir}/build-scripts/mountdirs.sh ${bootloader_mount_dir} \
#            ${kernel_mount_dir} ${diskimage_loopback} ${diskimage} ${current_user}; then
#        echo "Couldn't correctly mount the filesystems, aborting!"
#        exit 1
#    fi
#fi

if ! sudo ${rootsrcdir}/build-scripts/mountdirs.sh ${bootloader_mount_dir} \
          ${kernel_mount_dir} ${diskimage_loopback} ${diskimage} ${current_user}; then
    echo "Couldn't correctly mount the filesystems, aborting!"
    exit 1
fi

# Now, copy the files over
cp ${stage1} ${bootloader_mount_dir}/stage1.bin
cp ${kernel} ${kernel_mount_dir}/kernel.bin

# Inject the FAT info
echo "Injecting FAT info to the diskimage..."
dd if=${stage0} of=${diskimage} conv=notrunc bs=440 count=1
sudo -A ${rootsrcdir}/build-scripts/injectFAT.sh ${rootsrcdir} ${python} \
            ${bootloader_mount_dir}/stage1.bin 2048 ${diskimage} ${current_user}
sudo -A ${rootsrcdir}/build-scripts/injectFAT.sh ${rootsrcdir} ${python} \
            ${kernel_mount_dir}/kernel.bin 4096 ${diskimage} ${current_user}
sync

echo "Diskimage genereted successfully!"