#! /usr/bin/python

import sys
import os

if len(sys.argv) > 1:
	directories = sys.argv[1:]
else:
	directories = ['.']

for directory in directories:
	[os.chmod(os.path.join(directory, fileName), 0644)
	 for fileName in os.listdir(directory)]
