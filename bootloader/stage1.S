# @file stage1.S
# @author Diogo Gomes
# @brief Main bootloader assembly stub, that sits in the FAT32 partition. Purpose is
# to set up the CPU to a known state, get information from the BIOS, and load
# the kernel, while providing some user input
# @version 0.2
# @date 2023-11-05
# 2023 Diogo Gomes



.code16
.global _start
.extern _multiboot_main

# Set constants
_stack_pointer = 0x6FFE
_stage0_memory_location = 0x0500
_info_store_location = 0x7000
_dap_memory_location = 0x1000
_fat_inject_magic = 0x3141
_fat_inject_magic_offset = 0x5A
_stage1_magic = 0x5926
_stage1_location = 0x3000
_multiboot_magic = 0x2BADB002


_int_15h_eax_const = 0xE820
_int_15h_edx_const = 0x534D4150

# Stage1 signature
.section .sign
.word 0x5926

.text
_start:
    # Set up the stack pointer (should already be set, but want to make sure)
    # We're setting it up right bellow us, at 30FE
    mov $_stack_pointer, %ax
    mov %ax, %sp
    mov %ax, %bp

    # Preserve %dl, it has the drive number
    push %dx

    leaw entring_stage1_msg, %si
    call write_loop

    # Now the fun begins. Must start querying BIOS for sizes
    # We'll use stage0's memory address to store the results
    mov $_info_store_location, %di # Is the destination

    # First the lower memory
    clc # Clear carry flag
    int $0x12 # Request size of lower memory
    leaw int12_error_msg, %si
    jc error
    mov %ax, (%di)

# Now the upper memory, using int 15h, %eax = 0xE820
# Need to use a loop to do it
# This trashes all registers
# Int 15h stores results in %di
get_upper_memory:
    #Loop setup
    add $6, %di # Save four bytes for upper mem entry count, increment 2 bytes from before
                # Add 4 bytes for length (each entry), store 24 there for simplicity    
    mov $24, %eax
    mov %eax, (%di)
    add $4, %di
    xor %bp, %bp # Use %bp as a counter, reset at the end
    xor %ebx, %ebx # ebx to zero
    mov $_int_15h_edx_const, %edx
    mov $_int_15h_eax_const, %eax
    movl $1, 20(%di) # Force a valid ACPI 3.X entry
    mov $24, %ecx # Read 24 bytes at a time
    int $0x15
    leaw int15_initial_error_msg, %si
    jc error # Carry set on first call means "unsupported function"
    mov $_int_15h_edx_const, %edx # Some BIOSes may trash this
    cmp %edx, %eax # On success, eax must have been reset to "SMAP"
    jne error
    leaw int15_short_error_msg, %si
    test %ebx, %ebx # If %ebx is zero, list is only 1 entry long (worthless)
    je error
    jmp .get_upper_memory_skip_loop

.get_upper_memory_loop:
    # Store lenght of entry (always 24 for simplicity)
    mov $24, %eax
    mov %eax, (%di)
    add $4, %di # Increment

.get_upper_memory_skip_length:
    mov $_int_15h_eax_const, %eax
    movl $1, 20(%di) # Force a valid ACPI 3.X entry
    mov $24, %ecx
    mov $_int_15h_edx_const, %edx # Some BIOSs may trash this, if they won't we do
    int $0x15
    jc get_upper_memory_done # Carry set means "end of list"

    
.get_upper_memory_skip_loop:
    jcxz .get_upper_memory_skip_entry
    cmp $20, %ecx # Is this a ACPI 3.X entry?
    jbe .get_upper_memory_20bytes
    testb $1, 20(%di) # If this is 3.X, then is the "ignore this data" bit set?
    je .get_upper_memory_skip_entry

.get_upper_memory_20bytes:
    mov 8(%di), %ecx # Lower uint32_t of memory region lenght
    or 12(%di), %ecx # Or with upper uint32_t to test for 0
    jz .get_upper_memory_skip_entry # Skip it
    inc %bp # Increment counter
    add $24, %di
    jmp .get_upper_memory_keep_entry

.get_upper_memory_skip_entry:
    test %ebx, %ebx # If %ebx is 0, reached the end
    jne .get_upper_memory_skip_length

.get_upper_memory_keep_entry:
    test %ebx, %ebx # Same as .get_upper_memory_skip_entry, but jump to beggining of loop
    jne .get_upper_memory_loop

