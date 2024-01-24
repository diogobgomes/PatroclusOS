# Roadmap of the Project
 
 Okay, what do we have to do?

 Right now, get some understanding

 Later on, get build system, organize project. We've got an hello world, get on it.

## Priorities
- [x] Bootloader
- [x] Project File structure
- [x] Build system

## Bootloader
I'm opting for a 2 stage design: stage0, the first 512 bytes, loads stage1, and that's our actual bootloader, that's then capable of loading the rest of the operating system

- [x] Stage0
- [x] Stage1

## Kernel
- [ ] GDT
- [ ] You know what, we actually are gonna go for x86_64

## Other stuff
- [ ] For now, we're gonna ignore calling global constructors. It's just not worth it with how much effort needs to go in, with the x86_64 stuff. The workaround is to compile a special version of gcc with support for these features, see [webpage](https://wiki.osdev.org/Building_libgcc_for_mcmodel%3Dkernel)