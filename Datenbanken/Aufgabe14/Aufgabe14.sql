-- Aufgabe 1:

CREATE USER NN PASSWORD '123';

CREATE OR ALTER view V_NN
AS SELECT Nachname FROM Leser;

GRANT SELECT ON v_nn TO NN;



-- Aufgabe 2:

CREATE OR ALTER view V_NN2
AS SELECT * FROM Leser
WHERE LeserOID IN (SELECT LeserOID FROM Vormerkung);

GRANT SELECT ON V_NN2 TO NN;



-- Aufgabe 3:

GRANT SELECT ON V_NN TO public;



-- Aufgabe 4:

CREATE ROLE Leserbetreuer2;

GRANT SELECT, delete ON Leser TO Leserbetreuer2;

GRANT UPDATE (Mail) ON Leser TO Leserbetreuer2;

GRANT SELECT, UPDATE ON Mahnung TO Leserbetreuer2;



-- Aufgabe 5:

CREATE USER ZZ PASSWORD '12345';

GRANT Leserbetreuer2 TO ZZ;



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







