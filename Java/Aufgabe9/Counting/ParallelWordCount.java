package Counting;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * Eine Klasse zum Zählen von Wörtern in einer Text-Datei, mit multi-threading
 */
public class ParallelWordCount {
	private ConcurrentHashMap<String, Integer> counts;

	/**
	 * Erstellt ein WordCount Objekt, liest die Datei ein und zählt die Vorkommen aller Wörter
	 * @param filename Dateipfad der zu lesenden Datei
	 */
	public ParallelWordCount(String filename) throws IOException, InterruptedException {
		counts = new ConcurrentHashMap<>(); // TreeMap zum Speichern der Ergebnisse erstellen

		// Parameterprüfung:
		if(filename == null)
			throw new RuntimeException("Error: ParallelWordCount(): Null is not a valid filename");

		// {
		File file = new File(filename); // Datei-Objekt erstellen

		// Prüfen ob die Datei existiert:
		if(!file.exists())
			throw new RuntimeException("Error: ParallelWordCount(): file at path <" + filename + "> does not exists.");

		// Prüfen ob file auf eine normale Datei zeigt:
		if(!file.isFile())
			throw new RuntimeException("Error: ParallelWordCount(): file at path <" + filename + "> is not a regular file.");

		// Prüfen ob die Datei lesbar ist:
		if(!file.canRead())
			throw new RuntimeException("Error: ParallelWordCount(): file at path <" + filename + "> is not readable.");
		// }

		// ExecutorService zum Verwalten der Zeilen-Threads erstellen:
		ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
		// ThreadPoolExecutor a = new ThreadPoolExecutor(8, 8, 0, null, null);
		
		// Datei zeile für Zeile einlesen und verarbeiten:
		// try (BufferedReader f = new BufferedReader(new FileReader(filename, StandardCharsets.UTF_8))) {
		try (BufferedReader f = new BufferedReader(new FileReader(file, StandardCharsets.UTF_8))) {
			String line = null;
			while ((line = f.readLine()) != null) {
				LineWordCountRunnable lwc = new LineWordCountRunnable(line); // Runnable für aktuelle Zeile erstellen
				executor.execute(lwc); // runnable in beliebigem thread des executorService ausführen
			}
		} catch (IOException e) {
			throw e; // Das try-catch ist lediglich dafür verantwortlich, die Reader korrekt freizugeben; die eventuelle IOException wird weitergeworfen
		}

		// Warten bis alle Zeilen-Threads fertig abgearbeitet wurden:
		executor.shutdown();
		executor.awaitTermination(60, TimeUnit.SECONDS); // InterruptedException wird in der Main methode gefangen

	}

	/**
	 * private Klasse zum verarbeiten (Wörter zählen) einer einzelnen Zeile text in einem eigenen Thread. Schreibt die Ergebnisse automatisch in die ConcurrentHashMap von ParallelWordCount.
	 */
	private class LineWordCountRunnable implements Runnable {
		private String line; // die zu verarbeitende Zeile

		/**
		 * Konstruktor (Erstellt Objekt, sonst nichts)
		 * @param line zu verarbeitende Zeile, wird im Objekt gespeichert
		 */
		public LineWordCountRunnable(String line) {
			this.line = line;
		}

		/**
		 * Thread-Einstiegspunkt, zerlegt die Zeile und schreibt die Anzahlen der Buchstaben in counts
		 */
		@Override
		public void run() {
			processLine(line);
		}

		/**
		 * interne Methode um ein einzelnes Wort zu verarbeiten (den dazugehörigen Zähler zu erstellen / zu erhöhen)
		 * @param word Das zu zählende Wort
		 */
		private void processWord(String word) {
			counts.putIfAbsent(word, 0); // Sicherstellen dass für <word> ein Counter existiert

			// Compare-exchange-schleife zum Erhöhen des Zählers:
			boolean done = false;
			while(!done) {
				Integer prevCount = counts.get(word);
				done = counts.replace(word, prevCount, prevCount + 1);
			}
		}

		/**
		 * interne Methode um eine einzelne Zeile einer Datei zu verarbeiten (zerlegen + wörter zählen)
		 * @param line Die zu verarbeitende Zeile
		 */
		private void processLine(String line) {
			// String[] words = line.split("[\\p{Space}\\p{Punct}]");
			String[] words = line.split("[\\p{Space}\\p{Punct}\\p{Cntrl}]");
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
	 * @return Die Anzahl des Wortes in des Input-Datei. 0 Falls das Wort nicht vorkam
	 */
	public int getWordCount(String word) {
		if(word == null)
			throw new RuntimeException("Error: ParallelWordCount::getWordCount(): null is not a valid word");

		if(!counts.containsKey(word))
			return 0;

		return counts.get(word);
	}
}
