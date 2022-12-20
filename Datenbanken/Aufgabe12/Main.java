import java.sql.*;

// import java.util.Properties;
// import javax.sql.*;


public class Main {
    final static String userName = "SYSDBA";
    final static String password = "masterkey";

    final static String dbFile = "C:/Users/Bened/Desktop/FH/Datenbanken/Aufgabe12/Aufgabe12.FDB"; // Laptop
    // final static String dbFile = "A:/_Info_Projects/FH/Datenbanken/Aufgabe12/Aufgabe12.FDB"; // PC


    public static void main(String[] args) {
        try {
            run();
        } catch(Exception e) {
            System.out.println("Exception thrown: " + e);
        }
    }

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

    public static void run() throws SQLException, ClassNotFoundException {
        Class.forName("org.firebirdsql.jdbc.FBDriver");
        Connection conn = DriverManager.getConnection("jdbc:firebirdsql://localhost:3050/" + dbFile, userName, password);

        System.out.println("Connected to database");

		boolean autoComm = conn.getAutoCommit();
		conn.setAutoCommit(false);

		System.out.print("\n\n\n");

		// printQueryResult(conn, "SELECT * FROM Buch;");
		// printQueryResult(conn, "SELECT * FROM Buch WHERE titel = 'Herr der Ringe 1';");



		// executeStatement(conn, "insert into Buch values(3, 'TestBuch', 'Max Mustermann');");
		
		printQueryResult(conn, "SELECT * FROM Buch;");

		// executeStatement(conn, "UPDATE Buch SET Autor = 'Angela Merkel' WHERE Autor = 'Max Mustermann';");
		executeStatement(conn, "UPDATE Buch SET Autor = 'Max Mustermann' WHERE Autor = 'Angela Merkel';");

		printQueryResult(conn, "SELECT * FROM Buch;");

		conn.setAutoCommit(autoComm);
		conn.close();
    }
}