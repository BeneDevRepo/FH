#pragma once

#include "Database.hpp"
#include "SQLParser.hpp"

class SQLExecutor { // used as namespace
public:

static inline void execute(Database& db, const Command& command, bool& stopProgram) {
	const std::string& type = command.name;

	if(type == "__exit__")
		stopProgram = true;
	else if(type == "__help__")
		showHelpPage();
	else if(type == "__create_table__")
		executeCreateTable(db, command);
	else if(type == "__drop_table__")
		executeDropTable(db, command);
	else if(type == "__select__")
		executeSelect(db, command);
	else if(type == "__insert__")
		executeInsert(db, command);
	else if(type == "__create_index__")
		executeCreateIndex(db, command);
}

static inline void showHelpPage() {
	std::cout << "<< This sql interpreter supports the following commands:\n";

	std::cout << " <  EXIT; -> stops program\n";
	std::cout << " <  HELP; -> show this help page\n";

	std::cout << " <  CREATE TABLE Buch; -> creates the example table\n";
	std::cout << " <  DROP TABLE Buch; -> deletes the example table\n";

	std::cout << " <  SELECT * FROM Buch;\n";
	std::cout << " <  SELECT Column1, column2 FROM Buch;\n";
}

static inline void executeCreateTable(Database& db, const Command& command) {
}

static inline void executeDropTable(Database& db, const Command& command) {
	db.drop();
}

static inline void executeSelect(Database& db, const Command& command) {
	for(const auto buch : db) {
		std::cout << buch << "\n";
	}
}

static inline void executeInsert(Database& db, const Command& command) {
	db.insert(Buch("Autor", "Titel", "Verlag", 1985, "Erscheinungsort", "ISBN"));
}

static inline void executeCreateIndex(Database& db, const Command& command) {
	db.createIndex("Autor");
}

};