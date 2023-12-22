# BOOTLOADER LIST OF OBJECT FILES

# We just need the relative path, we'll perform build directory
# substitution later
BOOTLDRSRC:=$(BOOTLDROBJS) \
stage0.S \
stage1.S \

STAGE1SRC:=stage1.S \
multiboot-loader.c \
vga.c \
string.c