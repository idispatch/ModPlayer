PRAGMA foreign_keys=1;
PRAGMA page_size=8192;
PRAGMA user_version=3;

CREATE TABLE country
(
	countryId INT,
	name TEXT NOT NULL,

	CONSTRAINT PK_countries PRIMARY KEY (countryId)
);

CREATE TABLE radio_style
(
	styleId INT,
	name TEXT NOT NULL,

	CONSTRAINT PK_radio_style PRIMARY KEY (styleId)
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
	CONSTRAINT FK_radio_country FOREIGN KEY (countryId) REFERENCES country (countryId),
	CONSTRAINT FK_radio_radio_style FOREIGN KEY (styleId) REFERENCES radio_style (styleId)
);

CREATE UNIQUE INDEX IX_country        ON country (name);
CREATE UNIQUE INDEX IX_radio_style    ON radio_style (name);
CREATE        INDEX IX_radio_name     ON radio (name);
CREATE        INDEX IX_radio_location ON radio (location);
