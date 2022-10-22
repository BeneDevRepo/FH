package lib;

public class Sdf {
    public interface SDF {
        public double at(final vec3 pos);
    }

    public static double march(final SDF sdf, final Ray ray) {
		double dist = 0;

		int numIterations = 0;
		
		double minD = 0;

		do {
			minD = sdf.at(ray.at(dist));
			dist += minD;
		} while(Math.abs(minD) > .001 && numIterations++ < 100);

		return dist;
	}

    public static double sphere(final vec3 p, double r) {
		return p.mag() - r;
	}
}
