# LIB CMAKE - v0.1
#
# Lib CMakeLists.txt, to be included by the top level CMakeLists.txt
#
#
# 2023 Diogo Gomes

# Export compilation database
set( CMAKE_EXPORT_COMPILE_COMMANDS on )

# Preprocessor directives
add_compile_definitions(__lib__)

include(${CMAKE_SOURCE_DIR}/include/sources.cmake)

# VARIABLES
set(
    LIB_SRC_LIST
    c/basic_io/basic_io.c
    c/string/string.c
    c/hidden/stdio_format.c
    c/stdlib/xtoa.c
    c/stdlib/abort.c
    cpp/basic_cpp_io/terminal.cpp
)

add_library(
    lib.ar STATIC
    ${LIB_SRC_LIST}
    ${HEADER_FILES}
)

target_include_directories(lib.ar PUBLIC ${CMAKE_SOURCE_DIR}/include)