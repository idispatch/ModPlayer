#!/usr/bin/python

import re
import sys
import os
import stat
import time
import sqlite3

def formatTime(t):
	return time.strftime("%m/%d/%Y %I:%M:%S %p", time.localtime(t))

if len(sys.argv) < 3:
	sys.exit(1)

database = sys.argv[1]
path = sys.argv[2]

print('Database: %s' % database)
print('Processing: %s' % path)

formats = {'mod':1,
			'669':2,
			'it':3,
			'med':4,
			'mtm':5,
			'oct':6,
			'okt':7,
			's3m':8,
			'stm':9,
			'xm':10,
			'ahx':11,
			'hvl':12,
			'mo3':13}

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
	format = None
	_, fileExtension = os.path.splitext(fileName)
	if fileExtension:
		fileExtension = fileExtension[1:]
		if fileExtension:
			format = formats.get(fileExtension)

	size = s[stat.ST_SIZE]
	mtime = s[stat.ST_MTIME]
	#fileInfo = (fileName, size, format, mtime, formatTime(mtime))
	fileInfo = (fileName, size, format, mtime)
	cursor.execute('SELECT id FROM songs WHERE fileName=?', (fileName,))
	record_id = cursor.fetchone()
	if record_id:
		record_id = record_id[0]
		fileInfo = (fileName, size, format, mtime, record_id)
		print(fileInfo)
		cursor.execute('UPDATE songs SET fileName=?, length=?, format=?, created=? WHERE id=?', fileInfo)
	else:
		print(fileInfo)
		cursor.execute('INSERT INTO songs (fileName, length, format, created) VALUES (?,?,?,?)', fileInfo)

conn.commit()
cursor.close()
conn.close()
