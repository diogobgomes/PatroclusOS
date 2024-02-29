# KERNEL LIST OF OBJECT FILES
#
# List of objects to be built
#
#
# 2024 Diogo Gomes

OBJS:=boot.o \
kernel.o \
entry.o \
interrupts.o \
interruptHandler.o

LINKER:=$(ROOTSRCDIR)/kernel/kernel.ld

LIBS:=../lib64/lib64.ar