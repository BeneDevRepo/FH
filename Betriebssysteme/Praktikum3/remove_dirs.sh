#!/bin/bash -
#============================================================================================
#
#
#				FILE				: remove_dirs.sh
#				
#				USAGE				: ./remove_dirs.sh
#
#				DESCRIPTION	: Datei (der Name ist der erste Aufrufparameter) in allen nicht
#                     versteckten Unterverzeichnissen loescht.
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
if [ ${#} -lt 0 ]
then
	echo -e "\n\tAufruf: $0 Anzahl_der_Verzeichnisse\n"
	exit 1
fi

#--------------------------------------------------------------------------------------------
#  Unterverzeichnisse ermitteln
#--------------------------------------------------------------------------------------------
liste=$(find -type d -name "[^.]*")

#--------------------------------------------------------------------------------------------
#  Datei in Unterverzeichniss loeschen
#--------------------------------------------------------------------------------------------
anzahl=0
erfolg=0

for l in $liste
do
	rm -rf $l && ((erfolg++))
	((anzahl++))
done

#--------------------------------------------------------------------------------------------
#  Kontrollausgabe
#--------------------------------------------------------------------------------------------
echo -e "\n${erfolg}/${anzahl} Verzeichnis geloescht\n"