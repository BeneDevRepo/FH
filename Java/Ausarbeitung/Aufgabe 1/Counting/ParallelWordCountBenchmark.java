package Counting;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Klasse zum Benchmarken von ParallelWordCount mit unterschiedlichen Parametern
 */
public class ParallelWordCountBenchmark {
	/**
	 * Benchmark-Methode
	 * @param filename Input-Datei dateipfad
	 * @param numThreads // Liste von Thread-Anzahlen die zu testen sind
	 * @param numLinesPerThread // Liste von linesPerThread-Anzahlen, die zu testen sind
	 * @return // Liste aus Result-Objekten, die infos zu Parametern und das Ergebnis der jeweiligen Laufzeitmessung enthalten
	 * @throws IOException
	 * @throws InterruptedException
	 */
	public static List<Result> benchmark(String filename, List<Integer> numThreads, List<Integer> numLinesPerThread)throws IOException, InterruptedException {
		ArrayList<Result> res = new ArrayList<>(); // Liste aus Resultaten zum Speichern der Messungen vorbereiten

		// Alle zu testenden Thread-Anzahlen durchgehen:
		for(Integer nThreads : numThreads) {

			// Alle zu testenden Werte für Zeilen pro Thread durchgehen:
			for(Integer nLines : numLinesPerThread) {
				Result currentRes = new Result(); // Datenkapsel für aktuelle Messung

				// Parameter der aktuellen Messung speichern:
				currentRes.numThreads = nThreads;
				currentRes.numLinesPerThread = nLines;

				currentRes.tMillis = (int)(System.nanoTime() / 1000000); // Startzeit speichern (in millisekunden)

				new ParallelWordCount(filename, nLines, nThreads); // Zeilen zählen

				currentRes.tMillis = (int)(System.nanoTime() / 1000000) - currentRes.tMillis; // Endzeit einlesen + differenz berechnen (in millisekunden)

				res.add(currentRes); // Datenkapsel zu Liste hinzufügen
			}
		}
		return res;
	}

	/**
	 * Klasse zum speichern einse Benchmark-resultats
	 */
	public static class Result {
		public int numThreads; // Anzahl Threads, die im Benchmark genutzt wurden
		public int numLinesPerThread; // Anzahl zeilen / Thread des jeweiligen Benchmarks
		public int tMillis; // gemessene Laufzeit des Benchmarks in Millisekunden
	}
}
