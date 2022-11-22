

-- 1:
SELECT
	(
		SELECT COUNT(M.MOID)
		FROM Mahnung M
		WHERE M.Datum = DATEADD(DAY, 1, A.RDat)
	) * 100 / COUNT(A.LNr) AS Prozent
FROM Ausleihe A;



-- 2:
SELECT
	(
		SELECT B.Titel
		FROM Buch B
		WHERE B.BuchOID = E.BuchOID
	) AS Titel,
	E.ExID AS Exemplar
FROM Exemplar E
WHERE
	(
		SELECT SUM(M.Betrag)
		FROM Mahnung M
		WHERE M.ExID = E.ExID
		GROUP BY M.ExID
	) > E.AnschBetrag;


-- 3:
SELECT 
	Titel,
	Exemplar
FROM (
		SELECT
			E.ExID AS Exemplar,
			E.AnschBetrag AS Preis
			(SELECT SUM(M.Betrag) FROM Mahnung M WHERE M.ExID = E.ExID GROUP BY M.ExID) AS Strafe,
			(SELECT B.Titel FROM Buch B WHERE B.BuchOID = E.BuchOID) AS Titel
		FROM Exemplar E
	)
WHERE Strafe > AnschBetrag;


-- 4:
SELECT Name
FROM (
	SELECT
		L.Nachname AS Name
		WHERE (
			SELECT COUNT(V.BuchOID)
			FROM Vormerkung V
			WHERE
				V.LeserOID = L.LeserOID
				AND EXTRACT(YEAR FROM V.VormDat) = 2011
				AND V.BuchOID IN (
					SELECT M.BuchOID
					FROM Mahnung M
					WHERE
						M.LeserOID = L.LeserOID
						AND EXTRACT(YEAR FROM M.Datum) > 2011
				)
			GROUP BY V.BuchOID
		) > 0
	)
);


-- 5:
SELECT
	(SELECT B.Titel FROM Buch B WHERE B.BuchOID = E.BuchOID) AS Buchtitel,
	E.ExID AS ExID,
	(SELECT IIF(COUNT(A.LNr)>0, "ausgeliehen", "nicht ausgeliehen") FROM Ausleihe A WHERE A.ADat <= CURRENT_DATE AND A.RDAT >= CURRENT_DATE) AS Status
FROM Exemplar E;


-- 6:
SELECT
	B.Titel,
	E.ExID AS ExID,
	(SELECT IIF(COUNT(A.LNr)>0, "ausgeliehen", "nicht ausgeliehen") FROM Ausleihe A WHERE A.ADat <= CURRENT_DATE AND A.RDAT >= CURRENT_DATE) AS Status
FROM Exemplar E
INNER JOIN Buch B
ON B.BuchOID = E.BuchOID;















