#!/usr/bin/python

import re
import sys
import os
import stat
import time
import sqlite3
import pprint

if len(sys.argv) < 3:
	sys.exit(1)

database = sys.argv[1]
path = sys.argv[2]
print('Database: ' + database)
print('Processing: ' + path)

info={}

infoFile = open(os.path.join(path, 'index.txt'))
lines=infoFile.readlines()
infoFile.close()

n = 0
currentFile = ''

for n in range(len(lines)):
	line = lines[n]
	line = line.rstrip('\n')
	m = re.search('^Loading (.+)\\.\\.\\. \\(\\d+ of \\d+\\)$', line)
	if m:
		currentFile = m.group(1)
		currentFile = os.path.basename(currentFile)
		info[currentFile] = {}

	if currentFile:
		m = re.search('^Module name\\s+:\\s+(.+)$', line)
		if m:
			songName = m.group(1).strip()
			info[currentFile]['songName'] = songName

		m = re.search('^Module type\\s+:\\s+(.+)', line)
		if m:
			trackerType = m.group(1).strip()
			info[currentFile]['tracker'] = trackerType

		m = re.search('^Module length:\\s+(\\d+) patterns$', line)
		if m:
			lengthPatterns = int(m.group(1))
			info[currentFile]['lengthPatterns'] = lengthPatterns

		m = re.search('^Patterns\\s*:\\s+(\\d+)$', line)
		if m:
			numPatterns = int(m.group(1))
			info[currentFile]['numPatterns'] = numPatterns

		m = re.search('^Instruments\\s*:\\s+(\\d+)$', line)
		if m:
			numInstruments = int(m.group(1))
			info[currentFile]['numInstruments'] = numInstruments

		m = re.search('^Samples\\s*:\\s+(\\d+)$', line)
		if m:
			numSamples = int(m.group(1))
			info[currentFile]['numSamples'] = numSamples

		m = re.search('^Channels\\s*:\\s+(\\d+)', line)
		if m:
			numChannels = int(m.group(1))
			info[currentFile]['numChannels'] = numChannels

		m = re.search('^Duration\\s*:\\s+(\\d+)min(\\d+)s', line)
		if m:
			minutes = int(m.group(1))
			seconds = int(m.group(2))
			info[currentFile]['duration'] = minutes * 60 + seconds

print('Files: %s' % len(info))

conn=sqlite3.connect(database)
cursor=conn.cursor()

for fileName in info.keys():
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

	x = info[fileName]
	if 'tracker' in x:
		trackerName = x['tracker']
		try:
			cursor.execute('SELECT id FROM trackers WHERE name=?', (trackerName,))
			tracker_id = cursor.fetchone()
			if tracker_id:
				tracker_id = tracker_id[0]
			else:
				cursor.execute('INSERT INTO trackers (name) VALUES (?)', (trackerName,))
				cursor.execute('SELECT id FROM trackers WHERE name=?', (trackerName,))
				tracker_id = cursor.fetchone()
				if tracker_id:
					tracker_id = tracker_id[0]
				else:
					tracker_id = None
		except:
			trackerName = None
			tracker_id = None
	else:
		trackerName = None
		tracker_id = None

	if all([attribute in x
			for attribute in ('songName',
					'lengthPatterns',
					'numInstruments',
					'numSamples',
					'numChannels',
					'numPatterns')]):
		try:
			cursor.execute('''UPDATE songs
						SET songName=?,
							tracker=?,
							pattern_length=?,
							instruments=?,
							samples=?,
							channels=?,
							patterns=?
						WHERE fileName=?''',
					(x['songName'],
					tracker_id,
					x['lengthPatterns'],
					x['numInstruments'],
					x['numSamples'],
					x['numChannels'],
					x['numPatterns'],
					fileName))
			print(fileName)
		except:
			print('*** Skipping %s...' % fileName)

conn.commit()
cursor.close()
conn.close()
