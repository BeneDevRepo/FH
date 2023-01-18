package Counting;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileReader;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.TreeMap;
import java.util.Set;


/**
 * Eine Klasse zum Zählen von Wörtern in einer Text-Datei
 */
public class WordCount {
	private TreeMap<String, Integer> counts; // TreeMap zum speichern der Anzahl jedes gezählten Wortes

	/**
	 * Erstellt ein WordCount Objekt, liest die Datei ein und zählt die Vorkommen aller Wörter
	 * @param filename Dateipfad der zu lesenden Datei
	 */
	public WordCount(String filename) throws IOException {
		counts = new TreeMap<>(); // TreeMap zum Speichern der Ergebnisse erstellen

		// Parameterprüfung:
		if(filename == null)
			throw new RuntimeException("Error: WordCount(): Null is not a valid filename");

		File file = new File(filename); // Datei-Objekt erstellen

		// Prüfen ob die Datei existiert:
		if(!file.exists())
			throw new RuntimeException("Error: WordCount(): file at path <" + filename + "> does not exists.");
		
		// Prüfen ob file auf eine normale Datei zeigt:
		if(!file.isFile())
			throw new RuntimeException("Error: WordCount(): file at path <" + filename + "> is not a regular file.");

		// Prüfen ob die Datei lesbar ist:
		if(!file.canRead())
			throw new RuntimeException("Error: WordCount(): file at path <" + filename + "> is not readable.");

		// Datei zeile für Zeile einlesen:
		// try (BufferedReader f = new BufferedReader(new FileReader(filename, StandardCharsets.UTF_8))) {
		try (BufferedReader f = new BufferedReader(new FileReader(file, StandardCharsets.UTF_8))) {
			String line = null;
			while ((line = f.readLine()) != null)
				processLine(line);
		} catch (IOException e) {
			throw e;  // Das try-catch ist lediglich dafür verantwortlich, die Reader korrekt freizugeben; die eventuelle ioexception wird weitergeworfen
		}
	}

	/**
	 * interne Methode um eine einzelne Zeile einer Datei zu verarbeiten (zerlegen + wörter zählen)
	 * @param line Die zu verarbeitende Zeile
	 */
	private void processLine(String line) {
		String[] words = line.split("[\\p{Space}\\p{Punct}\\p{Cntrl}]"); // Zeile in ein Array aus (möglichen) Wörtern zerteilen

		// Alle (möglichen) Wörter durchgehen
		for(String word : words) {
			// Leere Wörter überspringen:
			if(word.isBlank())
				continue;
			
			// "Wörter" mit 2 oder weniger Buchstaben überspringen:
			if(word.length() <= 2)
				continue;

			processWord(word.toLowerCase()); // Wort verarbeiten (zählen)
		}
	}

	/**
	 * interne Methode um ein einzelnes Wort zu verarbeiten (den dazugehörigen Zähler zu erstellen / zu erhöhen)
	 * @param word Das zu zählende Wort
	 */
	private void processWord(String word) {
		counts.putIfAbsent(word, 0); // 0 einsetzen falls Zähler nicht existiert
		counts.put(word, counts.get(word) + 1); // Zähler erhöhen
	}

	/**
	 * Gibt ein nicht veränderbares Set mit allen gezählten Wörtern (allen wörtern die mindestens einmal in der Eingabedatei vorkamen) zurück
	 * @return Menge aller Wörter die mindestens 1 mal gezählt wurden
	 */
	public Set<String> getWords() {
		return Collections.unmodifiableSet(counts.keySet()); // unmodifiableSet() erstellt eine nicht veränderbare "ansicht" auf counts.keySet()
	}

	/**
	 * gibt die Anzahl aller Vorkommen eines bestimmten Wortes in der Input datei zurück
	 * @param word Das zu untersuchende Wort
	 * @return Die Anzahl des Wortes in des Input-Datei. 0 Falls <word> nicht vorkam
	 */
	public int getWordCount(String word) {
		// Parameterüberprüfung:
		if(word == null)
			throw new RuntimeException("Error: WordCount::getWordCount(): null is not a valid word");

		// Wort wurde nicht gezählt (0 mal)
		if(!counts.containsKey(word))
			return 0;

		return counts.get(word);
	}
}
