-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2025-03-24 13:07:48.414

-- tables
-- Table: CompactDisk
CREATE TABLE CompactDisk (
    id int  NOT NULL,
    date date  NOT NULL,
    company char(256)  NOT NULL,
    price int  NOT NULL,
    CONSTRAINT CompactDisk_pk PRIMARY KEY (id)
);

-- Table: MusicalComposition
CREATE TABLE MusicalComposition (
    name char(256)  NOT NULL,
    author char(256)  NOT NULL,
    performer char(256)  NOT NULL,
    compactID int  NOT NULL,
    CONSTRAINT MusicalComposition_pk PRIMARY KEY (compactID)
);

-- Table: Trade
CREATE TABLE Trade (
    date date  NOT NULL,
    compactID int  NOT NULL,
    amount int  NOT NULL,
    code int  NOT NULL
);

-- Table: TradeCodeInfo
CREATE TABLE TradeCodeInfo (
    id int  NOT NULL,
    type char(10)  NOT NULL,
    CONSTRAINT TradeCodeInfo_pk PRIMARY KEY (id)
);

-- foreign keys
-- Reference: MusicalComposition_CompactDisk (table: MusicalComposition)
ALTER TABLE MusicalComposition ADD FOREIGN KEY (compactID)
    REFERENCES CompactDisk (id);

-- Reference: Trade_CompactDisk (table: Trade)
ALTER TABLE Trade ADD FOREIGN KEY (compactID)
    REFERENCES CompactDisk (id);

-- Reference: Trade_TradeCodeInf (table: Trade)
ALTER TABLE Trade ADD FOREIGN KEY (code)
    REFERENCES TradeCodeInfo (id);

-- End of file.

