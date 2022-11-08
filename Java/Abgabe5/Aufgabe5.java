import KontoSystem.*;


public class Aufgabe5 {
	public static final int EURO = 100; // 1 Euro = 100 Cents

	public static void main(String[] args) {
		{
			System.out.print("Person Erstellen: ");
			Person person = new Person("Max", "Mustermann");
			System.out.println(person);

			System.out.print("Konto Erstellen: ");
			Konto konto = new Konto(person, 500 * EURO);
			System.out.println(konto);

			System.out.print("1000 Euro anlegen: ");
			konto.einzahlen(1000 * EURO);
			System.out.println(konto);

			System.out.print("500 Euro abheben: ");
			konto.abheben(500 * EURO);
			System.out.println(konto);

			System.out.print("1000 Euro abheben: ");
			konto.abheben(1000 * EURO);
			System.out.println(konto);
		}

		System.out.println();
		{
			System.out.print("Student Erstellen: ");
			Student student = new Student("Max", "Mustermann");
			System.out.println(student);

			System.out.print("Konto Erstellen: ");
			Konto konto = new Konto(student, 500 * EURO);
			System.out.println(konto);
		}


		System.out.print("Testing Exceptions... ");
		boolean error = false;
		try {
			testExceptions();
		} catch(RuntimeException e) {
			error = true;
			System.out.println("FAILED: " + e.getMessage());
		}
		if(!error)
			System.out.println("Success!");
	}

	private static void testExceptions() {
		// ----- Person testen:
		// Person ohne Vorname:
		try {
			new Person(null, "Mustermann");
			throw new RuntimeException("Person mit vorname=null hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}

		// Person ohne Nachname:
		try {
			new Person("Max", null);
			throw new RuntimeException("Person mit nachname=null hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}

		// ----- Konto testen:
		// Konto ohne Person:
		try {
			new Konto(null);
			throw new RuntimeException("Konto ohne Besitzer (besitzer=null) hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}
		
		// Negativer Disposotopnskredit:
		try {
			Person p = new Person("Max", "Mustermann");
			new Konto(p, -1);
			throw new RuntimeException("Konto mit negativem Dispositionskredit hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}

		// Konto Überziehen:
		try {
			Person p = new Person("Max", "Mustermann");
			Konto k = new Konto(p, 500);
			k.abheben(501 * EURO);
			throw new RuntimeException("Konto überziehen hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}
		
		// Negativen Wert abheben:
		try {
			Person p = new Person("Max", "Mustermann");
			Konto k = new Konto(p);
			k.einzahlen(100 * EURO);
			k.abheben(-1 * EURO);
			throw new RuntimeException("Negativen Betrag von Konto abheben hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}
		
		// Negativen Wert einzahlen:
		try {
			Person p = new Person("Max", "Mustermann");
			Konto k = new Konto(p);
			k.einzahlen(-1 * EURO);
			throw new RuntimeException("Negativen Betrag auf Konto einzahlen hat keine Exception geworfen!");
		} catch (IllegalArgumentException e) {
		}
	}
}