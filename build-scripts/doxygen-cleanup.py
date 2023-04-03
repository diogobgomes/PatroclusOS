# DOXYGEN CLEANUP PYTHON SCRIPT - v0.1
#
# Simple build system helper script that configures the Makefile according to 
# whether doxygen is present or not: we choose between two options present on
# the Makefile.in for the 'docs' target
# 
# 2023 Diogo Gomes

import sys
import os

# Notice that we don't do any checking

# We expect the command line invocation to be: <script.py> file yes/no

fi = open(sys.argv[1], 'r')
ftmp = open(sys.argv[1]+'.tmp', 'w')

line = fi.readline()

# Read until we reach the desired spot
while line.find('<makedocs>') == -1:
	ftmp.write(line)
	line = fi.readline()

line = fi.readline() # Read another line

# Conditional
if sys.argv[2] == "yes":
	# Copy the relevant lines
	while line.find('<makedocs_error>') == -1:
		ftmp.write(line)
		line = fi.readline()
	# Ignore the error lines
	while line.find('<!makedocs>') == -1:
		line = fi.readline()
	line = fi.readline() # Leaves us in the next line to be written
else:
	# Ignore the doc lines
	while line.find('<makedocs_error>') == -1:
		line = fi.readline()
	line = fi.readline() # Read one more
	# Copy the relevant lines
	while line.find('<!makedocs>') == -1:
		ftmp.write(line)
		line = fi.readline()
	line = fi.readline() # Leaves us in the next line to be written

# Copy until the end
while line:
	ftmp.write(line)
	line = fi.readline()

# Close the files
fi.close()
ftmp.close()

# Replace the original with the tmp
os.replace(sys.argv[1]+'.tmp', sys.argv[1])