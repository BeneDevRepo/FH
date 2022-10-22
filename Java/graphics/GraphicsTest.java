import java.awt.image.*;
import java.util.concurrent.*;

import lib.*;


public class GraphicsTest {
	static GWindow window;

	static class SDF implements Sdf.SDF {
		public double at(final vec3 p) {
			final double sp1 = Sdf.sphere(p, .2f);
			final double sp2 = Sdf.sphere(vec3.sub(p, new vec3(-.5f, 0, 0)), .5f);
			return Math.min(sp1, sp2);
		}
	}

	static class Renderer implements GWindow.Renderer {
		double t = 0;

		vec3 pixelColor(final double x, final double y) {
			return new vec3(x / 2 + .5, Math.abs(t / 3. % 2. - 1.), y / 2 + .5);
		}

		@Override
		public void render(BufferedImage tex) {
			byte[] pixels =
				((DataBufferByte) tex.getRaster().getDataBuffer()).getData();

			for(int y = 0; y < tex.getHeight(); y++) {
				for(int x = 0; x < tex.getWidth(); x++) {
					final vec3 col = pixelColor(
						x * 2.f / tex.getWidth() - 1.f,
						y * 2.f / tex.getHeight() - 1.f);

					pixels[(y * tex.getWidth() + x) * 3 + 2] = (byte)(col.getX() * 255);
					pixels[(y * tex.getWidth() + x) * 3 + 1] = (byte)(col.getY() * 255);
					pixels[(y * tex.getWidth() + x) * 3 + 0] = (byte)(col.getZ() * 255);
				}
			}

			t += 1. / 16;
		}
	};

	public static void main(String[] args) {
		window = new GWindow(800, 800, new Renderer());

		ScheduledExecutorService executorService = Executors.newSingleThreadScheduledExecutor();
    	executorService.scheduleAtFixedRate(
			() -> { window.render(); },
			0,
			16,
			TimeUnit.MILLISECONDS);
	}
}
