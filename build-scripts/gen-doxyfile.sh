#!/bin/bash
# HELPER SCRIPT FOR CMAKE
#
# Gens the final Doxyfile
# Expect invocation gen-doxyfile.sh ${Doxyfile.in} ${Doxyfile} ${Root_src_dir}
#
#
# 2023 Diogo Gomes

set -e
# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap cleanup ERR

cleanup() {
    error_code=$?
    echo "\"${last_command}\" command failed with exit code $?. Aborting!"
    exit $((error_code))
}

# Generate doxyfile
sed -e "s,@Doxygen_Output_Directory,./.," -e \
"s,@Doxygen_Input_Directory,$3," < $1 \
> $2