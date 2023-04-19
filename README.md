# PatroclusOS

This is the README for PatroclusOS. Now that's taken care of

Why don't you head over to the [roadmap](roadmap.md) and actually get something done?

Alternatively, go see some [docs](docs/README.md)?

## Dependencies
- Cross compiler toolchain supporting C and C++: see [here](https://wiki.osdev.org/GCC_Cross-Compiler) for more details
- Python
- Xorriso
- make
- Read fat dirs
- grub2-file (optional, to check for multiboot)
- qemu-system-i386 (optional, to emulate)

## Instructions on how to connect QEMU to gdb

Start QEMU with:
```Shell
qemu-system-i386 -s -S -drive file=file,format=raw
```

QEMU now starts up, but waits for a gdb connection over HTTP on ``localhost:1234``.

Now, start gdb with ``gdb``. Now, type ``target remote localhost:1234`` to connect to gdb, and then simply type ``c`` to continue execution in gdb

### Useful commands
- ``info reg``: provide list of values in registers
- ``set architecture i8086``: set 8086 real mode architecture for disassembly
- ``x/10i $cs*16+$eip``: show next instructions
- ``x/nfu addr``: show memory at location addr, *n* times *u* units of memory (*b* = bytes, *h* = halfwords, *ẁ* = words, *g* = giant), using format *f* (*i* for machine instructions, *m* for memory tags, *x* for hexadecimal)