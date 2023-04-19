# FAT INJECTION PYTHON SCRIPT - v0.1
#
# Simple script, processes output of filefrag
# 
# 2023 Diogo Gomes

import sys
import os
import binascii

# Notice that we don't do any checking

# We expect the command line invocation to be: <script.py> file_in file_out partition_offset

# Open the file
fi = open(sys.argv[1], 'r')

# Discard first 3 lines
fi.readline()
fi.readline()
fi.readline()

# Keep final result
result = [[0,0]]

# Read until last line
line = fi.readline() # Read first, this is a do while
while line[0] == ' ':
    splitted = line.split()
    offset = splitted[3].replace('..','')
    length = splitted[5].replace(':','')
    result.append([offset,length])
    line = fi.readline()

fi.close()

# Write out file
fo = open(sys.argv[2], 'wb')

# First write signature
fo.write(b'\x41\x31')

# Second, write how many blocks there are
fo.write(len(result[1::]).to_bytes(2,'little'))

for element in result[1::]:
    offset = int(element[0]) + int(sys.argv[3])
    fo.write(offset.to_bytes(8,'little'))
    fo.write(int(element[1]).to_bytes(2, 'little'))

fo.close()