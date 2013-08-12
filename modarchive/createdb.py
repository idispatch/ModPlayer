#! /usr/local/bin/python3

import re
import os
import sys
import sqlite3

songs = {}  # modid -> song object
names = {}  # fileName -> song object
hashes = {} # fileName -> md5 hash
trackers = {}

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

def fileExists(fileName):
	for extension, directory in paths.items():
		if fileName.endswith(extension):
			full_filename = ("%s/%s" % (directory, fileName))
			if os.path.exists(full_filename):
				return full_filename
			return None
	return None

def skipSongByExtension(fileName):
	return fileName.endswith('.mo3') or fileName.endswith('.ahx') or fileName.endswith('.hvl')

def createSong(modid, filename):
	if skipSongByExtension(filename):
		return None
	fullFileName = fileExists(filename)
	if not fullFileName:
		return None
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
				'playCount': 0,
				'lastPlayed': 0,
				'myFavourite': 0,
				'instruments': None,
				'channels': None,
				'orders': None,
				'samples': None,
				'patterns': None,
				'length': None
				}

		statinfo = os.stat(fullFileName)
		song['size'] = statinfo.st_size
		for name, fmtid in formats.items():
			if filename.endswith(name):
				song['format'] = fmtid
				break

		songs[modid] = song
		if filename in names:
			print("!!! Name %s already exists" % filename)
		else:
			names[filename] = song
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
		if song:
			song['downloads'] = int(downloads)

def readGenres():
	for i in range(1,150):
		fileName = 'genre-%d.txt' % i
		if not os.path.exists(fileName):
			continue
		lines = open(fileName,'r').readlines()
		for line in lines:
			line = line.rstrip()
			if line[0] == '#':
				continue
			x = line.split('|')
			try:
				modid, genre, filename, score = x
			except:
				print(x)
				raise
			modid = int(modid)
			genre = int(genre)
			song = createSong(modid, filename)
			if song:
				song['genre'] = int(genre)
				if score:
					song['score'] = int(score)

def read1000scores():
	lines = open('top-1000-score.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, filename, downloads = x
		modid = int(modid)
		song = createSong(modid, filename)
		if song:
			song['downloads'] = int(downloads)

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
			if song:
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
		if song:
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
				'stm/md5.txt'):
		if not os.path.exists(md5):
			continue
		lines = open(md5,'r').readlines()
		for line in lines:
			line = line.rstrip()
			if line[0] == '#':
				continue
			filename = line[line.find('  ') + 2:]
			filehash = line[:line.find(' ')]
			hashes[filename] = filehash
	for modid in songs.keys():
		song = songs[modid]
		filename = song['filename']
		if filename not in hashes:
			print("*** No MD5: %s" % filename)
		else:
			song['md5'] = hashes[filename]

def readSongInfo():
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
					#print("!!! File %s not found in names" % currentFileName)
					currentFileName = None
					song = None

			if song:
				m = MODULE_NAME.search(line)
				if m:
					v = m.group(1).strip()
					v = v.replace('  ', ' ')
					song['title'] = v

				m = MODULE_TYPE.search(line)
				if m:
					tracker = m.group(1).strip()
					if tracker not in trackers:
						tracker_id = len(trackers) + 1
						trackers[tracker] = tracker_id
					else:
						tracker_id = trackers[tracker]

					song['tracker'] = tracker_id

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
 artist, downloads, favourited, score, playCount, lastPlayed, myFavourite, patterns, orders, instruments, samples, channels)
 VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)''',
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
		 song['playCount'],
		 song['lastPlayed'],
		 song['myFavourite'],
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

	read1000downloads() # top-1000-downloads.txt:   # modid|filename|downloads
	read1000scores()    # top-1000-scores.txt:      # modid|filename|downloads
	readGenres()        # genre-xxx.txt:            # modid|genre|filename|score
	readScores()        # top-scores-xxx.txt:       # modid|filename|score|maxscore
	readFavourited()    # top-favourited.txt:       # modid|filename|favourited
	readMD5()
	readSongInfo()

	createDatabase(databaseFile)
