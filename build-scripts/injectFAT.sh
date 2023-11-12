#!/bin/bash
# HELPER SCRIPT FOR MAKEFILE
#
# Script to inject the file sectors into the FAT filesystem
# Boot Record
# Expect invocation: injectFAT.sh $(ROOTSRC_DIR) $(PYTHON) source_file partition_offset target_diskimage
#
#
# 2023 Diogo Gomes

set -e

tmpfile_in=".inject_tmpin"
tmpfile_out=".inject_tmpout"

# Get the information into the temporary file
sudo filefrag -e $3 > ${tmpfile_in}

# Use python script to process the file (i know it's an ugly hack, I don't know bash)
$2 ${1}/build-scripts/injectFAT.py ${tmpfile_in} ${tmpfile_out} $4

# Now write it out to the diskfile
# Offset = partition_offset * 512 + 90, size = 420
dd if=${tmpfile_out} of=$5 conv=notrunc seek=$(($4 * 512 + 90)) bs=1 count=420

# Remove temporary files
rm -f ${tmpfile_in}
rm -f ${tmpfile_out}

# Done!
