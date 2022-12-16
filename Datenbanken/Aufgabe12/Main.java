import java.sql.*;
import java.util.Properties;

// import javax.sql.*;


public class Main {
    final static String userName = "SYSDBA";
    final static String password = "masterkey";
    final static String serverName = "localhost";
    final static String dbFile = "C:/Users/Bened/Desktop/FH/Datenbanken/Aufgabe12/Aufgabe12.FDB";
    final static String driverName = "org.firebirdsql.jdbc.FBDriver";
    // final static int portNumber = 8080;

    public static void main(String[] args) {
        try {
            run();
        } catch(Exception e) {
            System.out.println("Exceprion thrown: " + e);
        }
    }

    public static void run()
            throws SQLException, ClassNotFoundException {
        Properties connectionProps = new Properties();
        connectionProps.put("user", userName);
        connectionProps.put("password", password);

        Class.forName(driverName);

        Connection conn = DriverManager.getConnection(
            "jdbc:firebird:native:localhost" + ":" + dbFile);
            // connectionProps);
        System.out.println("Connected to database");

        conn.createStatement().executeQuery("select * from Buch;");

        System.out.println("Hello World");
    }
}