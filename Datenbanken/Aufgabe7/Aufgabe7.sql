

-- 1:
SELECT
	B.Titel AS Titel,
	B.Verfasser AS Verfasser
FROM Buch B
WHERE B.BuchOID NOT IN (
	SELECT BuchOID
	FROM Ausleihe
	GROUP BY BuchOID
);

-- 2:
SELECT
	B.Titel AS Titel,
	E.ExID AS Exemplar
FROM Buch B, Exemplar E
WHERE B.BuchOID = E.BuchOID
AND E.ExID IN (
	SELECT ExID
	FROM Ausleihe
	WHERE EXTRACT(DAY FROM RDat - CURRENT_DATE) > 14
);

-- 3:
SELECT B.Titel
FROM Buch B
WHERE B.BuchOID IN (
	SELECT BuchOID
	FROM Exemplar
	WHERE AnschDat = MAX(AnschDat)
);

-- 4:
SELECT B.Titel
FROM Buch B
WHERE B.BuchOID = ANY (
	SELECT BuchOID
	FROM Mahnung
	WHERE AnschDat > '2012-10-15'
);

-- 5:
SELECT B.Titel
FROM Buch B
WHERE B.BuchOID IN (
	SELECT E.BuchOID
	FROM Exemplar E, Vormerkung V
	WHERE E.BuchOID = V.BuchOID
		AND MIN(V.VormDat) < MIN(E.AnschDat)
	GROUP BY E.BuchOID
);

-- 6:
SELECT
	L.Nachname AS Name,
	COUNT(
		SELECT *
		FROM Ausleihe A
		WHERE A.LeserOID = L.LeserOID
	) AS AnzahlAusleihen
FROM Leser L;

-- 7:
SELECT
	B.Titel AS Titel,
	E.ExID AS Exemplar
FROM Buch B, Exemplar E
WHERE B.BuchOID = E.BuchOID
AND E.ExID NOT IN (
	SELECT ExID
	FROM Ausleihe
);


