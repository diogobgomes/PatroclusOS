#!/bin/bash
# HELPER SCRIPT FOR MAKEFILE
#
# Just a simple script designed to mount the necessary filesystems,
# supposed to be called by make
# Can't ouptut absolutely anything, due to make being absolutely
# awful
# To be called with: mountdirs.sh $(MOUNTDIR) $(LOOPBACK) $(DISKIMAGE)
#
#
# 2023 Diogo Gomes

set -e

# First create the mountpoint
mkdir -p $1

# Create loopback device
sudo losetup -P $2 $3 $4 &> /dev/null

# Mount device
sudo mount ${2}p1 $1 &> /dev/null -o rw,umask=0000
