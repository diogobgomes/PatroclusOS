# Roadmap of the Project
 
 Okay, what do we have to do?

 Right now, get some understanding

 Later on, get build system, organize project. We've got an hello world, get on it.

## Priorities
- [ ] Bootloader
- [x] Project File structure
- [x] Build system

## Bootloader
I'm opting for a 2 stage design: stage0, the first 512 bytes, loads stage1, and that's our actual bootloader, that's then capable of loading the rest of the operating system

- [ ] Stage0
- [ ] Stage1