#!/bin/bash -
#===============================================================================
#
#          FILE: cd-umount.sh
#
#         USAGE: ./cd-umount.sh [sourceDir]
#
#   DESCRIPTION: 
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Benedikt Binger, 
#  ORGANIZATION: 
#       CREATED: 20.12.2022 19:45:37
#      REVISION:  ---
#===============================================================================


# Mount-verzeichnis setzen:
mountDir="$HOME/CDs"


# Quellverzeichnis "$HOME/CDs" als Standard verwenden:
sourceDir="$HOME/CDs"

# Quellverzeichnis aus Parameter uebernehmen, falls moeglich:
if [ "$#" -ne 0 ]; then
	sourceDir="$1"
fi


# Testen ob Mountverzeichnis ein Verzeichnis ist:
if [ ! -d "$mountDir" ]; then

	# Falls Mountverzeichnis nicht existiert: versuchen, es anzulegen:
	if ! mkdir -p "$mountDir"; then
		echo "Mountverzeichnis ($mountDir) Ist kein Verzeichnis!"
		exit 1
	fi
fi


# Testen ob Quellverzeichnis ein Verzeichnis ist:
if [ ! -d "$sourceDir" ]; then
	echo "Quellverzeichnis ($sourceDir) Ist kein Verzeichnis!"
	exit 2
fi

# Dateiendung ".image" verwenden, falls endung nicht explizit gesetzt wurde:
fileEnding=${fileEnding:="image"}


# Testen ob fusermount  installiert ist:
if ! command -v fusermount >/dev/null 2>&1; then
	echo "fusermount ist nicht installiert!"
	exit 3
fi


anzahl=0

for filename in $sourceDir/*.$fileEnding; do
	# Alles ueberspringen was keine NORMALE Datei ist (z.B. im Falle, dass keine Datei gefunden wurde):
	[ -f "$filename" ] || continue

	# Testen ob image-Datei lesbar ist:
	if [ ! -r "$filename" ]; then
		echo "Datei ($filename) ist nicht lesbar!"
		continue
	fi
	
	# Dateinamen von Pfad trennen, dateiendung entfernen und das ergebnis zum mountpoint-pfad zusammensetzen:
	baseName=$(basename -- "$filename")
	rawName="${baseName%.*}"
	mountPoint="$mountDir/$rawName"

	# Pruefen ob mountpoint bereits verwendet wird:
	if ! egrep -q "^[^ ]* $mountPoint " /proc/mounts; then
		echo "Image $filename ist nicht eingehaengt!"
		continue
	fi	

	# Image ausheangen:
	if ! fusermount -u "$mountPoint"; then
		echo "Image $filaneme konnte nicht von $mountPoint ausgehaengt werden!"
		continue
	fi
	
	
	# Mountpoint entfernen:
	if ! rmdir "$mountPoint"; then
		echo "Mount-ordner $mountPoint konnte nicht entfernt werden!"
		continue
	fi

	((anzahl++))
	echo "Image $filename wurde erfolgreich aus $mountPoint ausgehaengt."
done

echo "$anzahl Images erfolgreich ausgehaengt."
