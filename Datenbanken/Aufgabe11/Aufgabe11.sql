-- 1:
CREATE TABLE Geburtsdatum (
	GebDatOID integer,
	Tag integer,
	Monat integer,
	Jahr integer,
	GebDat Date
);

UPDATE Geburtsdatum 
SET GebDat = CAST ((Tag||'.'||Monat||'.'||Jahr) AS Date);


CREATE TRIGGER t_GebOID FOR Geburtsdatum
	BEFORE INSERT
AS
BEGIN



-- 2:
CREATE PROCEDURE Aufgabe2
AS
DECLARE VARIABLE zaehler integer;
DECLARE VARIABLE t integer;

begin
  zaehler = 10000;
  FOR select NachnameOID from Nachname into t
  do
    begin
        update nachname
        set NachnameOID = :zaehler
        where NachnameOID = :t;
        zaehler = zaehler + 1;
  end
end


-- 3:
-- STP die alle OIDs aus Vorname in Keywerte schreibt

create or alter procedure KeywerteVergabe
as
declare variable nummer integer;

begin
    for select VornameOID from vorname into nummer
    do
    begin
        insert into keywerte values (:nummer);
    end
end


-- Trigger zum vorname einfuegen

create trigger einfuegen for vorname
active before insert position 0
as
begin
    execute procedure EinfuegenKeywert (new.VornameOID);
end


-- Trigger zum vornamen loeschen

create trigger loeschen for vorname
active before delete position 0
as
begin
    execute procedure LoeschenKeywert (old.VornameOID);
end


-- STP zum nachnamen erstellen

create procedure EinfuegenKeywert (wert integer)
as
begin
    insert into Keywerte values (:wert);
end

-- STP zum nachnamen loeschen

create procedure LoeschenKeywert (wert integer)
as
begin
    delete from Keywerte
    where Werte = :wert;
end






-- 4

create procedure Testdaten
as

declare variable vname varchar(30);
declare variable nname varchar(30);
declare variable ort varchar(50);
declare variable strassse varchar(50);
declare variable gebdat date;
declare variable zufall integer;
declare variable zaehler integer;

begin
counter=0;
while (counter < 10000)
do
begin
    zufall = (cast(rand() *199 as integer)+1);
    select vorname1 from vorname where VornameOID = :zufall into :vname;

    zufall = 10000+(cast(rand() *200 as integer));
    select nachname1 from nachname where NachnameOID = :zufall into :nname;

    zufall = (cast(rand() *200 as integer)*2);
    select ort1 from Wohnort where OrtOID = :zufall into :ort;

    zufall = (cast(rand() *200 as integer)+1);
    select strasse1 from Strasse where StrasseOID = :zufall into :strassse;

    zufall = (cast(rand() *100 as integer)+1);
    select GebDat from Geburtsdatum where GebDatOID = :zufall into :gebdat;

    insert into Person (vorname,Nachname,Orte,Strassen,Geburtsdaten) values
    (:vname, :nname, :ort, :strassse, :gebdat);

    counter = counter + 1;
    end
end





