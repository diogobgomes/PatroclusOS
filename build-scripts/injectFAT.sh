#!/bin/bash
# HELPER SCRIPT FOR MAKEFILE
#
# Script to inject the file sectors into the FAT filesystem
# Boot Record. Expect to be run as root
# Expect invocation: injectFAT.sh $(ROOTSRC_DIR) $(PYTHON) source_file partition_offset target_diskimage current_user
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

tmpfile_in=".inject_tmpin"
tmpfile_out=".inject_tmpout"

# Get the information into the temporary file
filefrag -e $3 > ${tmpfile_in}

# Use python script to process the file (i know it's an ugly hack, I don't know bash)
sudo -u $6 $2 ${1}/build-scripts/injectFAT.py ${tmpfile_in} ${tmpfile_out} $4

# Now write it out to the diskfile
# Offset = partition_offset * 512 + 90, size = 420
sudo -u $6 dd if=${tmpfile_out} of=$5 conv=notrunc seek=$(($4 * 512 + 90)) bs=1 count=420

# Remove temporary files
sudo -u $6 rm -f ${tmpfile_in}
sudo -u $6 rm -f ${tmpfile_out}

# Done!
