#!/bin/bash
#===============================================================================
#
# FILE: Praktikum5-Script.sh
#
# USAGE: ./Praktikum5-Script.sh outputfile
#
# DESCRIPTION: ---
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

outfile="report.txt"


# Clear output file
echo "" > "$outfile"
if ! echo "" >> "$outfile" &> /dev/null; then
	echo "Kann nicht in outputdatei schreiben."
	exit 1
fi


# 5.6.1:  Verzeichnisse nach Groesse sortiert auflisten
printf "\nDirectories in / ordered by size, descending:\n" >> "$outfile"
du -S -d1 / 2>/dev/null | sort -nr >> "$outfile"


# 5.6.2:  Dateien nach Groesse sortiert auflisten
printf "\nFiles in /usr/bin ordered by size, descending:\n" >> "$outfile"
ls -lsF /usr/bin | grep "*" | sort -nr >> "$outfile"


# 5.6.3:  Dateien ohne Besitzer
printf "\nFiles in / without user:\n" >> "$outfile"
#cd /
find / -maxdepth 1 -nouser | sort >> "$outfile"



# 5.6.4:  SUID oder SGID
printf "\nFiles in / with SUID or GUID set\n" >> "$outfile"
find / -type f -executable -perm /6000 -ls 2>/dev/null >> "$outfile"
