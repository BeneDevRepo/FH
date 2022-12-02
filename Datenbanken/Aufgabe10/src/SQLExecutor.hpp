#pragma once

#include "Database.hpp"
#include "SQLParser.hpp"

#include <iomanip>
#include <cstring>


class SQLExecutor { // used as namespace
public:

static inline void execute(Database& db, const Command& command, bool& stopProgram) {
	if(command.type() != Command::MAP)
		throw std::runtime_error("Error parsing command: result is not a map");

	// const auto& commandMap = ((const CommandMap&)command).map;
	const CommandMap& commandMap = (const CommandMap&)command;

	// if(commandMap.find("_command_") == commandMap.end())
	if(commandMap.map.find("_command_") == commandMap.map.end())
		throw std::runtime_error("Error parsing command: command has noassociated command type");

	// const Command& type = *commandMap.at("_command_");
	const Command& type = *commandMap.map.at("_command_");

	if(type.type() != Command::STRING)
		throw std::runtime_error("Error parsing command: command type is not of type string");

	const std::string& typeStr = ((const CommandString&)type).str;

	if(typeStr == "exit") {
		stopProgram = true;
	} else if(typeStr == "help") {
		showHelpPage();
	// } else if(typeStr == "create_table") {
	// 	executeCreateTable(db, commandMap);
	} else if(typeStr == "drop") { // drop table
		executeDropTable(db, commandMap);
	} else if(typeStr == "show") { // show table
		executeShow(db, commandMap);
	} else if(typeStr == "select") {
		executeSelect(db, commandMap);
	} else if(typeStr == "insert") {
		executeInsert(db, commandMap);
	} else if(typeStr == "create") { // create_index
		executeCreateIndex(db, commandMap);
	} else if(typeStr == "find") {
		executeFind(db, commandMap);
	}
}

static inline void showHelpPage() {
	std::cout << "<< This sql interpreter supports the following commands:\n";

	std::cout << " <  HELP; -> show this help page\n";
	std::cout << " <  EXIT; -> stops program\n";

	std::cout << " <  DROP TABLE Buch; -> deletes the table\n";

	std::cout << " <  SELECT * FROM Buch [ORDER BY ... ASC/DESC] [WHERE <column> = \"value\"];\n";
	std::cout << " <  SELECT Column1, column... FROM Buch [ORDER BY ... ASC/DESC] [WHERE <column> = \"value\"];\n";

	std::cout << " <  INSERT INTO Buch [(columns...)] values(values...);\n";

	std::cout << " <  CREATE INDEX <name> ON Buch(<column>);\n";
}

// static inline void executeCreateTable(Database& db, const Command& command) {
// }

static inline void executeDropTable(Database& db, const CommandMap& command) {
	db.drop();
}

static inline void executeShow(Database& db, const CommandMap& command) {
	const std::unordered_map<std::string, std::unique_ptr<Command>>& map = command.map; // extract hashmap

	if(map.find("_target_") == map.end()) // check if target is specified
		throw std::runtime_error("Error executing Show command: no target specified");

	const Command& target = *map.at("_target_");

	if(target.type() != Command::STRING)
		throw std::runtime_error("Error executing select: target is not a valid identifier");

	const std::string& targetStr = ((CommandString&)target).str;

	if(targetStr == "tables") {
		std::cout << " - Buch\n";
	} else if (targetStr == "indices") {
		static constexpr const char* COLUMNS[6] {
			"Autor",
			"Titel",
			"Verlagsname",
			"Erscheinungsjahr",
			"Erscheinungsort",
			"ISBN"
		};
		for(size_t i = 0; i < 6; i++) {
			const char* col = COLUMNS[i];
			if(db.hasIndex(col)) {
				std::cout << " - Index on Buch(" << col << ")\n";
			}
		}
	}
}

// Function to print a single database-row using a sorted list of columns:
static inline void printRow(const Buch& row, const std::vector<const Buch::Column*>& columns) {
	std::cout << "| ";

	for(const auto& columnMeta : columns) {
		const size_t& columnSize = columnMeta->size; // for cleaner code; will probably get optimized away
		const std::string& columnName = columnMeta->name; // extract column name

		if(columnMeta->type == Buch::Column::STRING)
			std::cout << std::setw(columnSize) << row.getString(columnName) << " | "; // print column
		else /* if type == Buch::Column::INTEGER */
			std::cout << std::setw(columnSize) << row.getInt(columnName) << " | "; // print column
	}

	std::cout << "\n";
}

static inline void executeSelect(Database& db, const CommandMap& command) {
	const std::unordered_map<std::string, std::unique_ptr<Command>>& map = command.map; // extract hashmap

	const bool explicitColumns = map.find("_columns_") != map.end(); // true if a list of columns is specified, false if * is used

	// Generate a list of column metadata onjects for all columns that should be printed:
	std::vector<const Buch::Column*> columnMeta; // sorted list of metadata for columns stat should be printed
	if(explicitColumns) {
		// validate that columns is a List:
		if(map.at("_columns_")->type() != Command::LIST)
			throw std::runtime_error("Error executing Select: invalid command tree 2");

		const CommandList& columns = (CommandList&)*map.at("_columns_"); // extract column list

		// validate column list:
		for(const auto& column : columns.list) {
			// validate that current colummn name Entry is actually a string:
			if(column->type() != Command::STRING)
				throw std::runtime_error("Error executing Select: invalid command tree: Column name not parsed");

			// validate that column name string is a valid column name:
			const std::string& columnName = ((CommandString&)*column).str;
			// if(Buch::columns.find(columnName) == Buch::columns.end())
			if(!Buch::hasColumn(columnName))
				throw std::runtime_error("Error executing Select: invalid column name <" + columnName + ">");
		}

		// use column names to create list of column metadata objects:
		for(const auto& column : columns.list) {
			const std::string& columnName = ((CommandString&)*column).str; // extract column name
			columnMeta.push_back(&Buch::columnInfo(columnName));
		}
	} else {
		// no explicit columns were specified. add all columns of Buch in correct order instead:
		for (const std::string& columnName : { "autor", "titel", "verlagsname", "erscheinungsjahr", "erscheinungsort", "isbn" })
			columnMeta.push_back(&Buch::columnInfo(columnName));
	}



	// Extract Where-Clause information:
	const bool filter = command.map.find("_where_") != command.map.end(); // true if where-clause was found
	const std::string* filterColumn; // where comparison column name
	const std::string* filterLiteral; // where comparison literal

	if(filter) {
		const Command& whereOptions = *command.map.at("_where_");

		if(whereOptions.type() != Command::MAP)
			throw std::runtime_error("Could not execute select: where clause is not a map");
		
		const std::unordered_map<std::string, std::unique_ptr<Command>>& whereMap = ((CommandMap&)whereOptions).map;

		// check if _where_column_ entry exists:
		if(whereMap.find("_where_column_") == whereMap.end())
			throw std::runtime_error("Could not execute select: where clause does not contain column name");
		
		const Command& filterColumnCommand = *whereMap.at("_where_column_");

		if(filterColumnCommand.type() != Command::STRING)
			throw std::runtime_error("Could no execute select: where clause column name is not a string");
		
		filterColumn = &((CommandString&)filterColumnCommand).str; // extract column name

		// check if column name is valid:
		if(!Buch::hasColumn(*filterColumn))
			throw std::runtime_error("Could not execute select: Where clause: invalid column name");
		
		if(Buch::columnInfo(*filterColumn).type != Buch::Column::STRING)
			throw std::runtime_error("Could not execute select: Where clause: column content is not a string");



		// check if _where_literal_ entry exists:
		if(whereMap.find("_where_literal_") == whereMap.end())
			throw std::runtime_error("Could not execute select: where clause does not contain comparison value name");

		const Command& filterLiteralCommand = *whereMap.at("_where_literal_");

		if(filterLiteralCommand.type() != Command::STRING)
			throw std::runtime_error("Could no execute select: where clause literal is not a string");
		
		filterLiteral = &((CommandString&)filterLiteralCommand).str; // extract comparison literal
	}

	// std::cout << "Where <" + *filterColumn + "> = " + *filterLiteral + "\n";

	// print table header:
	std::cout << "| ";
	for(const auto& columnMeta : columnMeta)
		std::cout << std::setw(columnMeta->size) << columnMeta->name << " | ";
	std::cout << "\n";

	// print horizontal separator:
	std::cout << "+ ";
	for(const auto& columnMeta : columnMeta) {
		for(size_t i = 0; i < columnMeta->size; i++)
			std::cout << "-";
		std::cout << " + ";
	}
	std::cout << "\n";


	const bool sort = map.find("_sort_") != map.end();
	if(sort) {
		// print rows sorted by index:
		const Command& sortParams = *map.at("_sort_");

		if(sortParams.type() != Command::MAP)
			throw std::runtime_error("Error extracting sort parameters");
		
		const std::unordered_map<std::string, std::unique_ptr<Command>>& sortMap = ((CommandMap&)sortParams).map; // map of sort parameters

		if(sortMap.find("_sort_column_") == sortMap.end())
			throw std::runtime_error("Error executing sorted select: could not extract column name");
		
		const Command& columnNameC = *sortMap.at("_sort_column_");

		if(columnNameC.type() != Command::STRING)
			throw std::runtime_error("Error executing sorted select: column name is not a string");
		
		const std::string& columnName = ((CommandString&)columnNameC).str;

		if(!db.hasIndex(columnName))
			throw std::runtime_error("Error: Select: could not sort by \"" + columnName + "\" (column is not indexed)");
		
		const Index& index = db.getIndex(columnName);
		
		
		const Command& sortDirC = *sortMap.at("_sort_direction_");

		if(sortDirC.type() != Command::STRING)
			throw std::runtime_error("Error executing sorted select: sorting direction is not a string");
		
		const std::string& sortDir = ((CommandString&)sortDirC).str;

		const bool ascending = sortDir == "asc";

		if(ascending) {
			for(const auto& entry : index) {
				if(filter)
					if(db.getRow(entry.pos()).getString(*filterColumn) != *filterLiteral)
						continue;
				printRow(db.getRow(entry.pos()), columnMeta);
			}
		} else { // descending:
			for(auto it = index.end(); it != index.begin(); ) {
				--it;
				if(filter)
					if(db.getRow(it->pos()).getString(*filterColumn) != *filterLiteral)
						continue;
				printRow(db.getRow(it->pos()), columnMeta);
			}
		}
		

	} else {
		// print rows in storage order:

		// print selected columns, row by row:
		for(const auto buch : db) {
			if(filter)
				if(buch.getString(*filterColumn) != *filterLiteral)
					continue;
			printRow(buch, columnMeta);
		}
	}
}


/*************************************
 * INSERT SQL Command executor
 */
static inline void executeInsert(Database& db, const CommandMap& command) {
	// db.insert(Buch("Autor", "Titel", "Verlagsname", 1985, "Erscheinungsort", "ISBN"));

	const std::unordered_map<std::string, std::unique_ptr<Command>>& map = command.map; // extract hashmap

	const bool explicitColumns = map.find("_columns_") != map.end(); // true if a list of columns is specified

	/**
	 * Generate a list of column metadata onjects for all columns that were explicitly or implicitly specified:
	 */
	std::vector<const Buch::Column*> columnMeta; // sorted list of metadata for columns stat should be printed
	if(explicitColumns) {
		// validate that columns is a List:
		if(map.at("_columns_")->type() != Command::LIST)
			throw std::runtime_error("Error executing Insert: invalid command tree 2");

		const CommandList& columns = (CommandList&)*map.at("_columns_"); // extract column list

		// validate column list:
		for(const auto& column : columns.list) {
			// validate that current colummn name Entry is actually a string:
			if(column->type() != Command::STRING)
				throw std::runtime_error("Error executing Insert: invalid command tree: Column name not parsed");

			const std::string& columnName = ((CommandString&)*column).str;

			// validate that the specified column actually exists:
			if(!Buch::hasColumn(columnName))
				throw std::runtime_error("Error executing Insert: invalid column name <" + columnName + ">");
		}

		// use column names to create list of column metadata objects:
		for(const auto& column : columns.list) {
			const std::string& columnName = ((CommandString&)*column).str; // extract column name
			columnMeta.push_back(&Buch::columnInfo(columnName));
		}
	}
	else // Otherwise: no explicit columns specified; use list of all columns instead:
	{
		// no explicit columns were specified. add all columns of Buch in correct order instead:
		for (const std::string& columnName : { "autor", "titel", "verlagsname", "erscheinungsjahr", "erscheinungsort", "isbn" })
			columnMeta.push_back(&Buch::columnInfo(columnName));
	}


	std::cout << "Inserting into: ";
	for(const auto& col : columnMeta) {
		std::cout << col->name << ", ";
	}
	std::cout << "\n";


	/**
	 * Extract Values to be inserted into their corresponding columns:
	*/
	// validate that _values_ exists in command map:
	if(map.find("_values_") == map.end()) // true if a list of columns is specified
		throw std::runtime_error("Error executing insert: no values found");
	
	// validate that _values_ field is of type List:
	if(map.at("_values_")->type() != Command::LIST)
		throw std::runtime_error("Error executing insert: invalid _values_ field");

	const std::vector<std::unique_ptr<Command>>& values = ((CommandList&)*map.at("_values_")).list; // extract values list


	// validate number of arguments:
	if(columnMeta.size() != values.size())
		throw std::runtime_error("Could not insert: Number of specified columns does not match number of provided values");


	Buch newBook; // new column

	// --- populate new row:
	for(size_t i = 0; i < columnMeta.size(); i++) {
		const Buch::Column& col = *columnMeta[i];
		const Command& val = *values[i];
		// validate that current colummn name Entry is actually a string:

		void* dest = ((char*)&newBook) + col.offset;

		switch(col.type) {
		case Buch::Column::STRING:
			{
				if(val.type() != Command::STRING)
					throw std::runtime_error("Error: Insert: could not insert non-string value into column \"" + col.name + "\"");

				const std::string& src = ((CommandString&)val).str;
				strncpy((char*)dest, src.c_str(), col.size);
			}
			break;
		case Buch::Column::INTEGER:
			if(val.type() != Command::INTEGER)
				throw std::runtime_error("Error: Insert: could not insert non-integer value into column \"" + col.name + "\"");

			*(int*)dest = ((CommandInt&)val).value;
			break;
		}
	}

	db.insert(newBook); // insert new row
}

static inline void executeCreateIndex(Database& db, const CommandMap& command) {
	const std::unordered_map<std::string, std::unique_ptr<Command>>& map = command.map; // extract hashmap

	// validate that column list exists:
	if(map.find("_columns_") == map.end())
		throw std::runtime_error("Error: Create Index: columns not found");
	
	const Command& columns = *map.at("_columns_");

	if(columns.type() != Command::LIST)
		throw std::runtime_error("Error: Create Index: column list is not a list");
	
	const std::vector<std::unique_ptr<Command>>& columnList = ((CommandList&)columns).list;

	if(columnList.size() == 0)
		throw std::runtime_error("Error creating index: no column specified");
	
	const Command& column = *columnList[0];

	if(column.type() != Command::STRING)
		throw std::runtime_error("Error: Create Index: column is not a string");
	
	const std::string& columnString = ((CommandString&)column).str;


	db.createIndex(columnString);
}


static inline void executeFind(Database& db, const CommandMap& command) {
	const std::unordered_map<std::string, std::unique_ptr<Command>>& whereMap = command.map; // extract hashmap

	// Extract Where-Clause information:


	// const std::unordered_map<std::string, std::unique_ptr<Command>>& whereMap = ((CommandMap&)whereOptions).map;

	// check if _where_column_ entry exists:
	if(whereMap.find("_where_column_") == whereMap.end())
		throw std::runtime_error("Could not execute find: where clause does not contain column name");
	
	const Command& filterColumnCommand = *whereMap.at("_where_column_");

	if(filterColumnCommand.type() != Command::STRING)
		throw std::runtime_error("Could no execute find: where clause column name is not a string");
	
	const std::string& filterColumn = ((CommandString&)filterColumnCommand).str; // extract column name

	// check if column name is valid:
	if(!Buch::hasColumn(filterColumn))
		throw std::runtime_error("Could not execute find: Where clause: invalid column name");

	if(Buch::columnInfo(filterColumn).type != Buch::Column::INTEGER)
		throw std::runtime_error("Could not execute find: Where clause: column is not of integer type");



	// check if _where_literal_ entry exists:
	if(whereMap.find("_where_literal_") == whereMap.end())
		throw std::runtime_error("Could not execute find: where clause does not contain comparison value name");

	const Command& filterLiteralCommand = *whereMap.at("_where_literal_");

	if(filterLiteralCommand.type() != Command::INTEGER)
		throw std::runtime_error("Could no execute find: where clause literal is not an integer");

	const int& filterLiteral = ((CommandInt&)filterLiteralCommand).value; // extract comparison literal

	const size_t ind = db.getIndex(filterColumn).find(filterLiteral);

	std::cout << "Result: " << db.getRow(ind) << "\n";

	// std::cout << "Find " << filterColumn << " = " << filterLiteral << "\n";
}

};