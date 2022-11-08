import KontoSystem.*;


public class Aufgabe5 {
	public static final int EURO = 100; // 1 Euro = 100 Cents

	public static void main(String[] args) {
		boolean exception; // true falls exception korrekt geworfen wurde

		Person person = new Person("Max", "Mustermann");
		Konto konto = new Konto(person, 500 * EURO);

		System.out.print("1000 Euro anlegen: ");
		konto.einzahlen(1000 * EURO);
		System.out.println(konto);

		System.out.print("500 Euro abheben: ");
		konto.abheben(500 * EURO);
		System.out.println(konto);
		
		System.out.print("1000 Euro abheben: ");
		konto.abheben(1000 * EURO);
		System.out.println(konto);



		// Exceptions testen:
		System.out.print("Test: 1 Euro abheben (Überzieht Kreditlimit): ");
		try {
			exception = false;
			konto.abheben(1 * EURO);
		} catch (RuntimeException re) {
			exception = true;
		}
		System.out.println(exception ? "Erfolg: Exception geworfen" : "Fehler: Exception nicht geworfen");

		System.out.print("Test: Konto mit person=null: ");
		try {
			exception = false;
			new Konto(null);
		} catch (RuntimeException re) {
			exception = true;
		}
		System.out.println(exception ? "Erfolg: Exception geworfen" : "Fehler: Exception nicht geworfen");

		System.out.print("Test: negativer Dispositionskredit: ");
		try {
			exception = false;
			new Konto(person, -1);
		} catch (RuntimeException re) {
			exception = true;
		}
		System.out.println(exception ? "Erfolg: Exception geworfen" : "Fehler: Exception nicht geworfen");
	}
}