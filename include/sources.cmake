# List of header files, to be included in CMakeLists
#
#
# 2024 Diogo Gomes

set(
    HEADER_FILES
    bootloader/multiboot-loader.h
    klib/string.h
    klib/vga.h
    sys/elf.h
    sys/multiboot.h
)

list(TRANSFORM HEADER_FILES PREPEND ${CMAKE_SOURCE_DIR}/include/)