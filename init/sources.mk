# INIT LIST OF OBJECT FILES
#
# List of objects to be built
#
#
# 2024 Diogo Gomes

OBJS:=init.o \
main.o \
elfLoader.o \
longMode.o \
enterKernel.o \
paging.o

LINKER:=$(ROOTSRCDIR)/init/init.ld

LIBS:=../lib32/lib32.ar