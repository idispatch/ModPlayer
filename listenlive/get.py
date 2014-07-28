#! /usr/bin/python2.7
# -*- coding: utf-8 -*-
from __future__ import print_function
import sys
import io
import re
import subprocess
import json
import HTMLParser
import codecs

countries = {
'andorra.html': 'Andorra',
'armenia.html': 'Armenia',
'austria.html': 'Austria',
'azerbaijan.html': 'Azerbaijan',
'belarus.html': 'Belarus',
'belgium.html': 'Belgium',
'bosnia.html': 'Bosnia-Herzegovina',
'bulgaria.html': 'Bulgaria',
'croatia.html': 'Croatia',
'cyprus.html': 'Cyprus',
'czech-republic.html': 'Czech Republic',
'denmark.html': 'Denmark',
'estonia.html': 'Estonia',
'faroe.html': 'Faroe Islands',
'finland.html': 'Finland',
'france.html': 'France',
'georgia.html': 'Georgia',
'germany.html': 'Germany',
'gibraltar.html': 'Gibraltar',
'greece.html': 'Greece',
'hungary.html': 'Hungary',
'iceland.html': 'Iceland',
'ireland.html': 'Ireland',
'italy.html': 'Italy',
'kosovo.html': 'Kosovo',
'latvia.html': 'Latvia',
'liechtenstein.html': 'Liechtenstein',
'lithuania.html': 'Lithuania',
'luxembourg.html': 'Luxembourg',
'macedonia.html': 'Macedonia',
'malta.html': 'Malta',
'moldova.html': 'Moldova',
'monaco.html': 'Monaco',
'montenegro.html': 'Montenegro',
'netherlands.html': 'Netherlands',
'norway.html': 'Norway',
'poland.html': 'Poland',
'portugal.html': 'Portugal',
'romania.html': 'Romania',
'russia.html': 'Russia',
'san-marino.html': 'San Marino',
'serbia.html': 'Serbia',
'slovakia.html': 'Slovakia',
'slovenia.html': 'Slovenia',
'spain.html': 'Spain',
'sweden.html': 'Sweden',
'switzerland.html': 'Switzerland',
'turkey.html': 'Turkey',
'ukraine.html': 'Ukraine',
'uk.html': 'United Kingdom',
'vatican.html': 'Vatican State'
}

def sanitize(i):
	try:
		parser = HTMLParser.HTMLParser()
		return [unicode(parser.unescape(x)).strip() for x in i]
	except:
		print(i)
		raise

def getcountries(f):
	for line in io.open(name, 'rt', encoding='utf-8').readlines():
		m = re.search(r'<a href="([^"/]+)">([^<]+)</a>', line)
		if m:
			if m.group(2) == 'email me':
				continue
			print("{0}|{1}".format(*m.groups()))
			url = '/'.join(['www.listenlive.eu',m.group(1)])
			args = ['wget', url, '-O', m.group(1)]
			print(args)
			subprocess.call(args)

def getstations(names):
	stations = []
	for name in names:
		#contents = io.open(name, 'r', encoding='utf-8').read()
		try:
			contents = codecs.open(name, "r", "utf-8").read()
		except:
			print("Could not read file " + name)
			raise
		for row in re.findall('<tr>(.+?)</tr>', contents, re.MULTILINE | re.DOTALL):
			cols = re.findall('<td[^>]*>(.+?)</td>', row, re.MULTILINE | re.DOTALL)
			if cols and len(cols) == 5:
				td1 = re.search(r'<a href="([^"]+)">\s*<b>([^<]+)</b>\s*</a>', cols[0], re.MULTILINE | re.DOTALL)
				td2 = re.match('([^<]+)', cols[1], re.MULTILINE | re.DOTALL)
				td5 = re.match('([^<]+)', cols[4], re.MULTILINE | re.DOTALL)
				if td1 and td2 and td5:
					url, sname = sanitize(td1.groups())
					location = sanitize(td2.groups())
					style = sanitize(td5.groups())
					td3 = re.findall(r'<img src="([^"]+)" width="12" height="12" alt="([^"]+)"\s*/>', cols[2], re.MULTILINE | re.DOTALL)
					td4 = re.findall(r'<a href="([^"]+)">([^<]+)</a>', cols[3], re.MULTILINE | re.DOTALL)
					if td3 and td4:
						#for icon, format in td3:
						format = 'MP3'
						for playlist, bitrate in td4:
							if playlist[:4].lower() == 'mms:':
								continue
							if playlist[:10].lower() == 'javascript':
								continue
							if playlist[-4:].lower() in ('.m3u', '.pls'):
								bitrate = int(re.search(r'(\d+)', bitrate).group(1))
								u = {
									'url': url,
									'name': sname,
									'location': location[0],
									'style': style[0],
									'format': format,
									'playlist': playlist,
									'bitrate': bitrate,
									'country': countries[name]
								}
								stations.append(u)

	stations.sort(key=lambda x: (x['country'], x['name'], x['format'], x['bitrate']))
	result = json.dumps(stations,
						indent=2,
						encoding='UTF-8',
						sort_keys=True,
						skipkeys=True,
						ensure_ascii=False,
						check_circular=True)
	print(result.encode('utf-8'))
	return stations

def dbscript(name):
	def escape_sql(s):
		try:
			return unicode(s).replace(u"'", u"''")
		except:
			print("Could not encode " + s, file=sys.stderr)
			raise
	try:
		data = codecs.open(name, "r", encoding="utf8").read()
		data = json.loads(data, encoding="utf8")
	except:
		print("Could not read file " + name)
		raise

	countries = set([s[u'country'] for s in data])
	countries = list(countries)
	countries.sort()
	sql_countris = {}
	for i, name in enumerate(countries):
		sql_countris[name] = i + 1
		name = escape_sql(name)
		s = u"INSERT INTO country (countryId, name) VALUES ({0}, '{1}');".format(i + 1, name)
		print(s.encode('utf-8'))
	print()

	styles = set([s[u'style'] for s in data])
	styles = list(styles)
	styles.sort()
	sql_styles = {}
	for i, name in enumerate(styles):
		sql_styles[name] = i + 1
		name = escape_sql(name)
		s = u"INSERT INTO radio_style (styleId, name) VALUES ({0}, '{1}');".format(i + 1, name)
		print(s.encode('utf-8'))
	print()

	for i, d in enumerate(data):
		try:
			s = (u"INSERT INTO radio (radioId, name, playlist, bitrate, countryId, location, styleId, url) VALUES "
			"({0}, '{1}', '{2}', {3}, {4}, '{5}', {6}, '{7}');".format(
				i + 1,
				escape_sql(d['name']),
				escape_sql(d['playlist']),
				d['bitrate'],
				sql_countris[d['country']],
				escape_sql(d['location']),
				sql_styles[d['style']],
				escape_sql(d['url'])))
			print(s.encode('utf-8'))
		except:
			print(d, file=sys.stderr)
			raise
	print()

if __name__ == '__main__':
	if len(sys.argv) < 3:
		sys.exit(1)
	elif sys.argv[1] == '-c': # countries from index.html
		getcountries(sys.argv[1])
	elif sys.argv[1] == '-s': # stations from country.html
		getstations(sys.argv[2:])
	elif sys.argv[1] == '-d': # stations from all.json
		dbscript(sys.argv[2])