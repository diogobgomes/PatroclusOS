# BOOTLOADER LIST OF OBJECT FILES
#
# List of objects to be built
#
#
# 2024 Diogo Gomes

STAGE0_OBJS:=stage0.0.o

STAGE0_LINKER:=$(ROOTSRCDIR)/bootloader/stage0.ld

STAGE1_OBJS:=stage1.1.o \
main.1.o


STAGE1_LINKER:=$(ROOTSRCDIR)/bootloader/stage1.ld

LIBS:=../lib32/lib32.ar