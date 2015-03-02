PRAGMA foreign_keys=1;
PRAGMA page_size=8192;
PRAGMA cache_size=2000;
PRAGMA case_sensitive_like=false; 
PRAGMA user_version=5;

CREATE TABLE artists
(
	id INTEGER PRIMARY KEY,
	name TEXT NOT NULL,
	score INT,
	downloads INT,
	rating INT
);

CREATE TABLE genres
(
	id INTEGER PRIMARY KEY,
	name TEXT NOT NULL
);

CREATE TABLE albums
(
    id INTEGER PRIMARY KEY,
    artistId INT,
    name TEXT NOT NULL,
    CONSTRAINT FK_albums_artists FOREIGN KEY (artistId) REFERENCES artists (id) ON DELETE RESTRICT ON UPDATE RESTRICT
);

CREATE TABLE formats
(
	id INTEGER PRIMARY KEY,
	name TEXT NOT NULL,
	description TEXT NOT NULL
);

CREATE TABLE trackers
(
	id INTEGER PRIMARY KEY,
	name TEXT NOT NULL
);

CREATE TABLE songs
(
	id INTEGER PRIMARY KEY,
	fileName TEXT UNIQUE,
	size INTEGER  NOT NULL,
	title TEXT NOT NULL,

	format INTEGER NOT NULL,
	length INTEGER NOT NULL,
	tracker INTEGER NOT NULL,
	genre INTEGER,
	artist INTEGER,

	downloads INTEGER,
	favourited INTEGER,
	score INTEGER,

	playCount INTEGER,
	lastPlayed INTEGER,
	myFavourite INTEGER,

	patterns INTEGER NOT NULL,
	orders INTEGER NOT NULL,
	instruments INTEGER NOT NULL,
	samples INTEGER NOT NULL,
	channels INTEGER NOT NULL,
	
	CONSTRAINT FK_songs_formats FOREIGN KEY (format) REFERENCES formats (id) ON DELETE RESTRICT ON UPDATE RESTRICT,
    CONSTRAINT FK_songs_trackers FOREIGN KEY (tracker) REFERENCES trackers (id) ON DELETE RESTRICT ON UPDATE RESTRICT,
    CONSTRAINT FK_songs_genres FOREIGN KEY (genre) REFERENCES genres (id) ON DELETE RESTRICT ON UPDATE RESTRICT,
    CONSTRAINT FK_songs_artists FOREIGN KEY (artist) REFERENCES artists (id) ON DELETE RESTRICT ON UPDATE RESTRICT
);

CREATE TABLE playlists
(
	id INTEGER PRIMARY KEY,
	name TEXT NOT NULL
);