get_upper_memory_done:
    mov $(_info_store_location + 2), %si
    mov %bp, (%si)
    clc
    add $120, %di # Leave space for the multiboot info structure

    # Restore %bp
    mov $_stack_pointer, %ax
    mov %ax, %bp

# Now we need to start reading the Kernel!
# We'll re-use the code from stage1. Right now, we'll just search for the only
# non-bootable partition (assume disk is one bootable with bootloader, and one)
# non-bootable with our kernel), and assume that's our OS.
# TODO: in the future, it would be nicer to search all partitions, test them all
# for bootable, and let the user choose from the ones we have, but that's for
# another day
# TODO: This can only load a small kernel. Ideally, would get an actual filesystem
# driver to get over limitations with our method, and load progressively into buffer,
# relocate to higher memory, return to real mode, and do like that until it's all
# read out like it should be

start_reading_kernel:
    # Reset disk controller, test for error
    pop %dx # Restore %dx - drive number
    mov $0, %ah
    int $0x13
    leaw int13_test_error_msg, %si
    jc error


    # First up, do the boot_loop jump from stage0, but with finding non-bootable
    mov $(_stage0_memory_location + 0x1be), %bx # First partition table entry offset
    mov $0b10000000, %cl # Mask
    mov $0, %dh # Counter

    .boot_loop:
        # Test bootable
        mov (%bx), %al
        andb %cl, %al
        cmp %cl, %al
        jne .kernel_partition_found

        # Didn't find it, loop to next
        # Test if we're not at 4
        mov $1, %al
        add %al, %dh
        mov $0x04, %al
        cmp %al, %dh
        je .kernel_partition_notFound

        mov $16, %ax
        add %ax, %bx
        jmp .boot_loop

    .kernel_partition_notFound:
        leaw kernel_partition_notFound_error_msg, %si
        jmp error

    .kernel_partition_found:
    # Now we need to read from the HDD

    # Read Boot Record
    mov %di, %fs
    mov $_dap_memory_location, %di 
    mov $1, %cx
    mov %bx, %si
    add $0x08, %si # LBA start in partition info
    mov (%si), %ebx
    call lba_read

    # Check for the Boot Record kernel signature
    mov %fs, %ax
    add $_fat_inject_magic_offset, %ax # Get offset of signature
    mov %ax, %di
    movw (%di), %ax
    movw $_fat_inject_magic, %bx
    leaw bootr_signature_notFound_error_msg, %si
    cmp %ax, %bx
    jne error


read_kernel:
    add $0x02, %di
    push %di # Save this for later, trust me bro
    mov (%di), %cx # Number of sectors to read

    #mov %fs, %ax
    #add $512, %ax
    #mov %ax, %fs

    .read_kernel_loop:
        push %cx

        # Start reading stuff to the DAP
        add $0x02, %di
        mov (%di), %ebx # LBA
        add $0x08, %di
        mov (%di), %cx # Sectors
        mov %di, %ax
        mov $_dap_memory_location, %di
        push %ax
        call lba_read

        # Increment %fs before restoring %cx: multiply by number of sectors
        mov $512, %ax
        push %dx
        mul %cx # Number of sectors read
        pop %dx
        mov %fs, %cx
        add %cx, %ax
        mov %ax, %fs
        pop %ax
        pop %cx
        mov %ax, %di

        sub $1, %cx
        jz .read_kernel_complete

        jmp .read_kernel_loop

    .read_kernel_complete:
    pop %ax # Initial value of pointer when we started
    mov $0, %dh
    push %dx # Save drive number

    mov %di, %bx
    sub %ax, %bx
    push %bx # Save it for laaaaater

    # We go to uncharted territory now - enabling the A20 gate
    leaw read_kernel_succesfull_msg, %si
    call write_loop


a20_gate:
    # Temporarily disable A20 so we can test enabling it!
    #mov $0x2400, %ax
    #int $0x15


    # First of all, we gotta test it
    call check_a20
    mov $1, %bx
    cmp %ax, %bx
    je a20_gate_enabled

    # Line is disabled
    leaw a20_line_disabled_msg, %si
    call write_loop

