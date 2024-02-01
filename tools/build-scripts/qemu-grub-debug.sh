#!/bin/bash
# HELPER SCRIPT FOR MAKE
#
# Script to launch qemu and gdb in the same command
# Launch with ./qemu-grub-debug.sh <iso>
#
#
# 2024 Diogo Gomes

set -m

qemu-system-x86_64 -cdrom ${1} -d int,cpu_reset -no-reboot -no-shutdown -s -S &

gdb