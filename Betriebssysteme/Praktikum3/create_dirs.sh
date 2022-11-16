#!/ bin / bash -
#===============================================================================
#
# FILE: create_dirs.sh
#
# USAGE: ./create_dirs.sh number_of_dirs
#
# DESCRIPTION: Unterverzeichnisse d0 d1 ... erzeugen
#
# OPTIONS: ---
# REQUIREMENTS: ---
# BUGS: ---
# NOTES: ---
# AUTHOR: 
# ORGANIZATION: Fachhochschule Südwestfalen, Iserlohn
# CREATED: 26.08.2013 18:40
# REVISION: ---
#===============================================================================

#-----------------------------------------------------------------------
# Aufruf / Anzahl der Aufrufparameter überprüfen
#-----------------------------------------------------------------------
if [ ${#} -lt 1 ]
then
echo -e "\n\tAufruf: $0 Anzahl_der_Verzeichnisse\n"
exit 1
fi

#-----------------------------------------------------------------------
# Verzeichnisse anlegen und zählen
#-----------------------------------------------------------------------
anzahl=0
erfolg=0
while [ $anzahl -lt $1 ]
do
  mkdir d$anzahl && ((erfolg++))
  ((anzahl++))
done

#-----------------------------------------------------------------------
# Kontrollausgabe
#-----------------------------------------------------------------------
echo -e " \n${erfolg}/${anzahl} Verzeichnisse angelegt\n"