enable_20_gate:
    # We're gonna try, in order, int 0x15 enable, keyboard controller, and fast a20
    .int15_test:
        clc
        mov $0x2403, %ax
        int $0x15
        jc .keyboard_enable # Int 15h is not supported
        test %ah,%ah
        jnz .keyboard_enable # Int 15h is not supported

    .int15_enable:
        mov $0x2401, %ax
        int $0x15
        jc .keyboard_enable
        test %ah,%ah
        jnz .keyboard_enable

        # Test it
        call check_a20
        mov $1, %bx
        cmp %ax, %bx
        je a20_gate_enabled

    .keyboard_enable:
        leaw a20_line_attempt_keyboard_msg, %si
        call write_loop

        cli
        call a20_wait
        mov $0xad, %al
        out %al, $0x64

        call a20_wait
        mov $0xd0, %al
        out %al, $0x64

        call a20_wait_2
        in $0x60, %al
        push %eax

        call a20_wait
        mov $0xd1, %al
        out %al, $0x64

        call a20_wait
        pop %eax
        or $2,%al
        out %al, $0x60

        call a20_wait
        mov $0xae, %al
        out %al, $0x64

        call a20_wait
        sti

        # Test it
        call check_a20
        mov $1, %bx
        cmp %ax, %bx
        je a20_gate_enabled

    .fast_a20_enable:
        leaw a20_line_attempt_fast_msg, %si
        call write_loop

        in $0x92, %al
        test $2, %al
        jnz a20_gate_failed

        or $2, %al
        and $0xfe, %al
        out %al, $0x92

        # Test it
        call check_a20
        mov $1, %bx
        cmp %ax, %bx
        je a20_gate_enabled

a20_gate_failed:
    # We've exausted the options to enable the A20 gate, so we're giving up
    leaw a20_line_error_msg, %si
    jmp error

a20_gate_enabled:
    leaw a20_line_enabled_msg, %si
    call write_loop

    cli # Disable interrupts

load_GDT:
    mov $gdt_desc, %di
    lgdt (%di)

setup_protected:
    # Gonna setup jump to protected mode

    # Set the protection enable bit in %cr0
    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0

    # Preserve kernel size counter in %cx, drive number in %dx don't trust the stack
    xor %edx, %edx
    xor %ecx, %ecx
    pop %cx
    pop %dx

    # Long jump
    ljmp $CODE_SEGMENT, $enter_protected_mode

.code32
enter_protected_mode:
    # Load segment registers with data segment selector corresponding to data segment descriptor
    mov $DATA_SEGMENT, %eax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs
    mov %eax, %ss

    # Setup protected mode stack (same place, temporary)
    mov $0xf00, %eax
    mov %eax, %ebp
    mov %eax, %esp

    # We get here. Now is the time to load the multiboot header
    # We're leaving here legacy kernel_size as last parameter, can add to _multiboot_main at any time
    push %ecx
    push %edx

    # Figure out where kernel is buffered to
    # TODO we can improve this by storing initial location on stack
    mov $(_info_store_location + 0x2), %eax
    mov %eax,%esi
    mov (%esi), %eax
    mov $28, %ebx # Size of each mem_upper entry
    mul %ebx
    mov %esi, %ebx
    add %ebx, %eax
    add $124, %eax # We currently to the last mem entry, point instead to first kernel
                   # Also add 120 to account for multiboot info structure
    mov %eax, %esi


    # Get memory locations
    mov $_info_store_location, %eax
    mov $_info_store_location + 2, %ecx

    # Setup call to function
    push %ecx
    push %eax
    push %esi
    call _multiboot_main
    
    #addl $8, %esp # Discard calling variables

    # Deal with potential error condition: %eax == NULL
    test %eax,%eax
    je multiboot_loader_error

    # Set up multiboot state
    mov %eax, %ecx # Preserve pointer to kernel
    mov $_multiboot_magic, %eax # %eax must have the _multiboot_magic value
    pop %ebx # This minus 120 is the info structure
    sub $120, %ebx

    jmp *%ecx

multiboot_loader_error:
    cli
    hlt


# Useful functions, 16-bit real
.code16
write_loop:
    xor %bh, %bh #Set page 0 for int 10h
    lodsb #Load *(%si) to %al (and increment)
    orb %al, %al #Zero is end of string
    jz write_done
    mov $0x0e, %ah #Teletype mode
    int $0x10
    jmp write_loop

write_done:
    ret

error:
    call write_loop

    # Halt
    cli
    hlt

