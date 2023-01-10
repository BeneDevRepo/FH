import Counting.WordCount;

public class Main {
	public static void main(String[] args) {
		try {
			WordCount wc = new WordCount("Grundgesetz.txt");
			WordCount wc2 = new WordCount("Grundgesetz2.txt");
		} catch(Exception e) {
			e.printStackTrace(System.err);
		}
	}
}