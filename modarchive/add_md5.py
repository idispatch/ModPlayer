#!/usr/bin/python

import re
import sys
import os
import stat
import time
import sqlite3

if len(sys.argv) < 3:
	sys.exit(1)

database = sys.argv[1]
path = sys.argv[2]
print('Database: ' + database)
print('Processing: ' + path)

hashes={}

hashFile = open(os.path.join(path, 'md5.txt'))
lines=hashFile.readlines()
for line in lines:
	m = re.match('(\w+)  (.+)', line)
	if m:
		hashes[m.group(2)] = m.group(1)
hashFile.close()

print('Files: %s' % len(hashes))

conn=sqlite3.connect(database)
cursor=conn.cursor()

for fileName in os.listdir(path):
	normFileName = fileName.lower()
	if (not normFileName.endswith('.669') and
		not normFileName.endswith('.ahx') and
		not normFileName.endswith('.hvl') and
		not normFileName.endswith('.mod') and
		not normFileName.endswith('.mo3') and
		not normFileName.endswith('.it') and
		not normFileName.endswith('.xm') and
		not normFileName.endswith('.s3m') and
		not normFileName.endswith('.med') and
		not normFileName.endswith('.mtm') and
		not normFileName.endswith('.stm') and
		not normFileName.endswith('.oct') and
		not normFileName.endswith('.okt')):
		continue
	s = os.stat(os.path.join(path,fileName))
	mode = s.st_mode
	if not stat.S_ISREG(mode):
		continue
	if fileName in hashes:
		cursor.execute('UPDATE songs SET md5=? WHERE fileName=?', (hashes[fileName],fileName))

conn.commit()
cursor.close()
conn.close()
