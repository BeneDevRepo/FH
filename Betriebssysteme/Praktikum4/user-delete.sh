#!/bin/bash
#===============================================================================
#
# FILE: user-delete.sh
#
# USAGE: ./user-delete.sh user-file
#
# DESCRIPTION: Nutzer mittels datei loeschen
#
# OPTIONS: ---
# REQUIREMENTS: ---
# BUGS: ---
# NOTES: ---
# AUTHOR: Benedikt Binger
# ORGANIZATION: Fachhochschule Südwestfalen, Iserlohn
# CREATED: ---
# REVISION: ---
#===============================================================================



# Sicherstellen dass das script mit root-privilegien ausgeführt wird:
if [ "$EUID" -ne 0 ]
then
	echo "Neustart als root..."
	sudo $0 $@
	exit $?
fi


# Anzahl der Parameter pruefen:
if [ "$#" -ne 1 ]; then
	echo "Usage: ./user-delete.sh FILE"
fi

# Inputdatei speichern:
users="$1"


# Testen ob inputdatei existiert und lesbar ist:
if [ ! -r "$users" ]
then
	echo "Datei kann nicht gelesen werden!"
	exit 1
fi


### Hauptschleife:

#cut -d";" -f"1,2,3,4,5" "$users" | {
cat "$users" | tr ';' ' ' | {
#cat "$users" | {
	while read name firstname number loginname password 
	do
		### Start eines Durchgangs fuer einen einzelnen Nutzer:
		
		
		# Nutzer ausdrucken:
		# echo "$name $number"
		
		# Nutzer entfernen:
		userdel -r "$loginname"
		
	done
	#< "$users"
}