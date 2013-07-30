#! /usr/local/bin/python3

import re
import os
import sys
import sqlite3

songs = {}
names = {}
hashes = {}
trackers = {}

def createSong(modid, filename):
	if modid in songs:
		song = songs[modid]
	else:
		song = {'id': modid,
				'filename': filename,
				'size': None,
				'title': None,
				'format': None,
				'tracker': None,
				'downloads': None,
				'favourited': None,
				'score': None,
				'genre': None,
				'artist': None,
				'md5': None,
				'instruments': None,
				'channels': None,
				'orders': None,
				'samples': None,
				'patterns': None,
				'length': None
				}
		songs[modid] = song
	if filename not in names:
		names[filename] = song
	else:
		pass #print('Song "%s" is found (modid=%s)' % (filename, modid))
	return song

def read1000downloads():
	lines = open('top-1000-downloads.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, filename, downloads = x
		modid = int(modid)
		song = createSong(modid, filename)
		song['downloads'] = downloads

def read1000scores():
	lines = open('top-1000-score.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, filename, downloads = x
		modid = int(modid)
		if modid not in songs:
			song = createSong(modid, filename)
			song['downloads'] = downloads
		else:
			if filename != songs[modid]['filename'] or downloads!=songs[modid]['downloads']:
				print('Song %d is found ("%s","%s" <=> "%s","%s")' %
						(modid, songs[modid]['filename'], songs[modid]['downloads'], filename, downloads))
			else:
				pass #print('Song %d is found' % (modid))

def readScores():
	for score in (10,9,8,7,6,5,4,3,2,1):
		scoreFile = ('top-scores-%d.txt' % score)
		if not os.path.exists(scoreFile):
			continue
		lines = open(scoreFile,'r').readlines()
		for line in lines:
			line = line.rstrip()
			if line[0] == '#':
				continue
			x = line.split('|')
			modid, filename, score, total = x
			modid = int(modid)
			score = int(score)
			song = createSong(modid, filename)
			song['score'] = score

def readFavourited():
	favouritedFileName = 'top-favourited.txt'
	if not os.path.exists(favouritedFileName):
		return
	lines = open(favouritedFileName,'r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, filename, favourited = x
		modid = int(modid)
		favourited = int(favourited)
		song = createSong(modid, filename)
		song['favourited'] = favourited

def readMD5():
	for md5 in ('mod/md5.txt',
				'it/md5.txt',
				's3m/md5.txt',
				'xm/md5.txt',
				'oct/md5.txt',
				'okt/md5.txt',
				'669/md5.txt',
				'med/md5.txt',
				'mtm/md5.txt',
				'ahx/md5.txt',
				'hvl/md5.txt',
				'mo3/md5.txt',
				'stm/md5.txt'):
		if not os.path.exists(md5):
			continue
		lines = open(md5,'r').readlines()
		for line in lines:
			line = line.rstrip()
			if line[0] == '#':
				continue
			name = line[line.find('  ') + 2:]
			hash = line[:line.find(' ')]
			hashes[name] = hash

	print("Songs: %d" % (len(songs)))
	for modid in songs.keys():
		song = songs[modid]
		filename = song['filename']
		if filename not in hashes:
			print("No MD5: %s" % filename)
		else:
			song['md5'] = hashes.get(filename)

def readsongInfo():
	LOADING = re.compile('^Loading (.+)\\.\\.\\. \\(\\d+ of \\d+\\)$')
	MODULE_NAME = re.compile('^Module name\\s+:\\s+(.+)$')
	MODULE_TYPE = re.compile('^Module type\\s+:\\s+(.+)')
	ORDERS = re.compile('^Module length:\\s+(\\d+) patterns$')
	PATTERNS = re.compile('^Patterns\\s*:\\s+(\\d+)$')
	INSTRUMENTS = re.compile('^Instruments\\s*:\\s+(\\d+)$')
	SAMPLES = re.compile('^Samples\\s*:\\s+(\\d+)$')
	CHANNELS = re.compile('^Channels\\s*:\\s+(\\d+)')
	MODULE_LENGTH = re.compile('^Duration\\s*:\\s+(\\d+)min(\\d+)s')

	for info in ('mod/index.txt',
				'it/index.txt',
				's3m/index.txt',
				'xm/index.txt',
				'oct/index.txt',
				'okt/index.txt',
				'669/index.txt',
				'med/index.txt',
				'mtm/index.txt',
				'stm/index.txt'):
		if not os.path.exists(info):
			continue

		currentFileName = ''
		song = None

		try:
			lines = open(info,'r', encoding='cp1251').readlines()
		except:
			print("Error at file %s" % info)
			raise

		for line in lines:
			line = line.rstrip('\n')
			m = LOADING.search(line)
			if m:
				currentFileName = m.group(1)
				currentFileName = os.path.basename(currentFileName)

				if currentFileName in names:
					song = names[currentFileName]
				else:
					currentFileName = None
					song = None

			if song:
				m = MODULE_NAME.search(line)
				if m:
					song['title'] = m.group(1).strip()

				m = MODULE_TYPE.search(line)
				if m:
					tracker = m.group(1).strip()
					if tracker not in trackers:
						tracker_id = len(trackers) + 1
						trackers[tracker] = tracker_id
					else:
						tracker_id = trackers[tracker]

					song['tracker'] = tracker_id
					#song['tracker_name'] = tracker

				m = ORDERS.search(line)
				if m:
					song['orders'] = int(m.group(1))

				m = PATTERNS.search(line)
				if m:
					song['patterns'] = int(m.group(1))

				m = INSTRUMENTS.search(line)
				if m:
					song['instruments'] = int(m.group(1))

				m = SAMPLES.search(line)
				if m:
					song['samples'] = int(m.group(1))

				m = CHANNELS.search(line)
				if m:
					song['channels'] = int(m.group(1))

				m = MODULE_LENGTH.search(line)
				if m:
					minutes = int(m.group(1))
					seconds = int(m.group(2))
					song['length'] = (minutes * 60 + seconds) * 1000
					song = None

def basicInfo():
	formats = {
		'.mod':1,
		'.xm': 10,
		'.s3m': 8,
		'.stm': 9,
		'.it': 3,
		'.med': 4,
		'.oct': 6,
		'.okt': 7,
		'.ahx': 11,
		'.mtm': 5,
		'.mo3': 13,
		'.669': 2,
		'.hvl': 12
	}
	paths = {
		'.mod':'mod',
		'.xm': 'xm',
		'.s3m': 's3m',
		'.stm': 'stm',
		'.it': 'it',
		'.med': 'med',
		'.oct': 'oct',
		'.okt': 'okt',
		'.ahx': 'ahx',
		'.mtm': 'mtm',
		'.mo3': 'mo3',
		'.669': '669',
		'.hvl': 'hvl'
	}
	for modid in songs.keys():
		song = songs[modid]
		filename = song['filename']

		full_filename = filename
		for extension, directory in paths.items():
			if full_filename.endswith(extension):
				full_filename = ("%s/%s" % (directory, full_filename))
				if os.path.exists(full_filename):
					statinfo = os.stat(full_filename)
					song['size'] = statinfo.st_size

		for name, fmtid in formats.items():
			if filename.endswith(name):
				song['format'] = fmtid
				break

		if 'format' not in song or 'size' not in song:
			print(song)
			raise Exception

def createDatabase(databaseName):
	connection = sqlite3.connect(databaseName)
	cursor = connection.cursor()

	for tracker, tracker_id in trackers.items():
		cursor.execute('''INSERT INTO trackers (id, name) VALUES (?,?)''',
			(tracker_id, tracker))

	connection.commit()

	numSongsInserted = 0

	for modid, song in songs.items():
		filename = song['filename']
		title = song['title']
		size = song['size']
		length = song['length']

		if not size:
			print("NOT inserting %d (%s) - size is empty" % (modid, filename))
			continue

		if not length:
			print("NOT inserting %d (%s) - length is empty" % (modid, filename))
			continue

		if not title:
			song['title'] = filename

		if song['md5'] is None:
			print("NOT inserting %d (%s) - no MD5" % (modid, filename))
			continue

		if not (filename.endswith('.669') or
				filename.endswith('.it') or
				filename.endswith('.med') or
				filename.endswith('.mod') or
				filename.endswith('.mtm') or
				filename.endswith('.okt') or
				filename.endswith('.oct') or
				filename.endswith('.s3m') or
				filename.endswith('.stm') or
				filename.endswith('.xm')):
			continue

		#print("Inserting %d (%s) = %s" % (modid, song['filename'], song))
		cursor.execute('''INSERT INTO songs (id, fileName, size, title, format, length, tracker, genre,
 artist, downloads, favourited, score, md5, patterns, orders, instruments, samples, channels)
 VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)''',
		(modid,
		 song['filename'],
		 song['size'],
		 song['title'],
		 song['format'],
		 song['length'],
		 song['tracker'],
		 song['genre'],
		 song['artist'],
		 song['downloads'],
		 song['favourited'],
		 song['score'],
		 song['md5'],
		 song['patterns'],
		 song['orders'],
		 song['instruments'],
		 song['samples'],
		 song['channels']))

		numSongsInserted += 1

	print("Inserted songs: %d" % numSongsInserted)
	connection.commit()
	cursor.close()
	connection.close()


if __name__ == '__main__':
	if len(sys.argv) < 2:
		databaseFile = 'catalog.sqlite'
	else:
		databaseFile = sys.argv[1]

	read1000downloads()
	read1000scores()
	readScores()
	readFavourited()
	readMD5()
	readsongInfo()
	basicInfo()

	createDatabase(databaseFile)
