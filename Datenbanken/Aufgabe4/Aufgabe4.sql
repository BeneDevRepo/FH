
-- 1
SELECT BuchOID, Titel, ExID, AnschDat FROM Buch, Exemplar
WHERE Buch.BuchOID = Exemplar.BuchOID;

SELECT b.BuchOID, b.Titel, e.ExID, e.AnschDat
FROM Buch b NATURAL JOIN Exemplar e;


-- 2
SELECT *
FROM Leser, Vormerkung
WHERE Leser.LeserOID = Vormerkung.LeserOID;

SELECT *
FROM Leser l NATURAL JOIN Vormerkung v;


-- 3
SELECT BuchOID, Titel, Verfasser
FROM Buch, Mahnung, Vormerkung
WHERE Mahnung.BuchOID = Vormerkung.BuchOID
	AND Buch.BuchOID = Mahnung.BuchOID;

SELECT BuchOID, Titel, Verfasser
FROM Buch b NATURAL JOIN Mahnung NATURAL JOIN Vormerkung;


-- 4
SELECT Buch.Titel, SUM(Mahnung)
FROM Buch NATURAL JOIN Mahnung
WHERE Buch.Titel = "Java ist eine Insel";

-- 5
SELECT Titel, Nachname
FROM Leser NATURAL JOIN Mahnung NATURAL JOIN Buch
WHERE Buch.Titel = "Grundlagen von Datenbanksystemen";

-- 6
SELECT *
FROM Exemplar, Mahnung
Where Exemplar.AnschDat > Mahnung.Datum
	And Exemplar.BuchOID = Mahnung.BuchOID
	And Exemplar.ExID = Mahnung.ExID;

SELECT *
FROM Exemplar NATURAL JOIN Mahnung
WHERE Exemplar.AnschDat > Mahnung.Datum;
	
	
	
