# COMMENT CLEANUP PYTHON SCRIPT - v0.1
#
# Simple build system helper script that removes the initial comment block from
# the make configuration files used to build the final make script.
#
# THIS IS SIMPLY A TEMPORARY FIX, A STUPID ONE AT THAT
# Ultimately, of course we should use awk or sed, but I don't know either, and
# don't feel like learning right now, or loose any more sleep with this darn
# build system (I just wanna code :-( )
#
#
# 2023 Diogo Gomes

import sys

# Notice that we don't do any checking, please don't judge, this is really just 
# bodge. A bad one.

# We expect the command line invocation to be: <script.py> file_in file_out

fi = open(sys.argv[1], 'r')
fo = open(sys.argv[2], 'a')

line = fi.readline()

# Discard the first comments
while line[0] == '#':
	line = fi.readline()

while line:
	fo.write(line)
	line = fi.readline()

fi.close()
fo.close()
