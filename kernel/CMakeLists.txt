# KERNEL CMAKE - v0.1
#
# Kernel CMakeLists.txt, to be included by the top level CMakeLists.txt
#
#
# 2023 Diogo Gomes

# Export compilation database
set( CMAKE_EXPORT_COMPILE_COMMANDS on )

# Preprocessor directives
add_compile_definitions(__kernel__)

add_custom_target(
    kernel.bin
    COMMAND cp ${CMAKE_SOURCE_DIR}/kernel/kernel.bin ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
)