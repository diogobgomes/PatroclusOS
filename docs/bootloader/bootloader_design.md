# Some musings about the design of the bootloader

Like before, we'll have a two stage design: stage0 still sits in the mbr, but now stage1 sits in the space right after. You know there's _at least_ 1024 sectors free before the first partition, right? Excluding the MBR, that 1023 sectors, or approximately 511 KiB, which is more than enough for any reasonable person to use for a stage1. 

That significantly simplifies stage0, since now it just has to set up the basic CPU state, and then jump to stage1, which will do all of the heavy lifting. To help with that, we're actually going to be using C++ to write it now, which will allow such niceties as a FAT32 "driver" that can read the filesystem properly.

**An important note on using C++**: the GCC compiler doesn't actually like to obey what we tell it to do. So, we cannot really return from C++ code assembly gracefully (the compiler will emit an invalid opcode for the _ret_ instruction, prepended with _0x66_, which makes it go wrong). So, instead of the easy way to return, we always have to call a function. Actually, not even call it, I think only in protected mode, everything else must be inline assembly (though I guess you could hack a proper return that way?). Sigh... Still worth it to be able to use C++.

**IMPORTANT NOTE** Change of plans. 16-bit real mode cpp code is reeeeeally buggy. I think it's easier to have a 16-bit real mode call function, and deal with that. Otherwise, we severely have to limit the heap size

## Stage 0

Stage 0 now only needs to do a few things:
1) [x] Copy itself to as low memory as possible
2) [x] Canonicalize %CS:%EIP
3) [x] Load segment registers (%DS, %ES, %FS, %GS, %SS)
4) [x] Set the stack pointer
5) [x] Reset the HDD controller
6) [x] Read stage1 sectors from the disk
7) [x] Jump to it

If anything fails, we must:
1) [x] Warn the user of a failure
2) [x] Disable interrupts
3) [x] Halt

## Stage 1

Stage 1 does all of the heavy lifting:
1) [x] Set up the C++ execution environment
Now, in C++:
2) [x] Enable the A20 gate (and set up way to enter and leave protected mode)
3) [x] Query the BIOS for the size of lower memory
4) [x] Query the BIOS for a memory map of upper memory
5) [x] Read the FAT32 filesystem, and locate the kernel and init
6) [x] Read the kernel and init into memory
7) [x] Copy/load them into high memory using protected mode (these two may involve a loop)
8) [x] Set up the multiboot execution environment
9) [x] Begin execution of init

If anything fails, we must:
1) [x] Warn the user of a failure
2) [x] Disable interrupts
3) [x] Halt

Stage 1 will have a header, which will help stage0 load it. It's very simply two bytes of magic (0x3141), and two bytes with the number of sectors to load, so four bytes in total.
This specifically means that stage1's entry point is shifted by 4 bytes

As a technical note, we'll also be (initially) limiting stage1 to 127 sectors (or 512*127 = 63.5 KiB)

## Memory map
We'll use the following memory map for the bootloader:
| Address       | Purpose       |
| ------------- | ------------- |
| 0x0 - 0x4ff   | Reserved - BIOS |
| 0x500 - 0x7ff | Stack (a whole 1 KiB of it!) |
| 0x800 - 0x9ff | Final location of stage0 |
| 0x1000 - 0x109f | DAP memory location |
| 0x1010 - ?     | Stage1 location (directly loaded here)
| ? - 0x6ffff   | "Heap", location to read kernel to |
| 0x70000 - 0x7ffff | Disk reading area: 64 KiB, boundary aligned, as it's supposed to be |
| 0x80000 - 0xfffff | Reserved - Various things |