lba_read:
    # Inputs:
    # %ebx - 32-bit LBA address
    # %cx - sector count
    # %es:%fs - buffer
    # %dl - drive number
    # %di - DAP location

    # Set up the DAP
    movb $0x10, (%di) # Size of DAP
    add $1, %di
    movb $0, (%di) # Unused, should be 0
    add $1, %di
    movw %cx, (%di) # Sector count
    add $2, %di
    movw %fs, (%di)
    add $2, %di
    movw %es, (%di)
    add $2, %di
    mov %ebx, (%di) # LBA

    sub $8, %di
    mov %di , %si
    mov $0x42, %ah
    int $0x13

    # Test for error
    leaw read_error_msg, %si
    jc error

    ret

check_a20:
    # Function to check whether A20 is enabled
    # Returns:
    # %ax == 0 if A20 is disabled
    # %ax == 1 if A20 is enabled

    # Preserve state of registers
    pushf
    push %ds
    push %es
    push %di
    push %si
    cli

    # Where we are testing
    # 0x0000:0x0500(0x00000500) -> %ds:%si
    # 0xffff:0x0510(0x00100500) -> %es:%di
    xor %ax,%ax # %ax = 0
    mov %ax, %es

    not %ax # %ax = 0xffff
    mov %ax, %ds

    mov $0x0500, %di
    mov $0x0510, %si

    # Preserve what's at the locations
    mov %es:(%di), %al
    push %ax

    mov %ds:(%si), %al
    push %ax

    # Write stuff to these locations
    movw $0x00, %es:(%di)
    movw $0xff, %ds:(%si)
    
    # Compare
    cmpw $0xff, %es:(%di) # If equal, we've wrapped around

    # Restore locations
    pop %ax
    mov %al, %ds:(%si)
    pop %ax
    mov %al, %es:(%di)

    mov $0, %ax
    je .check_a20_exit

    mov $1, %ax # They're not equal, A20 is enabled

    .check_a20_exit:
    pop %si
    pop %di
    pop %es
    pop %ds
    popf

    ret

a20_wait:
    in $0x64, %al
    test $2, %al
    jnz a20_wait
    ret

a20_wait_2:
    in $0x64, %al
    test $1, %al
    jz a20_wait_2
    ret

# Messages
entring_stage1_msg:
    .asciz "Entering Stage1...\n\r"

read_kernel_succesfull_msg:
    .asciz "Reading kernel into memory was successful!\n\r"

testing_a20_line_msg:
    .asciz "Testing A20 line...\n\r"

a20_line_enabled_msg:
    .asciz "A20 line is enabled\n\r"

a20_line_disabled_msg:
    .asciz "A20 line is disabled, attempting to enable it...\n\r"

a20_line_attempt_keyboard_msg:
    .asciz "BIOS interrupt 15h failed, attempting keyboard controller...\n\r"

a20_line_attempt_fast_msg:
    .asciz "Keyboard controller method failed, attempting Fast A20...\n\r"

a20_line_error_msg:
    .asciz "Error: Could not enable A20 line, aborting"

int12_error_msg:
    .asciz "Error: Int 12h returned an error"

int15_initial_error_msg:
    .asciz "Error: First int 15h returned an error"

int15_short_error_msg:
    .asciz "Error: First int 15h returned 1 entry long list - worthless"

int13_test_error_msg:
    .asciz "Error: Int 13h initial test returned an error"

kernel_partition_notFound_error_msg:
    .asciz "Error: Couldn't find the kernel partition"

bootr_signature_notFound_error_msg:
    .asciz "Error: Couldn't find the boot record signature"

read_error_msg:
    .asciz "Error: Int 13h read failure"

test_msg:
    .asciz "We're here\n\r"

test2_msg:
    .asciz "This is a test message\n\r"


.code32
.data
gdt_start:
gdt_null_segment:
    .quad 0
gdt_code_segment:
    .word 0xffff
    .word 0x0000
    .byte 0
    .byte 0b10011010
    .byte 0b11001111
    .byte 0
    #.long 0x0000ffff
    #.long 0x00cf9a00
gdt_data_segment:
    .word 0xffff
    .word 0x0000
    .byte 0
    .byte 0b10010010
    .byte 0b11001111
    .byte 0
    #.long 0x0000ffff
    #.long 0x00cf9200
gdt_end:
.equ GDT_START, gdt_start # This is necessary, cause for some reason just doing
                          # .long gdt_start doesn't store the right address, and
                          # I can't figure out why, but this hack works
gdt_desc:
    .word gdt_end - gdt_start - 1
    .long GDT_START
    

.equ CODE_SEGMENT, gdt_code_segment - gdt_start
.equ DATA_SEGMENT, gdt_data_segment - gdt_start