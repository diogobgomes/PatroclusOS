#!/bin/bash

tools=/home/dbarrosgomes/Documents/Programming/OSDev/tools/cross/bin/i686-elf

$tools-as -o boot.o boot.S
$tools-ld --oformat binary -o boot.bin -Ttext 0x7C00 boot.o

qemu-system-i386 -hda boot.bin