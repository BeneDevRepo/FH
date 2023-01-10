package Counting;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Set;
import java.util.TreeMap;

/**
 * Eine Klasse zum Zählen von Wörtern in einer Text-Datei
 */
public class WordCount {
	private TreeMap<String, Integer> counts;

	/**
	 * interne Methode um ein einzelnes Wort zu verarbeiten (den dazugehörigen Zähler zu erstellen / zu erhöhen)
	 * @param word Das zu zählende Wort
	 */
	private void processWord(String word) {
		if(!counts.containsKey(word)) {
			// Falls wort zum ersten Mal auftritt: <1> einfuegen:
			counts.put(word, 1);
		} else {
			// Andernfalls: Anzahl inkrementieren:
			counts.put(word, counts.get(word) + 1);
		}

		// System.out.println(word + ": " + counts.get(word));
	}

	/**
	 * interne Methode um eine einzelne Zeile einer Datei zu verarbeiten (zerlegen + wörter zählen)
	 * @param line Die zu verarbeitende Zeile
	 */
	private void processLine(String line) {
		String[] words = line.split("\\p{Space}|\\p{Punct}");
		for(String word : words) {
			// Leere Wörter überspringen:
			if(word.isBlank())
				continue;

			processWord(word); // Wort verarbeiten (zählen)
		}
	}

	/**
	 * Erstellt ein WordCount Objekt, liest die Datei ein und zählt die Vorkommen aller Wörter
	 * @param filename Dateipfad der zu lesenden Datei
	 */
	public WordCount(String filename) {
		counts = new TreeMap<>(); // TreeMap zum Speichern der Ergebnisse erstellen

		// Parameterprüfung:
		{
			File tmp = new File(filename); // Temporäres Datei-Objekt um zu prüfen ob filename auf eine valide Datei zeigt

			// Pruüfen ob die Datei existiert:
			if(!tmp.exists())
				throw new RuntimeException("Error: WordCount(): file at path <" + filename + "> does not exists.");

			// Prüfen ob die Datei lesbar ist:
			if(!tmp.canRead())
				throw new RuntimeException("Error: WordCount(): file at path <" + filename + "> is not readable.");
		}

		// Datei zeile für Zeile einlesen:
		try (BufferedReader f = new BufferedReader(new FileReader(filename))) {
			String line = null;
			while ((line = f.readLine()) != null)
				processLine(line);
		} catch (IOException e) {
			e.printStackTrace(System.err);
		}
	}

	/**
	 * Gibt ein Set mit allen gezählten Wörtern (allen wörtern die mindestens einmal in der Eingabedatei vorkamen) zurück
	 * @return Menge aller Wörter die mindestens 1 mal gezählt wurden
	 */
	public Set<String> getWords() {
		return counts.keySet();
	}

	/**
	 * gibt die Anzahl aller Vorkommen eines bestimmten Wortes in der Input datei zurück
	 * @param word Das zu untersuchende Wort
	 * @return Die Anzahl des Wortes in des Input-Datei. 0 Falls das Wort nicht vorkam
	 */
	public int getWordCount(String word) {
		if(!counts.containsKey(word))
			return 0;

		return counts.get(word);
	}
}
