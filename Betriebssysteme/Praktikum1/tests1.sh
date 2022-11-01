cat NichtExistenteDatei.txt 2>/dev/null || echo "Datei Existiert nicht"
cat tests1.sh 1>/dev/null && echo "Scriptdatei Existiert"
