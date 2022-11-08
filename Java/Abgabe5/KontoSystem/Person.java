package KontoSystem;

import java.util.List;
import java.util.ArrayList;

/**
 * Repräsentiert eine Person
 */
public class Person {
	private final String vorname;
	private final String nachname;

	private List<Konto> kontos;

	/**
	 * Erstellt eine Person mit Vor- und Nachnamen
	 * @param vorname (nicht null)
	 * @param nachname (nucht null)
	 */
	public Person(String vorname, String nachname) {
		if(vorname == null)
			throw new IllegalArgumentException("Jede Person muss einen Vornamen haben.");
		if(nachname == null)
			throw new IllegalArgumentException("Jede Person muss einen Nachnamen haben.");

		this.vorname = vorname;
		this.nachname = nachname;
		
		kontos = new ArrayList<Konto>();
	}

	/**
	 * Fügt ein Konto zur Besitzliste dieser Person hinzu,
	 * @param konto Konto (nicht null)
	 */
	void addKonto(Konto konto) {
		if(konto == null)
			throw new IllegalArgumentException("Konnte konto null nicht zu Person hinzufügen.");

		if(!kontos.contains(konto))
			kontos.add(konto);
	}

	/**
	 * Erstellt einen beschreibenden String aus Vorname, Nachname und Anzahl der besessenen Konten
	 */
	@Override
	public String toString() {
		return String.format("%s %s hat %d %s.", vorname, nachname, kontos.size(), kontos.size() == 1 ? "Konto" : "Konten");
	}
}
