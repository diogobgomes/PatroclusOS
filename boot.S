.code16
.global _start
_start:
    mov $0, %ax
    mov %ax, %ds #Data segment=0 because Ttext in loader takes care of it

    leaw msg, %si #Load msg address to si register
    cld #Clear load flags

    call write_loop

loop:
    jmp loop

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

msg:
    .asciz "Hello world!"

#Boot sector is 512 bytes long, 510 + 2 bytes for the signature
.org 510
.word 0xaa55