CREATE TABLE playlistEntries
(
	playlistId INTEGER,
	songId INTEGER,
    songOrder INTEGER,
    CONSTRAINT PK_playlistEntries PRIMARY KEY (playlistId,songId,songOrder),
    CONSTRAINT FK_playlistEntries_playlists FOREIGN KEY (playlistId) REFERENCES playlists (id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT FK_playlistEntries_songs FOREIGN KEY (songId) REFERENCES songs (id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE albumEntries
(
    albumId INTEGER,
    songId INTEGER,
    trackNumber INTEGER,
    CONSTRAINT PK_albumEntries PRIMARY KEY (albumId,songId,trackNumber), 
    CONSTRAINT FK_albumEntries_albums FOREIGN KEY (albumId) REFERENCES albums (id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT FK_albumEntries_songs FOREIGN KEY (songId) REFERENCES songs (id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE countries
(
    countryId INT,
    name TEXT NOT NULL,
    flag TEXT NULL,

    CONSTRAINT PK_countries PRIMARY KEY (countryId)
);

CREATE TABLE radio_styles
(
    styleId INT,
    name TEXT NOT NULL,

    CONSTRAINT PK_radio_styles PRIMARY KEY (styleId)
);

CREATE TABLE radio
(
    radioId INT,
    name TEXT NOT NULL,
    playlist TEXT NOT NULL,
    bitrate INT NOT NULL,
    countryId INT NULL,
    location TEXT NULL,
    styleId INT NULL,
    url TEXT NOT NULL,

    CONSTRAINT PK_radio PRIMARY KEY (radioId),
    CONSTRAINT FK_radio_countries FOREIGN KEY (countryId) REFERENCES countries (countryId),
    CONSTRAINT FK_radio_radio_styles FOREIGN KEY (styleId) REFERENCES radio_styles (styleId)
);

CREATE        INDEX IX_albums_name ON albums (name);

CREATE        INDEX IX_artists_name ON artists (name);
CREATE        INDEX IX_artists_score ON artists (score);
CREATE        INDEX IX_artists_downloads ON artists (downloads);
CREATE        INDEX IX_artists_rating ON artists (rating);

CREATE UNIQUE INDEX IX_songs_fileName ON songs (fileName);
CREATE        INDEX IX_songs_title ON songs (title);
CREATE        INDEX IX_songs_format ON songs (format);
CREATE        INDEX IX_songs_genre ON songs (genre);
CREATE        INDEX IX_songs_artist ON songs (artist);
CREATE        INDEX IX_songs_score ON songs (score);
CREATE        INDEX IX_songs_favourited ON songs (favourited);
CREATE        INDEX IX_songs_downloads ON songs (downloads);
CREATE        INDEX IX_songs_playCount ON songs (playCount);
CREATE        INDEX IX_songs_lastPlayed ON songs (lastPlayed);
CREATE        INDEX IX_songs_myFavourite ON songs (myFavourite);

CREATE UNIQUE INDEX IX_countries        ON countries (name);
CREATE UNIQUE INDEX IX_radio_styles     ON radio_styles (name);
CREATE        INDEX IX_radio_name       ON radio (name);
CREATE        INDEX IX_radio_location   ON radio (location);

BEGIN TRANSACTION;

INSERT INTO genres (id, name) VALUES (48, 'Alternative');
INSERT INTO genres (id, name) VALUES (38, 'Gothic');
INSERT INTO genres (id, name) VALUES (103, 'Grunge');
INSERT INTO genres (id, name) VALUES (36, 'Metal (general)');
INSERT INTO genres (id, name) VALUES (37, 'Metal - Extreme');
INSERT INTO genres (id, name) VALUES (35, 'Punk');
INSERT INTO genres (id, name) VALUES (54, 'Chiptune');
INSERT INTO genres (id, name) VALUES (55, 'Demo Style');
INSERT INTO genres (id, name) VALUES (53, 'One Hour Compo');
INSERT INTO genres (id, name) VALUES (1, 'Electronic (general)');
INSERT INTO genres (id, name) VALUES (2, 'Electronic - Ambient');
INSERT INTO genres (id, name) VALUES (9, 'Electronic - Breakbeat');
INSERT INTO genres (id, name) VALUES (106, 'Electronic - Chillout');
INSERT INTO genres (id, name) VALUES (3, 'Electronic - Dance');
INSERT INTO genres (id, name) VALUES (6, 'Electronic - Drum & Bass');
INSERT INTO genres (id, name) VALUES (40, 'Electronic - Gabber');
INSERT INTO genres (id, name) VALUES (39, 'Electronic - Hardcore');
INSERT INTO genres (id, name) VALUES (10, 'Electronic - House');
INSERT INTO genres (id, name) VALUES (99, 'Electronic - IDM');
INSERT INTO genres (id, name) VALUES (34, 'Electronic - Industrial');
INSERT INTO genres (id, name) VALUES (60, 'Electronic - Jungle');
INSERT INTO genres (id, name) VALUES (101, 'Electronic - Minimal');
INSERT INTO genres (id, name) VALUES (100, 'Electronic - Other');
INSERT INTO genres (id, name) VALUES (11, 'Electronic - Progressive');
INSERT INTO genres (id, name) VALUES (65, 'Electronic - Rave');
INSERT INTO genres (id, name) VALUES (7, 'Electronic - Techno');
INSERT INTO genres (id, name) VALUES (71, 'Trance (general)');
INSERT INTO genres (id, name) VALUES (63, 'Trance - Acid');
INSERT INTO genres (id, name) VALUES (67, 'Trance - Dream');
INSERT INTO genres (id, name) VALUES (66, 'Trance - Goa');
INSERT INTO genres (id, name) VALUES (64, 'Trance - Hard');
INSERT INTO genres (id, name) VALUES (85, 'Trance - Progressive');
INSERT INTO genres (id, name) VALUES (70, 'Trance - Tribal');
INSERT INTO genres (id, name) VALUES (74, 'Big Band');
INSERT INTO genres (id, name) VALUES (19, 'Blues');
INSERT INTO genres (id, name) VALUES (29, 'Jazz (general)');
INSERT INTO genres (id, name) VALUES (30, 'Jazz - Acid');
INSERT INTO genres (id, name) VALUES (31, 'Jazz - Modern');
INSERT INTO genres (id, name) VALUES (75, 'Swing');
INSERT INTO genres (id, name) VALUES (105, 'Bluegrass');
INSERT INTO genres (id, name) VALUES (20, 'Classical');
INSERT INTO genres (id, name) VALUES (45, 'Comedy');
INSERT INTO genres (id, name) VALUES (18, 'Country');
INSERT INTO genres (id, name) VALUES (46, 'Experimental');
INSERT INTO genres (id, name) VALUES (52, 'Fantasy');
INSERT INTO genres (id, name) VALUES (21, 'Folk');
INSERT INTO genres (id, name) VALUES (102, 'Fusion');
INSERT INTO genres (id, name) VALUES (28, 'Medieval');
INSERT INTO genres (id, name) VALUES (44, 'New Age');
INSERT INTO genres (id, name) VALUES (50, 'Orchestral');
INSERT INTO genres (id, name) VALUES (41, 'Other');
INSERT INTO genres (id, name) VALUES (59, 'Piano');
INSERT INTO genres (id, name) VALUES (49, 'Religious');
INSERT INTO genres (id, name) VALUES (43, 'Soundtrack');
INSERT INTO genres (id, name) VALUES (47, 'Spiritual');
INSERT INTO genres (id, name) VALUES (8, 'Video Game');
INSERT INTO genres (id, name) VALUES (76, 'Vocal Montage');
INSERT INTO genres (id, name) VALUES (42, 'World');
INSERT INTO genres (id, name) VALUES (56, 'Ballad');
INSERT INTO genres (id, name) VALUES (58, 'Disco');
INSERT INTO genres (id, name) VALUES (107, 'Easy Listening');
INSERT INTO genres (id, name) VALUES (32, 'Funk');
INSERT INTO genres (id, name) VALUES (12, 'Pop (general)');
INSERT INTO genres (id, name) VALUES (62, 'Pop - Soft');
INSERT INTO genres (id, name) VALUES (61, 'Pop - Synth');
INSERT INTO genres (id, name) VALUES (13, 'Rock (general)');
INSERT INTO genres (id, name) VALUES (14, 'Rock - Hard');
INSERT INTO genres (id, name) VALUES (15, 'Rock - Soft');
INSERT INTO genres (id, name) VALUES (72, 'Christmas');
INSERT INTO genres (id, name) VALUES (82, 'Halloween');
INSERT INTO genres (id, name) VALUES (22, 'Hip-Hop');
INSERT INTO genres (id, name) VALUES (26, 'R & B');
INSERT INTO genres (id, name) VALUES (27, 'Reggae');
INSERT INTO genres (id, name) VALUES (24, 'Ska');
INSERT INTO genres (id, name) VALUES (25, 'Soul');
INSERT INTO genres (id, name) VALUES (0, '- N/A -');

COMMIT TRANSACTION;

BEGIN TRANSACTION;

INSERT INTO formats (id, name, description) VALUES (1, 'MOD', 'Amiga Module');
INSERT INTO formats (id, name, description) VALUES (2, '669', '669 Mod Composer Module');
INSERT INTO formats (id, name, description) VALUES (3, 'IT', 'Impulse Tracker Module');
INSERT INTO formats (id, name, description) VALUES (4, 'MED','OctaMED Module');
INSERT INTO formats (id, name, description) VALUES (5, 'MTM','MultiTracker Module');
INSERT INTO formats (id, name, description) VALUES (6, 'OCT','OctaMED Module');
INSERT INTO formats (id, name, description) VALUES (7, 'OKT','Oktalyzer Module');
INSERT INTO formats (id, name, description) VALUES (8, 'S3M','ScreamTracker 3 Module');
INSERT INTO formats (id, name, description) VALUES (9, 'STM','ScreamTracker 2 Module');
INSERT INTO formats (id, name, description) VALUES (10, 'XM','FastTracker 2 Module');
INSERT INTO formats (id, name, description) VALUES (11, 'AHX','Amiga Chiptune Module');
INSERT INTO formats (id, name, description) VALUES (12, 'HVL','HivelyTracker Module');
INSERT INTO formats (id, name, description) VALUES (13, 'MO3','Ian Luck''s MP3/OGG Module');
INSERT INTO formats (id, name, description) VALUES (100, 'MP3','MP3 Song');
INSERT INTO formats (id, name, description) VALUES (101, 'OGG','Xiph.org Foundation OGG Song');
INSERT INTO formats (id, name, description) VALUES (102, 'FLAC','Free Lossless Audio Codec Song');
INSERT INTO formats (id, name, description) VALUES (103, 'WAV','Waveform Audio Format Song');
INSERT INTO formats (id, name, description) VALUES (104, 'ASF','Advanced Systems Format Song');
INSERT INTO formats (id, name, description) VALUES (105, 'WMA','Windows Media Audio Format Song');
INSERT INTO formats (id, name, description) VALUES (106, 'AAC','Advanced Audio Coding Format Song');
INSERT INTO formats (id, name, description) VALUES (107, 'MP4','MP4 Song');
INSERT INTO formats (id, name, description) VALUES (108, 'M4A','M4A Song');
INSERT INTO formats (id, name, description) VALUES (109, 'MKA','Matroska Audio Format Song');

COMMIT TRANSACTION;

BEGIN TRANSACTION;

INSERT INTO trackers (id, name) VALUES (0, '- N/A -');
INSERT INTO "artists" VALUES(0,'- N/A -',NULL,NULL,NULL);

COMMIT TRANSACTION;