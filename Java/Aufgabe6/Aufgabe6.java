import java.util.ArrayList;
import java.util.Vector;
import java.util.LinkedList;
import java.util.Iterator;

/**
 * Aufgabe 6
 */
final class Aufgabe6 {
	private final static int NUM_ELEMENTS_1 = 1000 * 1000 * 100;
	private final static int NUM_ELEMENTS_2 = 1000 * 100;
	

	/**
	 * Privater Konstruktor in final class, um Instanzierung zu verhindern
	 */
	private Aufgabe6() {
	}


	/**
	 * Objekt-Wrapper um integer
	 */
	private static class MutableInt {
		public int val;

		/**
		 * Konstruktor
		 * @param val Wert des Objekts
		 */
		public MutableInt() {
			this.val = 0;
		}

		public MutableInt(int val) {
			this.val = val;
		}
	}


	/**
	 * Aufgabe 1 implementierung
	 */
	private static void aufgabe1() {
		// Test-Vektor erstellen und füllen:
		Vector<MutableInt> test = new Vector<MutableInt>();
		for(int i = 0; i < NUM_ELEMENTS_1; i++)
			test.add(new MutableInt(i));
	
	
		
		// Index-basierter Zugriff:
		long time1 = System.currentTimeMillis();
		for(int i = 0; i < NUM_ELEMENTS_1; i++) {
			MutableInt mi = test.elementAt(i);
			mi.val = 7;
		}
		time1 = System.currentTimeMillis() - time1;
		
		
		// Iterator-basierter Zugriff:
		long time2 = System.currentTimeMillis();
		for(Iterator<MutableInt> it = test.iterator(); it.hasNext(); ) {
			MutableInt mi = it.next();
			mi.val = 5;
		}
		time2 = System.currentTimeMillis() - time2;
	
		
		// Foreach-basierter Zugriff:
		long time3 = System.currentTimeMillis();
		for(MutableInt mi : test)
			mi.val = 3;
		time3 = System.currentTimeMillis() - time3;
	
	
		final String format = "%-10s | %6d ms\n";
		System.out.println("Type       |  Time");
		System.out.println("-----------+----------");
		System.out.printf(format, "Index", time1);
		System.out.printf(format, "Iterator", time2);
		System.out.printf(format, "Foreach", time3);
	}
	

	/**
	 * Aufgabe 2
	 */
	private static void aufgabe2() {
		final long timeAA = ListBenchmark.testInsertStart (new ArrayList<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);
		final long timeAE = ListBenchmark.testInsertEnd   (new ArrayList<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);
		final long timeAR = ListBenchmark.testInsertRandom(new ArrayList<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);

		final long timeVA = ListBenchmark.testInsertStart (new Vector<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);
		final long timeVE = ListBenchmark.testInsertEnd   (new Vector<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);
		final long timeVR = ListBenchmark.testInsertRandom(new Vector<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);

		final long timeLA = ListBenchmark.testInsertStart (new LinkedList<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);
		final long timeLE = ListBenchmark.testInsertEnd   (new LinkedList<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);
		final long timeLR = ListBenchmark.testInsertRandom(new LinkedList<MutableInt>(), new MutableInt(), NUM_ELEMENTS_2);

		final String format = "%-20s | %6d ms\n";
		System.out.println("Type                 |  Time");
		System.out.println("---------------------+----------");
		System.out.printf(format, "ArrayList Anfang",	timeAA);
		System.out.printf(format, "ArrayList Ende",		timeAE);
		System.out.printf(format, "ArrayList Random",	timeAR);
		System.out.println("---------------------+----------");
		System.out.printf(format, "Vector Anfang",		timeVA);
		System.out.printf(format, "Vector Ende",		timeVE);
		System.out.printf(format, "Vector Random",		timeVR);
		System.out.println("---------------------+----------");
		System.out.printf(format, "LinkedList Anfang",	timeLA);
		System.out.printf(format, "LinkedList Ende",	timeLE);
		System.out.printf(format, "LinkedList Random",	timeLR);
	}
	

	/**
	 * Main-Methode (Führt tests aus)
	 * @param args unused
	 */
	public static void main(String[] args) {
		System.out.println("Aufgabe 1:");
		aufgabe1();
		System.out.println();
		
		System.out.println("Aufgabe 2:");
		aufgabe2();
		System.out.println();
	}
}