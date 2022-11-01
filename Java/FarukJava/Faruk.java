import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
// import java.util.*;

public final class Faruk {
	public static void main(String[] args) {
		final int length = 1000;
		final int high = 500;

		final int sthigh = 150; //for the start Button but to use only central 
		final int stlength = 30;


		/*
		 * the Window settings
		 */
		JFrame frame = new JFrame("FingFong"); //make the object for the frame window
		frame.setSize(length, high); //how the window is big from the width and high
		frame.setLocation(500, 150);//where the window on the display came
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //when you close the window that really close
		frame.setResizable(false); //that the window can't change

		/**
		 * Button settings
		 */
		JButton button = new JButton ("Spiel Starten !"); //make the object for the button "spiel starten"
		button.setSize(sthigh,stlength); //how the window is big from the width and high
		button.setLocation(400,100);// position 
		button.setLayout(null);
		
		button.addActionListener(new ActionListener(){	
		@Override
		public void actionPerformed(ActionEvent e) {
				JFrame gameframe = new JFrame("FinFong");
				gameframe.setSize(length, high); //how the window is big from the width and high
				gameframe.setLocation(500, 150);//where the window on the display came
				gameframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //when you close the window that really close
				gameframe.setResizable(false);

				// frame.add(gameframe); // <-----
				gameframe.setLayout(null);
				gameframe.setVisible(true);
			}
		});
		/**
		 * Bestenliste Button setting
		 */
		JButton bestl = new JButton ("Bestenliste!"); //make the object for the button "Bestenliste"
		bestl.setSize(sthigh,stlength); //how the window is big from the width and high
		bestl.setLocation(400,150);// position
		bestl.setLayout(null);
		
		/**
		 * Finish Button setting
		 */
		JButton End = new JButton ("End "); //make the object for the button "spiel starten"
		End.setSize(sthigh,stlength); //how the window is big from the width and high
		End.setLocation(400,200);// position 
		End.setLayout(null);
		
		/**
		 * Game name settings 
		 */
		JLabel gamename = new  JLabel ("PingPong"); //make the object for the headline
		gamename.setFont(new Font("MV BOLI",Font.HANGING_BASELINE,70));//font size and how color
		gamename.setSize(5000,100); //how the window is big from the width and high
		gamename.setLocation(320,10);// position 
		gamename.setLayout(null);
		
		/**
		 *Add the Buttons and Label 
		 */
		frame.add(gamename);
		frame.getContentPane().setBackground(Color.ORANGE);
		frame.add(button);
		frame.add(bestl);
		frame.add(End);
		frame.setLayout(null);
		frame.setVisible(true);
	}
}