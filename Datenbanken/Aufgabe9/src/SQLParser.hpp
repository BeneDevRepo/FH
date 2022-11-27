#pragma once

#include "SQLTokens.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>


std::ostream& operator<<(std::ostream& cout, const std::unique_ptr<Command>& command) {
	if(command->name.size() > 0)
		cout << command->name << ": ";

	switch(command->type()) {
		case Command::Type::NULL_:
			cout << "<null>";
			break;
		case Command::Type::STRING:
			cout << ((CommandString*)command.get())->str;
			break;

		case Command::Type::LIST:
			{
				const auto& list = ((CommandList*)command.get())->list;
				cout << "[";
				for(size_t i = 0; i < list.size(); i++) {
					std::cout << list[i];
					if(i < list.size() - 1)
						cout << ", ";
				}
				cout << "]";
			}
			break;
	}

	return cout;
}



class SQLParser {
private:
	std::unique_ptr<SQLToken> commands;

public:
	inline SQLParser() {
		SQLToken* exit = new SQLCommand(
			std::string("__exit__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("EXIT"),
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* help = new SQLCommand(
			std::string("__help__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("HELP"),
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* create_table = new SQLCommand(
			std::string("__create_table__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("CREATE"),
			new SQLSpace,
			new SQLLiteral("TABLE"),
			new SQLSpace,
			new SQLIdentifier("_table_"), // table name
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* drop_table = new SQLCommand(
			std::string("__drop_table__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("DROP"),
			new SQLSpace,
			new SQLLiteral("TABLE"),
			new SQLSpace,
			new SQLIdentifier("_table_"), // table name
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* select = new SQLCommand(
			std::string("__select__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("SELECT"),
			new SQLSpace,

			// columns to select:
			new SQLVariant(
				std::string("_columns_"),
				new SQLLiteral("*"),
				new SQLList(new SQLIdentifier)
			),

			new SQLSpace(true),
			new SQLLiteral("FROM"),
			new SQLSpace,

			// source table(s):
			new SQLList(
				std::string("_sources_"),
				new SQLIdentifier
			),

			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* insert = new SQLCommand(
			std::string("__insert__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("INSERT"),
			new SQLSpace,
			new SQLLiteral("INTO"),
			new SQLSpace,
			new SQLIdentifier("_table_"), // target table
			new SQLSpace(true),

			// optional target columns:
			new SQLOptional(
				new SQLParanthesis(
					new SQLList(
						std::string("_columns_"),
						new SQLIdentifier
					)
				)
			),

			new SQLSpace(true),
			new SQLLiteral("VALUES"),
			new SQLSpace(true),

			// list of values to insert:
			new SQLParanthesis(
				new SQLList(
					std::string("_values_"),
					new SQLIdentifier
				)
			),

			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* create_index = new SQLCommand(
			std::string("__create_index__"), // Command Name
			new SQLSpace(true),
			new SQLLiteral("CREATE"),
			new SQLSpace,
			new SQLOptional(
				new SQLCommand(
					new SQLLiteral("UNIQUE"),
					new SQLSpace
				)
			),
			new SQLLiteral("INDEX"),
			new SQLSpace,
			new SQLIdentifier("_index_name_"), // index name
			new SQLSpace,
			new SQLLiteral("ON"),
			new SQLSpace,
			new SQLIdentifier("_table_"), // target table
			new SQLSpace(true),

			new SQLParanthesis(
				new SQLList(
					std::string("_columns_"), // target columns
					new SQLIdentifier()
				)
			),

			new SQLSpace(true),
			new SQLLiteral(";")
		);

		commands = std::make_unique<SQLVariant>(
			exit,
			help,
			create_table,
			drop_table,
			select,
			insert,
			create_index
		);
	}


	inline SQLParseResult parse(const std::string& source, size_t start) {
		// for(; start < source.size() && source[start] == ' '; start++); // skip leading spaces

		auto res = commands->parse(source, start);
		if(res.success()) {
			// std::cout << "Command: " << result.command() << "\n";
			std::cout << "Command: " << res.command() << "\n";
			return res;
		}

		// return res;
		return SQLParseResult::Error("", 0);
	}
};