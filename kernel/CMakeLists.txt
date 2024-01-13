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

add_library( crti_dummy STATIC crti.S )
add_library( crtn_dummy STATIC crtn.S )

add_custom_target( kcrti
    COMMAND ar -x libcrti_dummy.a
    VERBATIM
    BYPRODUCTS ctri.S.obj
    DEPENDS crti_dummy
)

add_custom_target( kcrtn
    COMMAND ar -x libcrtn_dummy.a
    VERBATIM
    BYPRODUCTS ctrn.S.obj
    DEPENDS crtn_dummy
)

#export(
#    TARGETS crti_dummy
#    FILE crti.o.cmake
#    NAMESPACE dummy
#)
#
#export(
#    TARGETS crtn_dummy
#    FILE crtn.o.cmake
#    NAMESPACE dummy
#)

#get_filename_component(FILE_NAME crti.S NAME_WE)
set_source_files_properties( crti.S PROPERTIES OBJECT_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} OBJECT_OUTPUT_NAME crti.o )


#include(${CMAKE_CURRENT_BINARY_DIR}/crti.o.cmake)
#include(crtn.i.cmake)

#set_target_properties( kcrti_dummy PROPERTIES RULE_LAUNCH_LINK "${CMAKE_SOURCE_DIR}" )

add_executable(kernel.bin)

target_sources( kernel.bin
    PUBLIC
    boot.S
    kernel.c
)

# Make sure dummy targets compile
# BUG This dependency is wrong, although it forces the compilation, it doesn't force the linking
# As the files crtx aren't likely to change often, and this is a pain, I'm not fixing it now
# If needed, just force linking to happen manually
add_dependencies( kernel.bin kcrti kcrtn )

#target_link_options( kernel.bin
#    PRIVATE
#    -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld
#    -nostdlib
#    -lgcc
#)

# Set link options manually because cmake isn't cooperating
set( KERNEL_LINK_OPTIONS
    "-T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld -nostdlib -lgcc"
)

target_link_libraries( kernel.bin lib.ar )

# Get full path of crtbegin.o and crtend.o
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_O OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_O OUTPUT_STRIP_TRAILING_WHITESPACE )
#set( CRTI_O "$<TARGET_OBJECTS:kcrti_dummy>")
set( CRTI_O "crti.o")

set( CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} <CMAKE_CXX_LINK_FLAGS> <FLAGS> ${CRTBEGIN_O} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> ${CRTEND_O} ${KERNEL_LINK_OPTIONS}" )
set( CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <CMAKE_C_LINK_FLAGS> <FLAGS> crti.S.obj ${CRTBEGIN_O} <OBJECTS> -o <TARGET> <LINK_LIBRARIES> ${KERNEL_LINK_OPTIONS} ${CRTEND_O} crtn.S.obj" )

#add_custom_target(
#    kernel.bin
#    COMMAND cp ${CMAKE_SOURCE_DIR}/kernel/kernel.bin ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
#)

#add_custom_target(list_objects ALL
#  COMMAND echo "Objects:" $<TARGET_OBJECTS:kcrti_dummy>
#  VERBATIM
#  )