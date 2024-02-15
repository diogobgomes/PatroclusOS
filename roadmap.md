# Roadmap of the Project
 
 Okay, what do we have to do?

 Right now, get some understanding

 Later on, get build system, organize project. We've got an hello world, get on it.

## Priorities
- [x] Bootloader
- [x] Project File structure
- [x] Build system

## Bootloader
I'm opting for a 2 stage design: stage0, the first 512 bytes, loads stage1, and that's our actual bootloader, that's then capable of loading the rest of the operating system.
I'm redesigning the bootloader, to be able to use C++
Stage0 will load stage1. Stage1 sits in the "free" space right after the first sector on the disk. The first partition only starts at either sector 1024 or 2048, so worst case we have 1024*512 bytes, or 512 KiB for stage1. That should be more than enough to work with, right? So the plan is that stage0 is just a small assembly stub (very minimal redesign), and stage1 will be a proper program, with c++ and everything. That will allows us to write a FAT32 driver "easily" (I expect great regret to come from these words), and properly load everything that'll be required, including modules. No more faffing around with assembly, and writing crappy info structures to the VBR.

- [ ] Stage0
- [ ] Stage1

## Kernel
- [x] GDT
- [x] You know what, we actually are gonna go for x86_64

## Other stuff
- [ ] For now, we're gonna ignore calling global constructors. It's just not worth it with how much effort needs to go in, with the x86_64 stuff. The workaround is to compile a special version of gcc with support for these features, see [webpage](https://wiki.osdev.org/Building_libgcc_for_mcmodel%3Dkernel)