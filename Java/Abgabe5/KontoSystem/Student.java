package KontoSystem;


public class Student extends Person {
	public Student(String vorname, String nachname) {
		super(vorname, nachname);
	}

	@Override
	public String toString() {
		return super.toString() + ", (Student)";
	}
}
