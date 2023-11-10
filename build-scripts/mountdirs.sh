#!/bin/bash
# HELPER SCRIPT FOR MAKEFILE
#
# Just a simple script designed to mount the necessary filesystems,
# supposed to be called by make
# Can't ouptut absolutely anything, due to make being absolutely
# awful
# To be called with: mountdirs.sh $(BMOUNTDIR) $(KMOUNTDIR) $(LOOPBACK) $(DISKIMAGE)
#
#
# 2023 Diogo Gomes

set -e

# First create the mountpoint
mkdir -p $1
mkdir -p $2

# Create loopback device
sudo losetup -P $3 $4 $5 &> /dev/null

# Mount devices
sudo mount ${3}p1 $1 &> /dev/null -o rw,umask=0000
sudo mount ${3}p2 $2 &> /dev/null -o rw,umask=0000
