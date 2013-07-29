import urllib
import urllib.request
import re

m=re.compile('href="http://modarchive\\.org/data/downloads\\.php\\?moduleid=(\\d+)#([^\"]+)">Downloaded ([\\d,]+) times!', re.MULTILINE)
m1 = re.compile('href="http://modarchive\\.org/data/downloads\\.php\\?moduleid=(\\d+)#([^\"]+)"', re.MULTILINE)
m2 = re.compile("href='interactive\\.php\\?request=write_comment_page&amp;query=(\\d+)' class='middle'>Rated</a>([^<]+)</i>", re.MULTILINE)

def query(query_type):
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
	for score in (7,):
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

most_rated()