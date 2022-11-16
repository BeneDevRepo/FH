import java.util.List;
import java.util.Random;

public final class ListBenchmark {
	private ListBenchmark() {
	}

	public static <E> long testInsertStart(List<E> list, E object, final long NUM_OBJECTS) {
		final long start = System.currentTimeMillis();

		for(long i = 0; i < NUM_OBJECTS; i++)
			list.add(0, object);

		return System.currentTimeMillis() - start;
	}

	public static <E> long testInsertEnd(List<E> list, E object, final long NUM_OBJECTS) {
		final long start = System.currentTimeMillis();

		for(long i = 0; i < NUM_OBJECTS; i++)
			list.add(list.size(), object);

		return System.currentTimeMillis() - start;
	}

	public static <E> long testInsertRandom(List<E> list, E object, final long NUM_OBJECTS) {
		Random rand = new Random(System.currentTimeMillis());

		final long start = System.currentTimeMillis();

		for(long i = 0; i < NUM_OBJECTS; i++)
			list.add(rand.nextInt(list.size() + 1), object);

		return System.currentTimeMillis() - start;
	}
}
