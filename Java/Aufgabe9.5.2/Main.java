import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import Counting.*;

/**
 * Einstiegspunkt, enthält test code
 */
public class Main {
	/**
	 * testet ob WordCount und ParallelWordCount korrekt funktionieren
	 * @throws IOException
	 * @throws InterruptedException
	 */
	static void test1() throws IOException, InterruptedException {
		WordCount wc = new WordCount("Grundgesetz.txt");
		ParallelWordCount pwc = new ParallelWordCount("Grundgesetz.txt", 4, 8);

		// Mengen aller gefundenen Wörter extrahieren:
		Set<String> words = wc.getWords();
		Set<String> pwords = pwc.getWords();

		// Beide Klassen sollten die Gleichen Wörter, und dementsprechend auch die gleiche Anzahl an einzigartigen Wörtern finden:
		if(words.size() != pwords.size())
			throw new RuntimeException("Size missmatch!!! " + words.size() + " vs. " + pwords.size());

		// Testen ob die beiden Ergebnismengen (rein im Bezug auf die gefundenen Wörter) tatsächlich identisch sind:
		for(int i = 0; i < words.size(); i++) {
			String word = (String)words.toArray()[i];
			String pword = (String)pwords.toArray()[i];

			if(!pwords.contains(word))
				throw new RuntimeException("Error! pwords does not contain " + word);

			if(!words.contains(pword))
				throw new RuntimeException("Error! words does not contain " + pword);
		}

		// Testen ob die Anzahlen aller gefundenen Wörter zwischen beiden Methoden übereinstimmen:
		for(String word : words) {
			if(wc.getWordCount(word) != pwc.getWordCount(word)) {
				throw new RuntimeException(
					"Error! Word <" + word + "> was counted "
					+ wc.getWordCount(word) + " times by WordCount, and "
					+ pwc.getWordCount(word) + " times by ParallelWordCount!");
			}
		}

		// Test-Ausgabe:
		System.out.printf("%-50s | %-10s\n", "Wort", "Anzahl");
		String format = "%-50s | %10d\n";
		for(String word : words)
			System.out.printf(format, word, wc.getWordCount(word));
	}

	/**
	 * testet ParallelWordCountBenchmark
	 */
	static void benchmark() throws IOException, InterruptedException {
		// Benchmark-Parameter generieren:
		ArrayList<Integer> numThreads = new ArrayList<>();
		ArrayList<Integer> numLinesPerThread = new ArrayList<>();

		for(int n = 1; n <= Runtime.getRuntime().availableProcessors() * 2; n++)
			numThreads.add(n);

		numLinesPerThread.add(1);
		for(int n = 1; n <= 10; n++)
			numLinesPerThread.add(n * 10);

		// Benchmark ausführen:
		List<ParallelWordCountBenchmark.Result> res =
			ParallelWordCountBenchmark.benchmark(
				"Grundgesetz.txt",
				numThreads,
				numLinesPerThread);

		// Ergebnisse ausdrucken:
		System.out.printf("%10s | %10s | %10s\n", "Threads", "L/Thread", "Time");
		for(ParallelWordCountBenchmark.Result entry : res)
			System.out.printf("%10d | %10d | %10dms\n", entry.numThreads, entry.numLinesPerThread, entry.tMillis);
	}

	/**
	 * Einstiegspunkt, enthält tests
	 * @param args ignored
	 */
	public static void main(String[] args) {
		try {
			test1();
			// benchmark();

			// Falls irgendein Test schiefgelaufen wäre und eine Exception geworfen worden wäre, würde dieser Punkt nicht erreicht werden:
			System.out.println("All tests passed Successfully");
		} catch(Exception e) {
			System.out.println("Tests failed. Exception:");
			e.printStackTrace(System.err);
		}
	}
}