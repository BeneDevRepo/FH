import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import sample.*;

/**
 * Testklasse für {@link Sample} Implementationen.
 *
 * @author M. Faulstich
 */
public class SampleTestMain {
	public static final int REPEAT_TEST = 1000;

	public static final int MAX_SAMPLE_ELEMENTS = 10;

	/**
	 * Startet den Test.
	 * 
	 * Geprüft werden die Implementationen
	 * <ul>
	 * <li>{@link DummySample},</li>
	 * <li>{@link FloydBentleyIterativeSample} und</li>
	 * <li>{@link FloydBentleyRecursiveSample}.</li>
	 * </ul>
	 * 
	 * 
	 * Ein Test besteht aus der Ziehung von 0 bis 10 Elementen aus einer Menge
	 * von 10 {@link Integer} Werten 0...9.</br>
	 * Geprüft wird, dass
	 * <ul>
	 * <li>die Ergebnismenge keine null-Referenz ist,</li>
	 * <li>die Ergebnismenge die korrekte Anzahl an Elementen enthält,</li>
	 * <li>die Ergebnismenge keine null-Werte enthält und</li>
	 * <li>kein in der Ergebnismenge enthaltenes Element mehrfach
	 * vorkommt.</li>
	 * </ul>
	 *
	 * @param args
	 *           Konsolenparameter. Werden nicht verwendet.
	 */
	public static void main(String[] args) {
		List<Integer> sample = null;
		
		Sample<Integer> implementation = null;
		int elementsToDraw = 0;
		try {
			Sample[] implementations = {
				new DummySample<Integer>(),
				new FloydBentleyIterativeSample<Integer>(),
				new FloydBentleyRecursiveSample<Integer>() };
			
			// Alle Implementierungen von Sample aus implementations testern
			Iterator<Sample> sampleIterator =
				Arrays.stream(implementations).iterator();
			while (sampleIterator.hasNext()) {
				implementation = sampleIterator.next();

				// 1000 Wiederholungen für alle Tests.
				for (int testRun = 0; testRun < REPEAT_TEST; testRun++) {
					// Menge 0 bis MAX_SAMPLE_ELEMENTS Elemente prüfen
					for (int listSize = 0; listSize <= MAX_SAMPLE_ELEMENTS; listSize++) {
						
						// Menge anlegen und füllen
						sample = new ArrayList<Integer>();
						for (int sampleElement = 0; sampleElement < listSize; sampleElement++) {
							sample.add(sampleElement);
						}

						// Testlauf:
						// 0 bis Anzahl der Elemente ziehen
						System.out.printf(
							"%s: Testlauf %d: Ziehe 0...%d aus %d Elementen\n",
							implementation.getClass().getSimpleName(),
							testRun + 1, sample.size(), sample.size());
						for (elementsToDraw = 0; elementsToDraw <= sample.size(); elementsToDraw++) {
							List<Integer> drawSample = implementation.sample(sample,
								elementsToDraw);

							// Ziehungsergebnis anzeigen
							System.out.println(drawSample);

							// Ist Ergebnisliste unzulässigerweise null-Referenz?
							if (drawSample == null) {
								throw new IllegalStateException(
									String.format(
										"%s: Test fehlgeschlagen!\nErgebnismenge ist eine null-Referenz.",
										implementation.getClass().getSimpleName()
									)
								);
							}

							// Entspricht die Anzahl der gezogenen Elemente der Anzahl, die gezogen werden sollte?
							if (drawSample.size() != elementsToDraw) {
								throw new IllegalStateException(String.format(
										"%s: Test fehlgeschlagen!\nVersuch der Ziehung von %d Elementen, ergaben %d Elemente.",
										implementation.getClass().getSimpleName(), elementsToDraw,
										drawSample.size()));
							}

							// Gibt es unzulässige null-Referenzen in der Ergebnismenge? 
							int nullCount = nullcountNullOccurences(drawSample);
							if (nullCount > 0) {
								throw new IllegalStateException(String.format(
										"%s: Test fehlgeschlagen!\n%d ungültige null Referenzen in Ergebnismenge enthalten.",
										implementation.getClass().getSimpleName(),
										nullCount));
							}

							// Kommt ein Element in der Ergebnismege mehrfach vor?
							Integer occurences;
							if ((occurences = checkMultipleOccurences(drawSample)) != null) {
								throw new IllegalStateException(String.format(
										"%s: Test fehlgeschlagen!\nErgebnismenge enthält %d mehrfach.",
										implementation.getClass().getSimpleName(),
										occurences));
							}

						}

						System.out.printf("\n\n");
					}
				}
			}
			System.out.printf(
				"*** Alle Tests aller Implementationen erfolgreich! ***");
		} catch (Exception e) {
			System.err.printf(
				"Implementierung %s, Prüfliste %s Anzahl, die gezogen werden soll: %d\n",
				implementation.getClass().getSimpleName(),
				sample.toString(),
				elementsToDraw);
			System.err.println(e.getMessage());
			// Nur für den Programmierer:
			e.printStackTrace();
		}
	}

	/**
	 * Gibt die Anzahl der in der Menge enthaltenen null-Referenzen zurück.
	 *
	 * @param sample
	 *           Menge.
	 * @return Anzahl der gefundenen null-Referenzen.
	 */
	private static int nullcountNullOccurences(List<Integer> sample) {
		int result = 0;
		for (Integer integer : sample)
			if (integer == null)
				result++;

		return result;
	}

	/**
	 * Prüft, ob ein Element mehrfach in der Menge vorkommt.
	 *
	 * @param drawSample
	 *           Menge.
	 * @return Anzahl der gefundenen Vorkommen des ersten mehrfach vorkommenden
	 *         Elements.
	 */
	private static Integer checkMultipleOccurences(List<Integer> drawSample) {
		for (Integer integer : drawSample) {
			int count = 0;

			for (Integer i : drawSample)
				if (integer.equals(i))
					count++;

			if (count > 1)
				return integer;
		}

		return null;
	}
}
