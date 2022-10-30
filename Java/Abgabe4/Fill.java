import java.util.Arrays;
import java.lang.StringBuilder;

/**
 * Test-Class implementing and comparing different String-Concatenation techniques
 * @author Prof. Dr. Steins
 */
public final class Fill {
	final static int NUM_CHARS_STRCONC   =        100 * 1000; // Used for String Concatenation
	final static int NUM_CHARS_OPTIMIZED = 100 * 1000 * 1000; // Used for char[] and StringBuilder

	/**
	 * Empty private constructor
	 */
	private Fill() {
	}

	/**
	 * Fills string of <length> with char c
	 * @param length Length of output String
	 * @param c Character to fill String with
	 * @return output-string
	 */
	public static String createStringFilledWith_stringConcatenation(int length, char c) {
		String result = "";
		for (int i = 0; i < length; i++)
			result += c;
		return result;
	}

	// ==========  Aufgabe 2  ==========:
	/**
	 * Fills string of <length> with char c
	 * @param length Length of output String
	 * @param c Character to fill String with
	 * @return output-string
	 */
	public static String createStringFilledWith_charArray(int length, char c) {
		if(length < 1)
			return "";

		char[] chars = new char[length];
		Arrays.fill(chars, c);
		return new String(chars);
	}

	// ==========  Aufgabe 3  ==========:
	/**
	 * Fills string of <length> with char c
	 * @param length Length of output String
	 * @param c Character to fill String with
	 * @return output-string
	 */
	public static String createStringFilledWith_stringBuilder(int length, char c) {
		StringBuilder strBuild = new StringBuilder(length);
		for(int i = 0; i < length; i++)
			strBuild.append(c);
		return strBuild.toString();
	}

	/**
	 * Main Method containing test program
	 * @param args unused
	 */
	public static void main(String[] args) {
		String format = "|%-20s|%13d|%10d|";

		System.out.println("|Methode             |Laenge /char |Dauer /ms |");

		long start;
		
		start = System.currentTimeMillis();
		Fill.createStringFilledWith_stringConcatenation(NUM_CHARS_STRCONC, 'x');
		System.out.println(
			String.format(format,
				"Stringverkettung",
				NUM_CHARS_STRCONC,
				System.currentTimeMillis() - start
			)
		);
		
		start = System.currentTimeMillis();
		Fill.createStringFilledWith_charArray(NUM_CHARS_OPTIMIZED, 'x');
		System.out.println(
			String.format(format,
				"char-array",
				NUM_CHARS_OPTIMIZED,
				System.currentTimeMillis() - start
			)
		);

		start = System.currentTimeMillis();
		Fill.createStringFilledWith_stringBuilder(NUM_CHARS_OPTIMIZED, 'x');
		System.out.println(
			String.format(format,
				"StringBuilder",
				NUM_CHARS_OPTIMIZED,
				System.currentTimeMillis() - start
			)
		);
	}
}