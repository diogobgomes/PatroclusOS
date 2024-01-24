#!/bin/bash
# HELPER SCRIPT FOR MAKEFILE
#
# Just a simple script designed to mount the necessary filesystems,
# supposed to be called by make
# Can't ouptut absolutely anything, due to make being absolutely
# awful. Expect to be run as sudo
# To be called with: mountdirs.sh $(BMOUNTDIR) $(KMOUNTDIR) $(LOOPBACK) $(DISKIMAGE) $(current_user)
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

# First create the mountpoint
sudo -u $5 mkdir -p $1
sudo -u $5 mkdir -p $2

# Test if they are mounted
if ! sudo -u $5 mountpoint -q $1 && ! sudo -u $5 mountpoint -q $2; then
    sudo -u $5 echo "Filesystems not mounted, attempting to mount them"
else
    exit 0
fi

# Create loopback device
#sudo -A losetup -P $3 $4 &> /dev/null
losetup -P $3 $4 #&> /dev/null

# Mount devices
mount ${3}p1 $1 &> /dev/null -o rw,umask=0000
mount ${3}p2 $2 &> /dev/null -o rw,umask=0000
