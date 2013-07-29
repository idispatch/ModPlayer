CREATE TABLE genres (id INTEGER PRIMARY KEY, name TEXT);

CREATE TABLE formats (id INTEGER PRIMARY KEY, name TEXT, description TEXT);

CREATE TABLE favourites (id INTEGER, date_added INT);

CREATE TABLE playstats (id INTEGER, played_times INT);

CREATE TABLE trackers
(id INTEGER PRIMARY KEY AUTOINCREMENT,
 name TEXT);

CREATE TABLE songs
(id INTEGER PRIMARY KEY AUTOINCREMENT,
 fileName TEXT,
 songName TEXT,
 format INT,
 length INT,
 tracker INT,
 genre INT,
 artist INT,
 size INT,
 mod_id INT,
 num_downloads INT,
 num_favourited INT,
 md5 TEXT,
 rating INT,
 info TEXT,
 created TEXT,
 patterns INT,
 orders INT,
 instruments INT,
 samples INT,
 channels INT,
 pattern_length INT);

CREATE UNIQUE INDEX IX_genres_name ON genres (name);
CREATE UNIQUE INDEX IX_songs_fileName ON songs (fileName);
CREATE INDEX IX_songs_md5 ON songs (md5);
CREATE UNIQUE INDEX IX_trackers_name ON trackers (name);
CREATE UNIQUE INDEX IX_formats_name ON formats (name);

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

INSERT INTO formats (id, name, description) VALUES (1, 'MOD', 'Amiga Module');
INSERT INTO formats (id, name, description) VALUES (2, '669', '669 Mod Composer Module');
INSERT INTO formats (id, name, description) VALUES (3, 'IT', 'Impulse Tracker Module');
INSERT INTO formats (id, name, description) VALUES (4, 'MED','OctaMED Module');
INSERT INTO formats (id, name, description) VALUES (5, 'MTM','MultiTracker Module');
INSERT INTO formats (id, name, description) VALUES (6, 'OCT','OctaMED Module');
INSERT INTO formats (id, name, description) VALUES (7, 'OKT','Oktalyzer Module');
INSERT INTO formats (id, name, description) VALUES (8, 'S3M','ScreamTracker 3 Module');
INSERT INTO formats (id, name, description) VALUES (9, 'STM','Scream Tracker 2 Module');
INSERT INTO formats (id, name, description) VALUES (10, 'XM','FastTracker 2 Module');
--INSERT INTO formats (id, name, description) VALUES (11, 'AHX','Amiga Chiptune Module');
--INSERT INTO formats (id, name, description) VALUES (12, 'HVL','HivelyTracker Module');
--INSERT INTO formats (id, name, description) VALUES (13, 'MO3','Ian Luck''s MP3/OGG Module');
