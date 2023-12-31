# BOOTLOADER CMAKE - v0.1
#
# Bootloader CMakeLists.txt, to be included by the top level CMakeLists.txt
#
#
# 2023 Diogo Gomes

# Export compilation database
set( CMAKE_EXPORT_COMPILE_COMMANDS on )

# Preprocessor directives
add_compile_definitions(__bootloader__)

add_executable(
    stage0.bin
    stage0.S
)

target_link_options(
    stage0.bin PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/stage0.ld
)

add_executable(
    stage1.bin
    stage1.S
    multiboot-loader.c
    string.c
    vga.c
)

target_link_options(
    stage1.bin PRIVATE
    -T ${CMAKE_CURRENT_SOURCE_DIR}/stage1.ld
)

