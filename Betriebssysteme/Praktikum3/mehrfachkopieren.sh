#!/bin/bash -
#============================================================================================
#
#
#				FILE				: mehrfachkopieren.sh
#				
#				USAGE				: ./mehrfachkopieren.sh file_name
#
#				DESCRIPTION	: Datei (Kommandozielenparameter) in alle vorhandenen
#                     Unterverzeichnisse kopieren
#
#				OPTIONS			: ---
#				REQUIREMENTS: ---
#				BUGS				: ---
#				NOTES				: ---
#				AUTHOR			: 
#				ORGANIZATION: Fachhochschule Suedwestfalen, Iserlohn
#				CREATED			: 15.11.2022 21:30
#				REVISION		: ---	
#============================================================================================

#--------------------------------------------------------------------------------------------
#  Aufruf / Anzahl der Aufrufparameter ueberpruefen
#--------------------------------------------------------------------------------------------
if [ ${#} -lt 1 ]
then
	echo -e "\n\tAufruf: $0 Anzahl_der_Verzeichnisse\n"
	exit 1
fi

if [ ! -f $1 ]
then
	echo -e "\n\tParameter ist keine Datei\n"
	exit 1
fi

#--------------------------------------------------------------------------------------------
#  Pruefen, ob die zu kopierende Datei existiert
#--------------------------------------------------------------------------------------------
if [ ! -f "$1" ]
then
	echo -e "\n\tAufruf: Datei $1 nicht gefunden\n"
	exit 1
fi

#--------------------------------------------------------------------------------------------
#  Unterverzeichnisse ermitteln
#--------------------------------------------------------------------------------------------
liste=$(find -type d -name "[^.]*")

#--------------------------------------------------------------------------------------------
#  Datei kopieren und zaehlen
#--------------------------------------------------------------------------------------------
anzahl=0
erfolg=0

for l in $liste
do
	cp $1 -t $l && ((erfolg++))
	((anzahl++))
done

#--------------------------------------------------------------------------------------------
#  Kontrollausgabe
#--------------------------------------------------------------------------------------------
echo -e "\n${erfolg}/${anzahl} Datei kopiert\n"