import java.sql.*;

import java.util.Scanner;


public class Main {
    final static String userName = "SYSDBA";
    final static String password = "masterkey";

    final static String dbFile = "C:/Users/Bened/Desktop/FH/Datenbanken/Aufgabe12/Aufgabe12.FDB"; // Laptop
    // final static String dbFile = "A:/_Info_Projects/FH/Datenbanken/Aufgabe12/Aufgabe12.FDB"; // PC


	public static void printResult(ResultSet res) throws SQLException {
		ResultSetMetaData resMeta = res.getMetaData();

		final int numColumns = resMeta.getColumnCount();

		int[] columnLengths = new int[numColumns];
		for(int i = 0; i < numColumns; i++) {
			switch(resMeta.getColumnType(1 + i)) {
				case Types.INTEGER:
				case Types.FLOAT:
				case Types.DOUBLE:
				case Types.DECIMAL:
					columnLengths[i] = 10;
					break;
				
				case Types.CHAR:
				case Types.VARCHAR:
					columnLengths[i] = resMeta.getColumnDisplaySize(1 + i);
					break;
				
				default:
					System.out.println("Error: unknown column type!");
					break;
			}
		}

		String[] columnFormats = new String[numColumns];
		for(int i = 0; i < numColumns; i++)
			columnFormats[i] = "| %" + columnLengths[i] + "s ";
		
		
		for(int i = 0; i < numColumns; i++)
			System.out.printf(columnFormats[i], resMeta.getColumnName(1 + i));
		System.out.println("|");
		
		while(res.next()) {
			for(int i = 0; i < numColumns; i++) {
				System.out.printf(columnFormats[i], res.getString(1 + i));
			}
			System.out.println("|");
		}

		System.out.println();
	}

	public static void printQueryResult(Connection conn, String query) throws SQLException {
		System.out.println("Querying: " + query);
		Statement statement = conn.createStatement();
        ResultSet res = statement.executeQuery(query);

		printResult(res);

		res.close();
		statement.close();
	}
	
	public static void executeStatement(Connection conn, String command) throws SQLException {
		System.out.println("Executing: " + command);
		Statement statement = conn.createStatement();

		int res = statement.executeUpdate(command);
		System.out.printf("%d rows affected.\n", res);
		System.out.println();

		statement.close();
		conn.commit();
	}

	public static int readID(Scanner cin) {
		int id = -1;
		for(;;) {
			System.out.print("Please enter id: ");
			if(cin.hasNextInt()) {
				id = cin.nextInt();
				cin.nextLine(); // clear input
				break;
			}
			
			System.out.println("Could not parse integer, please try again...");
			cin.nextLine(); // clear input
		}
		return id;
	}

	public static void main(String[] args) {
        try {
            run();
        } catch(Exception e) {
            System.out.println("\n\nException thrown: " + e);
        }
    }

    public static void run() throws Exception {
        Class.forName("org.firebirdsql.jdbc.FBDriver");
        Connection conn = DriverManager.getConnection("jdbc:firebirdsql://localhost:3050/" + dbFile, userName, password);
		
		final boolean autoComm = conn.getAutoCommit();
		conn.setAutoCommit(false);
		
        System.out.println("\n\nConnected to database");


		System.out.println("Type help for more information...");
		


		Scanner cin = new Scanner(System.in);  // Create a Scanner object

		boolean done = false;
		while(!done) {
			System.out.println("Enter Command: ");
			System.out.print(" > ");

			String command = cin.nextLine();  // Read user input
			// System.out.println("Command: " + command);  // Output user input
			int id = -1;
			switch(command) {
				case "help":
					System.out.println(" Available Commands:");
					System.out.println(" - exit - disconnects database and stops program");
					System.out.println(" - print - prints all datasets");
					System.out.println(" - show\\n <id> prints specified dataset");
					System.out.println(" - rm\\n <id> removes specified dataset");
					System.out.println(" - set\\n <id> overwrites specified dataset");

					break;
				
				case "exit":
					done = true;
					break;
				
				case "print":
					printQueryResult(conn, "SELECT * FROM Buch;");
					break;
					
				case "show":
					id = readID(cin);
					printQueryResult(conn, "SELECT * FROM Buch WHERE BuchOID = " + id + ";");
					break;
					
				case "rm":
					id = readID(cin);
					executeStatement(conn, "DELETE FROM Buch WHERE BuchOID = " + id + ";");
					break;
					
				case "set":
					id = readID(cin);
					System.out.print("Please Enter new Title: ");
					String newTitle = cin.nextLine();
					System.out.print("Please Enter new Author: ");
					String newAuthor = cin.nextLine();

					// primitiv (erlaubt SQL-Injection):
					// executeStatement(conn, "UPDATE Buch SET Titel = '" + newTitle + "', Autor = '" + newAuthor + "' WHERE BuchOID = " + id + ";");

					// Korrekter (keine SQL-Injections):
					PreparedStatement updteStatement = conn.prepareStatement("UPDATE Buch SET Titel = ?, Autor = ? WHERE BuchOID = ? ;");
					updteStatement.setString(1, newTitle);
					updteStatement.setString(2, newAuthor);
					updteStatement.setInt(3, id);
					updteStatement.executeUpdate();
					conn.commit();
					break;
					
				default:
					System.out.println("Unknown command. Type help for more information...");
					break;

			}
		}

		cin.close();

		conn.setAutoCommit(autoComm);
		conn.close();
    }
}