# STAGE0
#
# Small assembly stub that sits in the MBR, and whose purpose it is
# to set up some basic things, load stage1, and jump to it
# 
#
# 2023 Diogo Gomes

.code16
.global _start
_start:
    cli # Disable interrupts

    # We want to load the correct value ($0) on all segment registers
    mov $0, %ax # Need an intermediate value, can't load directly to segments
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # We want now to copy ourselves to as low memory as possible
    mov $0x0100, %cx # 256 words in MBR
    mov $0x7C00, %si # Current MBR address
    mov $0x0500, %di # New MBR address: want to cut it finely
    rep movsw # Copy the MBR to the new address

    ljmp $0, $low_memory # Long jump to next line, setting %CS to 0

low_memory:
    # We must now set the stack pointer to a valid thing (but don't care about it)
    mov $0x7C00, %ax
    mov %ax, %sp
    mov %ax, %bp

    # Re-enable interrupts
    sti

    # Now we need to read and interpret the MBR partition table
    # Fortunately, it was loaded into memory with us
    # First thing, try to find some bootable partition table
    mov $(0x0500+0x1be), %bx # first partition table entry offset
    mov $0b10000000, %cl
    mov $0, %dh # Counter
boot_loop:
    # Test if it's bootable
    mov (%bx), %al
    andb %cl, %al
    cmp %cl, %al
    je bootable_found # Jump to the first bootable disk found
    
    # Didn't find it, loop to next
    # Test if we're not at 4
    mov $1, %al
    add %al, %dh
    mov $0x04, %al
    cmp %al, %dh
    je bootable_notFound

    mov $16, %ax
    add %ax, %bx
    jmp boot_loop


bootable_notFound:
    leaw bootable_notFound_error_msg, %si
    jmp error

bootable_found:
    # Now it's time to actually read stuff from a floppy (HDD)

    # Get everything set up for int 13h
    # First thing's first, reset the disk controller, test for error
    mov $0, %ah
    int $0x13
    # Test for error: jump if carry
    leaw reset_error_msg, %si
    jc error

    # TODO add check for extensions
    
    
    # READ
    mov $0x1000, %di
    mov $(0x1000 + 0x0010), %ax
    mov %ax, %fs
    mov $1, %cx
    mov %bx, %si
    add $0x08, %si # LBA start in partition info
    mov (%si), %ebx
    call lba_read


    # Check for the Boot Record stage1 signature
    mov $(0x1010 + 0x05A), %di
    movw (%di), %ax
    movw $0x3141, %bx
    leaw bootr_signature_notFound_error_msg, %si
    cmp %ax, %bx
    jne error

    # READ STAGE1
    # First need to get how many blocks we need
    add $0x02, %di
    mov (%di), %cx

    # Area we're going to read to
    mov $0x8000, %ax
    mov %ax, %fs

read_stage1_loop:
    push %cx # Preserve %cx, gonna use it

    # Start reading stuff to the DAP
    add $0x02, %di
    mov (%di), %ebx # LBA
    add $0x08, %di
    mov (%di), %cx # Sectors
    mov %di, %ax
    mov $0x1000, %di
    push %ax
    call lba_read

    # Increment %fs before restoring %cx : multiply by number of sectors
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
    jz read_stage1_complete

    jmp read_stage1_loop
    

read_stage1_complete:
    # Check for the stage1 signature
    #mov $0x8000, %di
    #movw (%di), %ax
    #movw $0x5926, %bx
    #leaw bootr_signature_notFound_error_msg, %si
    #cmp %ax, %bx
    #jne error

    # Everything is fine, let's jump to stage1!
    #jmp $0x8002
    
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

# TODO make this use stack, elegantly
error:
    call write_loop

    # Halt
    cli
    hlt

msg:
    .asciz "Here"

reset_error_msg:
    .asciz "Err: Reset contrlr"

read_error_msg:
    .asciz "Err: Read"

bootable_notFound_error_msg:
    .asciz "Err: Boot flg not found"

bootr_signature_notFound_error_msg:
    .asciz "Err: Boot Record sgn not found"


#Boot sector is 512 bytes long, 510 + 2 bytes for the signature
#.org 510
.org 510
.word 0xaa55
