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


CREATE TRIGGER T_Geburtsdatum FOR Geburtsdatum
	BEFORE INSERT
AS
declare variable zaehler integer = 1;
begin
  new.gebdatoid = :zaehler;
  :zaehler = :zaehler+1;
end



-- 2:
CREATE PROCEDURE Aufgabe2
AS
DECLARE VARIABLE zaehler integer;
DECLARE VARIABLE t integer;

begin
  zaehler = 0;
  FOR select NachnameOID from Nachname into t
  do
    begin
        update nachname
        set NachnameOID = 10000 + :zaehler
        where NachnameOID = :t;
        zaehler = zaehler + 1;
        --zeahler = zeahler % 1000;
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






-- 4:

create procedure Testdaten
as

declare variable vname varchar(30);
declare variable nname varchar(30);
declare variable ort varchar(50);
declare variable street varchar(50);
declare variable gebdat date;
declare variable random integer;
declare variable counter integer;

begin
	counter=0;
	while (counter < 10000)
	do
	begin
		random = mod(cast(rand() *199 as integer)+1, (select count(*) from vorname));
		select vorname1 from vorname where VornameOID = :random into :vname;

		random = mod(cast(rand() *200 as integer), (select count(*) from nachname));
		select nachname1 from nachname where NachnameOID = :random into :nname;

		random = mod(cast(rand() *200 as integer)+1, (select count(*) from wohnort));
		select ort1 from Wohnort where OrtOID = :random into :ort;

		random = mod(cast(rand() *200 as integer)+1, (select count(*) from strasse));
		select strasse1 from Strasse where StrasseOID = :random into :street;

		random = mod(cast(rand() *100 as integer)+1, (select count(*) from geburtsdatum));
		select GebDat from Geburtsdatum where GebDatOID = :random into :gebdat;

		insert into Person (personoid, vorname,Nachname,Orte,Strassen,Geburtsdaten) values
		(:counter, :vname, :nname, :ort, :street, :gebdat);

		counter = counter + 1;
    end
end

/* create procedure Testdaten
as

declare variable vname varchar(30);
declare variable nname varchar(30);
declare variable ort varchar(50);
declare variable street varchar(50);
declare variable gebdat date;
declare variable random integer;
declare variable counter integer;

begin
	counter=0;
	while (counter < 10000)
	do
	begin
		random = (cast(rand() *199 as integer)+1);
		select vorname1 from vorname where VornameOID = :random into :vname;

		random = 10000+(cast(rand() *200 as integer));
		select nachname1 from nachname where NachnameOID = :random into :nname;

		random = (cast(rand() *200 as integer)*2);
		select ort1 from Wohnort where OrtOID = :random into :ort;

		random = (cast(rand() *200 as integer)+1);
		select strasse1 from Strasse where StrasseOID = :random into :street;

		random = (cast(rand() *100 as integer)+1);
		select GebDat from Geburtsdatum where GebDatOID = :random into :gebdat;

		insert into Person (vorname,Nachname,Orte,Strassen,Geburtsdaten) values
		(:vname, :nname, :ort, :street, :gebdat);

		counter = counter + 1;
    end
end */





