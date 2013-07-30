#! /usr/local/bin/python3
import urllib
import urllib.request
import re

def query(query_type):
	print("# modid|filename|downloads")
	m=re.compile('href="http://modarchive\\.org/data/downloads\\.php\\?moduleid=(\\d+)#([^\"]+)">Downloaded ([\\d,]+) times!', re.MULTILINE)
	for page in range(1, 26):
		response = urllib.request.urlopen('http://modarchive.org/index.php?request=view_chart&query=%s&page=%d#mods' % (query_type, page))
		html = response.read()
		html = html.decode('utf-8')
		matches = m.findall(html)
		for match in matches:
			print("%s|%s|%s" % (match[0], match[1].lower(), match[2].replace(',','')))

def most_downloaded():
	query('tophits') # downloaded

def most_revered():
	query('topscore') # revered

def most_rated():
	m1 = re.compile('href="http://modarchive\\.org/data/downloads\\.php\\?moduleid=(\\d+)#([^\"]+)"', re.MULTILINE)
	m2 = re.compile("href='interactive\\.php\\?request=write_comment_page&amp;query=(\\d+)' class='middle'>Rated</a>([^<]+)</i>", re.MULTILINE)
	print("# modid|filename|score|maxscore")
	num_pages_by_score = {
		10:20,
		9:50,
		8:68,
		7:62,
		6:41,
		5:29,
		4:20,
		3:15}
	d = {}
	#for score in (10,9,8,7,6,5,4,3):
	for score in (4,):
		for page in range(1, num_pages_by_score[score] + 1):
			try:
				url = 'http://modarchive.org/index.php?request=view_by_rating_comments&query=%s&page=%d#mods' % (score, page)
				print("# %s" % url)
				response = urllib.request.urlopen(url)
				html = response.read()
				html = html.decode('utf-8')

				matches = m1.findall(html)
				for m in matches:
					modid = m[0]
					modname = m[1]
					d[modid] = modname.lower()

				matches = m2.findall(html)
				for m in matches:
					try:
						k = re.search('(\\d+) / (\\d+)', m[1].strip())
						modid = m[0]
						modscore = int(k.group(1))
						modhigh = int(k.group(2))
						print("%s|%s|%s|%s" % (modid, d[modid], modscore, modhigh))
					except Exception as e:
						print(e)
						print(m)
						print(matches)
						raise

			except Exception as e:
				print(e)
				print("Score: %d" % score)
				print("Page: %d" % page)
				print(d)
				raise


def most_favourited():
	m1 = re.compile('href="http://modarchive\\.org/data/downloads\\.php\\?moduleid=(\\d+)#([^\"]+)">Download</a>', re.MULTILINE)
	m2 = re.compile('href="module\\.php\\?(\\d+)#favourites">A favourite of (\\d+) members!', re.MULTILINE)

	print("# modid|filename|favourited")
	d = {}
	for page in range(1, 55):
		url = 'http://modarchive.org/index.php?format=&search_type=&query=&request=view_top_favourites&page=%d#mods' % page
		print("# %s" % url)

		response = urllib.request.urlopen(url)
		html = response.read()
		html = html.decode('utf-8')

		matches = m1.findall(html)
		for m in matches:
			modid = m[0]
			filename = m[1].lower()

			if modid in d:
				d[modid]['filename'] = filename
			else:
				d[modid] = {'filename':filename,
										'modid': modid}

		matches = m2.findall(html)
		for m in matches:
			modid = m[0]
			favourited = int(m[1])
			if modid in d:
				d[modid]['favourited'] = favourited
			else:
				d[modid] = {'favourited': favourited,
										'modid': modid}

	for modid, info in d.items():
		filename, favourited = info['filename'], info['favourited']
		print("%s|%s|%s" % (modid, filename, favourited))

most_rated()
#most_favourited()
