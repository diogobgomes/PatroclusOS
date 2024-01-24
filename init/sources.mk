# INIT LIST OF OBJECT FILES
#
# List of objects to be built
#
#
# 2024 Diogo Gomes

OBJS:=init.o \
main.o \
init_common.o

LINKER:=$(ROOTSRCDIR)/init/init.ld

LIBS:=../lib32/lib32.ar