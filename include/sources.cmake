# List of header files, to be included in CMakeLists
#
#
# 2024 Diogo Gomes

set(
    HEADER_FILES
    bootloader/multiboot-loader.h
    klib/string.h
    klib/basic_io.h
    sys/elf.h
    sys/multiboot.h
    klib/hidden/stdio_format.h
    klib/stdlib.h
    kernel/kernel.h
)

list(TRANSFORM HEADER_FILES PREPEND ${CMAKE_SOURCE_DIR}/include/)