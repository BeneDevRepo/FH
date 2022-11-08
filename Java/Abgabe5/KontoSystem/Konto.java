package KontoSystem;

/**
 * Repräsentiert ein Konto
 */
public class Konto {
	private final Person besitzer;
	private final long dispositionsKredit; // Dispositionskredit in cents

	private long guthaben; // Guthaben in cents

	/**
	 * Erstellt ein Konto
	 * @param besitzer des Kontos (nicht null)
	 * @param dispositionsKredit in cents
	 */
	public Konto(Person besitzer, long dispositionsKredit) {
		if(besitzer == null)
			throw new IllegalArgumentException("Konto ohne Besitzer nicht erlaubt!");
		if(dispositionsKredit < 0)
			throw new IllegalArgumentException("Negativer Dispositionskredit nucht erlaubt!");

		this.besitzer = besitzer;
		this.dispositionsKredit = dispositionsKredit;
		this.guthaben = 0;

		this.besitzer.addKonto(this);
	}

	/**
	 * Erstellt ein Konto mit Dispositionskredit=0
	 * @param besitzer des Kontos (nucht null)
	 */
	public Konto(Person besitzer) {
		this(besitzer, 0);
	}

	/**
	 * Zahlt <betrag> cents auf das Konto ein
	 * @param betrag in cents
	 */
	public void einzahlen(long betrag) {
		if(betrag < 0)
			throw new IllegalArgumentException("Kann keinen negativen Wert anlegen!");

		guthaben += betrag;
	}

	/**
	 * Hebt <betrag> cents vom Konto ab (sofern möglich)
	 * @param betrag
	 */
	public void abheben(long betrag) {
		if(betrag < 0)
			throw new IllegalArgumentException("Kann keinen negativen Wert abheben!");

		if(guthaben + dispositionsKredit < betrag)
			throw new IllegalArgumentException("Konnte nicht abheben: Kredit überzogen!");

		guthaben -= betrag;
	}

	/**
	 * Erstellt einen beschreibenden String für dieses Konto aus Besitzer, Guthaben und Dsipositionskredit
	 */
	@Override
	public String toString() {
		return String.format("Konto von %s, Guthaben %d.%d, Dispo %d.%d",
			besitzer,
			guthaben / 100, guthaben % 100,
			dispositionsKredit / 100, dispositionsKredit % 100);
	}
}
