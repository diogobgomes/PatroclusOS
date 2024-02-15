#!/bin/bash
# HELPER SCRIPT FOR MAKEFILE
#
# Just a simple script designed to mount the necessary filesystems,
# supposed to be called by make
# Can't ouptut absolutely anything, due to make being absolutely
# awful. Expect to be run as sudo
# To be called with: mountdirs.sh $(MOUNTDIR) $(LOOPBACK) $(DISKIMAGE) $(current_user)
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

# Variables
MOUNTDIR=$1
LOOPBACK=$2
DISKIMAGE=$3
USER=$4

# First create the mountpoint
sudo -u ${USER} mkdir -p ${MOUNTDIR}

# Test if they are mounted
if ! sudo -u ${USER} mountpoint -q ${MOUNTDIR}; then
    sudo -u ${USER} echo "Filesystem not mounted, attempting to mount it"
else
    sudo -u ${USER} echo "Filesystem mounted, skipping"
    exit 0
fi

# Create loopback device
#sudo -A losetup -P $3 $4 &> /dev/null
losetup -P ${LOOPBACK} ${DISKIMAGE} #&> /dev/null

# Mount device
mount ${LOOPBACK}p1 ${MOUNTDIR} &> /dev/null -o rw,umask=0000
#mount ${3}p2 $2 &> /dev/null -o rw,umask=0000
