package KontoSystem;

import java.util.List;
import java.util.ArrayList;


public class Person {
	private final String vorname;
	private final String nachname;

	private List<Konto> kontos;

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

		if(kontos.contains(konto))
			return; // konto gehört dieser Person bereits

		kontos.add(konto);
	}

	@Override
	public String toString() {
		return vorname + " " + nachname;
	}
}
