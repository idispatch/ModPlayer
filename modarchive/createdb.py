#! /usr/local/bin/python3

import re
import os
import sys
import sqlite3

artists = {}  # aidid -> artist object
songs = {}  # modid -> song object
names = {}  # fileName -> song object
hashes = {} # fileName -> md5 hash
trackers = {}
missing_files = {}

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

def createArtist(aid, name):
	if aid in artists:
		return artists[aid]
	artists[aid] = {'name':         name.strip(),
					'id':           int(aid),
					'songs':        {},
					'score':        None,
					'downloads':    None,
					'rating':       None,
					'avgdownloads': None}
	return artists[aid]

def createSong(modid, fileName):
	fileName = fileName.lower()
	if skipSongByExtension(fileName):
		return None
	fullFileName = fileExists(fileName)
	if not fullFileName:
		missing_files[modid] = fileName
		print("!!! Skipping %s (%d) - file does not exist" % (fileName, modid))
		return None
	if modid in songs:
		song = songs[modid]
	else:
		song = {'id':          int(modid),
				'filename':    fileName,
				'size':        None,
				'title':       None,
				'format':      None,
				'tracker':     None,
				'downloads':   None,
				'favourited':  None,
				'score':       None,
				'genre':       0,
				'artist':      0,
				'playCount':   0,
				'lastPlayed':  0,
				'myFavourite': 0,
				'instruments': None,
				'channels':    None,
				'orders':      None,
				'samples':     None,
				'patterns':    None,
				'length':      None
				}
		statinfo = os.stat(fullFileName)
		song['size'] = statinfo.st_size
		for name, fmtid in formats.items():
			if fileName.endswith(name):
				song['format'] = fmtid
				break
		songs[modid] = song
		if fileName in names:
			print("!!! Name %s already exists; existing id=%d, new id=%d" % (fileName, names[fileName]['id'], modid))
		else:
			names[fileName] = song
	return song

def read1000downloads():
	lines = open('top-1000-downloads.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, fileName, downloads = x
		modid = int(modid)
		song = createSong(modid, fileName.lower())
		if song:
			song['downloads'] = int(downloads)

def readGenres():
	lines = open('genres.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, genre, fileName, score = x
		modid = int(modid)
		genre = int(genre)
		song = createSong(modid, fileName.lower())
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
		modid, fileName, downloads = x
		modid = int(modid)
		song = createSong(modid, fileName.lower())
		if song:
			song['downloads'] = int(downloads)

def readScores():
	lines = open('top-scores.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		modid, fileName, score, total = x
		modid = int(modid)
		score = int(score)
		song = createSong(modid, fileName.lower())
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
		modid, fileName, favourited = x
		modid = int(modid)
		favourited = int(favourited)
		song = createSong(modid, fileName.lower())
		if song:
			song['favourited'] = favourited

def readMD5():
	return
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
			fileName = line[line.find('  ') + 2:]
			fileHash = line[:line.find(' ')]
			hashes[fileName] = fileHash
	for modid in songs.keys():
		song = songs[modid]
		fileName = song['filename']
		if fileName not in hashes:
			print("*** No MD5: %s" % fileName)
		else:
			song['md5'] = hashes[fileName]

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

	numArtistsInserted = 0
	for aid, artist in artists.items():
		name = artist['name']
		score = int(artist['score'])
		downloads = int(artist['downloads'])
		rating = int(artist['rating'])
		name = name.strip()
		cursor.execute('''INSERT INTO artists (id, name, score, downloads, rating) VALUES (?,?,?,?,?)''',
						(aid, name, score, downloads, rating))
		numArtistsInserted += 1
	connection.commit()

	numSongsInserted = 0
	for modid, song in songs.items():
		fileName = song['filename']
		title = song['title']
		size = song['size']
		length = song['length']

		if not size:
			print("!!! NOT inserting %d (%s) - size is empty" % (modid, fileName))
			continue

		if not length:
			print("!!! NOT inserting %d (%s) - length is empty" % (modid, fileName))
			continue

		if not title:
			song['title'] = fileName

		if not (fileName.endswith('.669') or
				fileName.endswith('.it') or
				fileName.endswith('.med') or
				fileName.endswith('.mod') or
				fileName.endswith('.mtm') or
				fileName.endswith('.okt') or
				fileName.endswith('.oct') or
				fileName.endswith('.s3m') or
				fileName.endswith('.stm') or
				fileName.endswith('.xm')):
			continue

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
	connection.commit()

	print("Inserted artists: %d" % numArtistsInserted)
	print("Inserted songs  : %d" % numSongsInserted)
	print("Missing songs   : %d" % len(missing_files))

	cursor.close()
	connection.close()

	missing = '\n'.join(['%s|%s' % (modid, fileName) for modid, fileName in missing_files.items()])
	missing = '# modid|filename\n' + missing
	open('missing-songs.txt','w').write(missing)

def readArtists():
	lines = open('artists.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		if len(x)!=2:
			print(line)
			raise
		aid, name = x
		artist = createArtist(int(aid), name)

	lines = open('top-artists.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		aid, name, score, downloads, rating, avgdownloads = x
		artist = createArtist(int(aid), name)
		artist['name'] = name
		artist['score'] = int(score)
		artist['downloads'] = int(downloads)
		artist['rating'] = int(rating)
		artist['avgdownloads'] = int(avgdownloads)

	lines = open('songs-by-artist.txt','r').readlines()
	for line in lines:
		line = line.rstrip()
		if line[0] == '#':
			continue
		x = line.split('|')
		aid, songid, fileName, score = x
		aid = int(aid)
		songid = int(songid)
		score = None if score == 'Unrated' else int(score)
		if '*' in fileName:
			continue
		song = createSong(songid, fileName)
		if not song:
			continue
		song['artist'] = aid
		artists[aid]['songs'][song['id']] = song
		if song['score'] is None:
			song['score'] = score
		if song['score'] is not None and score is not None and score != song['score']:
			print('*** Different score for song %s: %s and %s' % (song['id'], song['score'], score))
			song['score'] = max(song['score'], score)

	for aid in list(artists.keys()):
		if not artists[aid]['songs']:
			del artists[aid]

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
	readArtists()
	readMD5()
	readSongInfo()

	createDatabase(databaseFile)
