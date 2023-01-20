-- Aufgabe 1:

CREATE USER NN PASSWORD '123';

CREATE OR ALTER view V_NN
AS SELECT Nachname FROM Leser;

GRANT SELECT ON v_nn TO NN;



-- Aufgabe 2:

CREATE OR ALTER view V2_NN
AS SELECT * FROM Leser
WHERE LeserOID IN (SELECT LeserOID FROM Vormerkung);

GRANT SELECT ON V2_NN TO NN;



-- Aufgabe 3:

GRANT SELECT ON V_NN TO public;



-- Aufgabe 4:

CREATE ROLE LeserBetreuer;

GRANT SELECT, delete ON Leser TO LeserBetreuer;

GRANT UPDATE (Mail) ON Leser TO LeserBetreuer;

GRANT SELECT, UPDATE ON Mahnung TO LeserBetreuer;



-- Aufgabe 5:

CREATE USER ZZ PASSWORD '1234';

GRANT LeserBetreuer TO ZZ;



-- Aufgabe 6:

REVOKE SELECT ON V_NN FROM NN;



-- Aufgabe 7:

CREATE OR ALTER trigger EinfuegenBuch for Buch
active BEFORE INSERT position 0
AS
BEGIN
  EXECUTE PROCEDURE EinfuegenExemplar(new.BuchOID);
END 


CREATE OR ALTER PROCEDURE EinfuegenExemplar(nummer integer)
AS
BEGIN
    INSERT INTO Exemplar(BuchOID, EXID) VALUES(:nummer, 1);
END







