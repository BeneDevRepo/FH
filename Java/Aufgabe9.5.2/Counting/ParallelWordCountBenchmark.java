package Counting;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ParallelWordCountBenchmark {
	public static List<Result> benchmark(String filename, List<Integer> numThreads, List<Integer> numLinesPerThread)throws IOException, InterruptedException {
		ArrayList<Result> res = new ArrayList<>();
		for(Integer nThreads : numThreads) {
			for(Integer nLines : numLinesPerThread) {
				Result currentRes = new Result(); // Datenkapsel für aktuelle Messung

				currentRes.numThreads = nThreads;
				currentRes.numLinesPerThread = nLines;
				currentRes.tMillis = (int)(System.nanoTime() / 1000); // Startzeit speichern

				new ParallelWordCount(filename, nLines, nThreads); // Zeilen zählen

				currentRes.tMillis = (int)(System.nanoTime() / 1000) - currentRes.tMillis; // Endzeit einlesen + differenz berechnen

				res.add(currentRes); // Datenkapsel zu Liste hinzufügen
			}
		}
		return res;
	}

	public static class Result {
		public int numThreads;
		public int numLinesPerThread;
		public int tMillis;
	}
}
