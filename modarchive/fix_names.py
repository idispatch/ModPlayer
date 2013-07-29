#! /usr/bin/python

import sys
import os
import re
import shutil

if len(sys.argv) > 1:
	directories = sys.argv[1:]
else:
	directories = ['.']

for directory in directories:
	files = os.listdir(directory)
	for fileName in files:
		fromFileName = fileName
		toFileName = fileName.lower()
		m = re.search('\s', toFileName)
		if m:
			toFileName = toFileName.replace(' ','_')
		if fromFileName != toFileName:
			fromFileName = os.path.join(directory, fromFileName)
			toFileName = os.path.join(directory, toFileName)
			print("%s -> %s"  % (fromFileName, toFileName))
			shutil.move(fromFileName, toFileName)
