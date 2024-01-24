#!/bin/bash
# Definitions for config.sh
# Example file of how to specify programs for config.sh
# Copy this file, modify it for your purposes, and pass it to
# config.sh using --toolchain=<file>
# Must define all the variables here, except for the tooldirs (which are for convenience)
#
#
# 2024 Diogo Gomes

# Base dirs
tooldir=${HOME}/opt/x86_64-elf-cross/bin
tooldir32=${HOME}/opt/i686-elf-cross/bin

# Compilers
cc_compiler=${tooldir}/x86_64-elf-gcc
cxx_compiler=${tooldir}/x86_64-elf-g++
as_prog=${tooldir}/x86_64-elf-gcc
cc32_compiler=${tooldir32}/i686-elf-gcc
cxx32_compiler=${tooldir32}/i686-elf-g++
as32_prog=${tooldir32}/i686-elf-gcc

# General programs
ar_prog=ar
python_prog=python3
xorriso_prog=xorriso
grub_prog=grub2-mkrescue
grub_prog_check=grub2-file
qemu_prog=qemu-system-x86_64
doxygen_prog=doxygen