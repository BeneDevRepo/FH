package KontoSystem;

/**
 * Repräsentiert einen Studenten
 */
public class Student extends Person {
	/**
	 * Erstellt einen Studenten mit vor- und Nachname
	 * @param vorname (nicht null)
	 * @param nachname (nicht null)
	 */
	public Student(String vorname, String nachname) {
		super(vorname, nachname);
	}

	/**
	 * Erstellt einen beschreibenden String aus Vornamen, Nachnamen, Anzahl der besessenen Konten und einer Markierung als Student
	 */
	@Override
	public String toString() {
		return super.toString() + ", (Student)";
	}
}
