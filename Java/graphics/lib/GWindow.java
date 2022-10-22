package lib;

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

public class GWindow {
    private JFrame frame;

    public interface Renderer {
        public void render(BufferedImage texture);
    }

    public GWindow(final int width, final int height, final Renderer renderer) {
        frame = new JFrame("Window");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setResizable(false);
        frame.setVisible(true);

        BufferedImage frameBuffer = new BufferedImage(width, height, BufferedImage.TYPE_3BYTE_BGR);

        JPanel panel = new JPanel() {
            public void paintComponent(java.awt.Graphics g) {
                renderer.render(frameBuffer);

                Graphics2D g2d = (Graphics2D)g;
                g2d.drawImage(frameBuffer, null, 0, 0);
            }
        };

        panel.setPreferredSize(new java.awt.Dimension(width, height));
        panel.setVisible(true);

        frame.add(panel);
        frame.pack();
    }

    public void render() {
        frame.repaint();
    }
}