#!/bin/bash
#===============================================================================
#
# FILE: user-add.sh
#
# USAGE: ./uesr-add.sh
#
# DESCRIPTION: Nutzer aus create_dirs mit zufaelligen Passwoertern hinzufuegen
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


# Testen ob inputdatei existiert und lesbar ist:
if [ ! -r user+num.txt ]
then
	echo "Inputdatei fehlt!"
	exit 1
fi


# Name der Protokolldatei:
outfile="user-added-$(date +%Y%m%d-%H%M%S).txt"

# Counter fuer Erfolge und Versuche:
versuche=0
erfolge=0

### Hauptschleife ueber egrep-output:
header="|%-10s|%-10s|%-10s|\n"
record="|%-10s|%-10s|%10d|\n"
printf $header "Vorname" "Nachname" "Nummer"
egrep "^([[:upper:]][[:lower:][:digit:]]+([ -][[:upper:]][[:lower:][:digit:]]+)?[[:blank:]]+){2}[[:digit:]]+$" user+num.txt | {
	while read name firstname number
	do
		### Start eines Durchgangs fuer einen einzelnen Nutzer:
		((versuche++))

		echo -n "$name, " >> "$outfile" # nachname speichern
		echo -n "$firstname, " >> "$outfile"
		echo -n "$number, " >> "$outfile"
		
		# Nutzername erstellen:
		loginname="${name,,}"
		echo -n "$loginname, " >> "$outfile" # loginname speichern
		
		
		# Nutzer ausdrucken:
		printf $record $firstname $name $number
		
		# Testen dass pwgen installiert ist:
		if ! command -v pwgen &> /dev/null; then
			echo "Pwgen ist nicht installiert."
			exit 2
		fi
		
		# Testen ob nutzer bereits existiert:
		if ! id bene > /dev/null; then
			echo "Nutzer existiert bereits"
			continue
		fi
		
		# testen ob user nicht mit home-verzeichnis angelegt werden kann:
		if [ -d "/home/$firstname" ]; then
			echo "user hat bereits ein home-verzeichnis"
			continue
		fi
		
		# Passwort erstellen:
		password="$(pwgen 8 1)"
		echo -n "$password" >> "$outfile" # passwort speichern
		if [ $? -ne 0 ]; then
			echo "Passwort konnte nicht generiert werden"
			exit 3
		fi



		# Nutzer hinzufuegen:
		useradd -c "$number" "$loginname"
		if [ $? -ne 0 ]; then
			echo "Nutzer konnte nicht hinzugefuegt werden"
			continue
		fi
		
		# Passwort setzen:
		echo ${loginname}:${password} | chpasswd 2>/dev/null
		if [ $? -ne 0 ]; then
			echo "Passwort konnte nicht gesetzt werden"
			continue
		fi

		echo "ERFOLG" >> "$outfile" # Eintrag als erfolgreich markieren
		

		((erfolge++))
	done

	printf "Erfolge/Versuche: %d/%d\n" $erfolge $versuche
}

# useradd: Anlegen des Nutzers mit home-verzeichnis
#	benötigt login

# passwd: Password des Nutzers setzen
#	benötigt Password und validen user

# Eintrag in protokolldatei