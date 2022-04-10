.code16
.global _start
_start:
    cli
    #Limpar coisos

loop:
    jmp loop

.org 510
.word 0xaa55