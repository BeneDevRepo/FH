


-- 1
SELECT
	B.Titel AS Buchtitel,
	E.AnschDat AS angeschafft_am,
	COUNT(E) AS Anzahl
FROM Buch B
INNER JOIN Exemplar E
ON B.BuchOID = E.BuchOID
GROUP BY E.AnschDat;


-- 2
SELECT L.Nachname
FROM Leser L
INNER JOIN Ausleihe LA
INNER JOIN Ausleihe SA
INNER JOIN Leser S
ON L.LeserOID = LA.LeserOID
AND S.LeserOID = SA.LeserOID
AND LA.BuchOID = SA.BuchOID
AND NOT L.LeserOID = S.LeserOID
GROUP BY L.LeserOID;


-- 3
SELECT
	B.Titel AS Titel,
	M.Datum AS MahnDatum
FROM Mahnung M
INNER JOIN Buch B
ON M.BuchOID = B.BuchOID;


-- 4
SELECT
	L.Nachnahme AS Nachname,
	IIF(SUM(M.Betrag) > 0, SUM(M.Betrag), "kein Betrag offen") AS Betrag
FROM Leser L
INNER JOIN Mahnung M
ON L.LeserOID = M.LeserOID
WHERE M.Betrag IS NOT NULL
GROUP BY L.LeserOID;


-- 5
SELECT
	MAX(A.RDat) AS RDatum
FROM Ausleihe A
INNER JOIN Buch B
	ON B.BuchOID = A.BuchOID
INNER JOIN Leser L
	ON L.LeserOID = A.LeserOID
WHERE B.Titel = "Grundlagen von DB Systemen"
	AND B.Verfasser = "Elmasri"
	AND L.Nachname = "Schmitz"
GROUP BY A.ExID
ORDER BY A.RDat DESC;


-- 6
SELECT B.Titel AS Titel
FROM Buch B
INNER JOIN Mahnung M
	ON M.BuchOID = B.BuchOID
GROUP BY B.BuchOID
ORDER BY COUNT(M) DESC;


-- 7
SELECT
	L.Nachname AS Nachname,
	IIF(COUNT(V) > 0, V.VormDat, "Keine Vormerkung") AS Vormerkung
FROM Leser L
LEFT JOIN Vormerkung V
	ON V.LeserOID = L.LeserOID
GROUP BY Leser.LeserOID;











