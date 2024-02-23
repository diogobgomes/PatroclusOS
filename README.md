# PatroclusOS

This project is a small, hobbyist operating system/kernel designed for x86_64 architecture. It's built from scratch, as an educational project to teach myself operating system design.

## Features
- **x86_64 Architecture**
- **Included custom bootloader**: The project includes it's own custom (mostly) multiboot 1 compatible, that can be used to boot up the entire operating system. Support for any other multiboot 1 compatible bootloader is also available (tested in grub)
- **Written mostly in C++**: Only a minimal amount of assembly is used, simply to setup a C++ environment. Use the power of a high level language to do everything! No more cryptic assembly, or (arguably more aesthetically pleasing) C code.
- **Minimalist design**: Emphasis on simplicity and clarity, making it ideal for learning and experimentation. There's also some (attempts at) documentation, to help you understand what's going on

## Contributing
If someone actually wants to contribute to this, I'll be very (pleasantly) surprised, but contributions are of course welcome! Just open up a pull request, or an issue!

## Getting Started
Building this operating system takes some effort, unfortunately. In a future release, I plan to make a script that makes compiling the necessary toolchain easier. For now, there's some dependencies

### Dependencies
- Basic Linux common tools (ar, C/C++ compiler, bash, etc)
- GNU Make
- Python (to build diskimages, eventually we'll do it with bash)
- Grub2 and Xorriso (optional, can use grub as bootloader, but we have our own)
- qemu-system-x86_64 (optional, to emulate)
- Doxygen (optional, to build documentation)

BOCHS as an emulator is also supported, but there's no make bindings for it (yet)

### Building the toolchain
You need to build a 32-bit and 64-bit cross compiler toolchain supporting C and C++: see [here](https://wiki.osdev.org/GCC_Cross-Compiler) for more details. For the C++ 64-bit compiler, you must also follow the instructions [here](https://wiki.osdev.org/Libgcc_without_red_zone) to build libgcc with no red zone, to support exceptions (even though they aren't implemented just yet)

### Building the operating system
When you have your cross compiler, you can build the OS. First, you will have to specify where your toolchain (and other programs) are located, so the build system can find them. An example configuration file is located at ``config/toolchain.sh``, so copy it and edit it as you prefer. Now, create a build directory, ``cd`` into it, and run the ``config.sh`` script in that directory. You'll need to specify your toolchain file with the ``--toolchain=<file>`` command line option (run the ``--help`` option to see all available configuration options).

This will create all the necessary Makefiles in the directory it's run in. Now, just use Make to build the project!

Tip: running ``make qemu`` will build the diskimage, and start qemu, so you can experiment with the OS.


## Documentation

There are some docs [here](docs/README.md). Also, doxygen documentation is somewhat supported, just run ``make docs`` to build it (but first, you must follow the [Getting Started](#getting-started) guide)

## License
PatroclusOS is open-source software released under the MIT license. See the [LICENSE](LICENSE) file for more details.
