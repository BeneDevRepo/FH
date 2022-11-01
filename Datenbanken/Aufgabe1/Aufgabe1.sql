-- Aufgabe 1.3:

Der Unterschied zwischen Tabellenmodell und Implementierungsmodell ist, dass das Implementierungsmodell eine 1:1 Abbildung einer implementierbaren Datenbank ist, während das Tabellenmodell lediglich eine Darstellung der allgemeinen Tabellenstruktur ist.

-- Aufgabe 1.4:

CREATE TABLE Kunde(
	Nachname varchar(20),
	Erstbestellung date,
	KontoverbindungOID1 int NOT NULL,
	KontoverbindungOID2 int,
	KundeOID int NOT NULL
);

CREATE TABLE Kontoverbindung(
	Bankleitzahl varchar(20),
	Kontonummer varchar(20),
	KontoverbindungOID int NOT NULL
);

CREATE TABLE Bestellung(
	Menge int,
	Preis int,
	BestellungOID int NOT NULL
);

CREATE TABLE Artikel(
	Bezeichnung varchar(20),
	Nettoeinzelpreis int,
	ArtikelOID int NOT NULL
);

ALTER TABLE Kontoverbindung ADD CONSTRAINT PK_Kontoverbindung PRIMARY KEY(KontoverbindungOID);
ALTER TABLE Bestellung ADD CONSTRAINT PK_Bestellung PRIMARY KEY(BestellungOID);

ALTER TABLE Kunde ADD CONSTRAINT FK_KundeKontoverbindung1 FOREIGN KEY(KontoverbindungOID1) REFERENCES Kontoverbindung(KontoverbindungOID) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE Kunde ADD CONSTRAINT FK_KundeKontoverbindung2 FOREIGN KEY(KontoverbindungOID2) REFERENCES Kontoverbindung(KontoverbindungOID) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE Kunde ADD CONSTRAINT FK_KundeBestellung FOREIGN KEY(KundeOID) REFERENCES Bestellung(BestellungOID) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE Artikel ADD CONSTRAINT FK_ArtikelBestellung FOREIGN KEY(ArtikelOID) REFERENCES Bestellung(BestellungOID) ON DELETE NO ACTION ON UPDATE NO ACTION;


-- Aufgabe 1.7:

SELECT
	nachname,
	COUNT(KontoverbindungOID1) + COUNT(KontoverbindungOID2) AS Anzahl
FROM kunde
GROUP BY KundeOID;


-- Aufgabe 1.8:

SELECT
	kunde.nachname,
	kontoverbindung.bankleitzahl,
	kontoverbindung.kontonummer
FROM kunde
INNER JOIN
	kontoverbindung
ON (
	kunde.kundeoid = kontoverbindung.kontoverbindungoid
	OR kunde.kontoverbindungoid2 = kontoverbindung.kontoverbindungoid
)
WHERE kontoverbindung.kontoverbindungoid IS NOT NULL
ORDER BY nachname;

-- Aufgabe 1.9:

Das implementierungsmodell lässt das Anlegen von mehr als 2 oder weniger als 1 Konten nicht zu.

-- Aufgabe 2.1:

CREATE TABLE Fahrzeug(
	Kennzeichen varchar(20),
	FahrzeugOID int NOT NULL
);

CREATE TABLE Anhaenger(
	Art varchar(10),
	Laenge int,
	AnhaengerOID int NOT NULL
);

CREATE TABLE Krad(
	FuehKlasse varchar(10),
	Marke varchar(20),
	KradOID int NOT NULL
);

CREATE TABLE LKW(
	zulGesGew int,
	LKWOID int NOT NULL
);

CREATE TABLE PKW(
	Marke Marke,
	AnzPlaetze int,
	PKWOID int NOT NULL
);

ALTER TABLE Fahrzeug ADD CONSTRAINT PK_Fahrzeug PRIMARY KEY(FahrzeugOID);
ALTER TABLE PKW ADD CONSTRAINT PK_PKW PRIMARY KEY(PKWOID);

ALTER TABLE Anhaenger ADD CONSTRAINT FK_AnhaengerPKW FOREIGN KEY(AnhaengerOID) REFERENCES PKW(PKWOID) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE Krad ADD CONSTRAINT FK_KradFahrzeug FOREIGN KEY(KradOID) REFERENCES Fahrzeug(FahrzeugOID) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE LKW ADD CONSTRAINT FK_LKWFahrzeug FOREIGN KEY(LKWOID) REFERENCES Fahrzeug(FahrzeugOID) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE PKW ADD CONSTRAINT FK_PKWFahrzeug FOREIGN KEY(PKWOID) REFERENCES Fahrzeug(FahrzeugOID) ON DELETE NO ACTION ON UPDATE NO ACTION;


-- Aufgabe 2.3:

SELECT krad.marke, fahrzeug.kennzeichen
	FROM fahrzeug, krad
	WHERE fahrzeug.fahrzeugoid = (SELECT krad.kradoid FROM krad WHERE krad.kradoid = fahrzeug.fahrzeugoid AND Marke = 'BMW');




-- Aufgabe 2.4:

SELECT fahrzeug.kennzeichen
	FROM fahrzeug
	WHERE fahrzeug.fahrzeugoid = (SELECT Anhaenger.anhaengeroid FROM anhaenger WHERE anhaenger.anhaengeroid = fahrzeug.fahrzeugoid AND Art = 'Bootsanhaenger');










