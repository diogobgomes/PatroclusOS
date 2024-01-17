//#include <klib/basic_io.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/kernel.hpp>
#include <klib/basic_cpp_io.hpp>
#include <sys/multiboot.h>

void kmain(uint32_t multibootMagic, multiboot_info_structure* multibootInfo) {
    //TODO this will assume kernel is loaded by multiboot, no checking
    //asm("mov $0x3142,%eax;");
    //asm("cli;");
    //asm ("hlt;");
    //terminal_initialize();
    //terminal_clear();
    //terminal_write_string("What up?");
    basic_io::terminal defaultTerminal;
    defaultTerminal.clear();
    defaultTerminal.printf("PatroclusOS v0.1\n");
    //defaultTerminal.putEntryAt('A',0,0);
    //defaultTerminal.write("Yup yup! :)");
    defaultTerminal.write("Now for a true test \n\nDid you see that?\n");

    defaultTerminal.printf("We'll try something... %i is an integer, and 0x%x is an hex, and %s is a string\n",34,0x47f,"yoooooo");
    //defaultTerminal.scroll(1);
    defaultTerminal.printf("Multiboot magic (moment of truth): 0x%x",multibootMagic);

    defaultTerminal.printf("\n\nMultiboot info, wanna check some numbers out");
    defaultTerminal.printf("mem_lower = 0x%x, mem_upper = 0x%x, boot_device = 0x%i\n",multibootInfo->mem_lower,multibootInfo->mem_upper,multibootInfo->boot_device);
    defaultTerminal.printf("mmap_addr = 0x%x\n",multibootInfo->mmap_addr);



    mmap_structure_entry* mmap = (mmap_structure_entry*) multibootInfo->mmap_addr;
    size_t mmap_length = multibootInfo->mmap_length;

    //++mmap;

    uint32_t base_addr = mmap->base_addr;
    uint32_t length = mmap->length;
    uint32_t type = mmap->type;

    defaultTerminal.printf("Let's try to print out the first mmap:\nentry_length = 0x%x\nbase_addr = 0x%x\nlength = 0x%x\nType = 0x%x\nACPI = 0x%x\n",mmap->entry_length,base_addr,length,type,mmap->acpi_3_0_extended);

}
