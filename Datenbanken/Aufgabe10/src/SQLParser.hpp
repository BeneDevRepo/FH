#pragma once

#include "SQLTokens.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>


std::ostream& operator<<(std::ostream& cout, const std::unique_ptr<Command>& command) {
	switch(command->type()) {
		case Command::Type::NULL_:
			cout << "<null>";
			break;

		case Command::Type::STRING:
			cout << "\'" << ((CommandString*)command.get())->str << "\'";
			break;

		case Command::Type::INTEGER:
			cout << ((CommandInt*)command.get())->value;
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

		case Command::Type::MAP:
			{
				const auto& map = ((CommandMap*)command.get())->map;
				cout << "{";
				size_t i = 0;
				for(const auto& element : map) {
					std::cout << element.first << ": " << element.second;
					if(i < map.size() - 1)
						cout << ", ";
				}
				cout << "}";
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
			new SQLSpace(true),
			new SQLLiteral("EXIT", "_command_"), // Command Name
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* help = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("HELP", "_command_"), // Command Name
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* create_table = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("CREATE", "_command_"), // Command Name
			new SQLSpace,
			new SQLLiteral("TABLE"),
			new SQLSpace,
			new SQLIdentifier("_table_"), // table name
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* drop_table = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("DROP", "_command_"), // Command Name
			new SQLSpace,
			new SQLLiteral("TABLE"),
			new SQLSpace,
			new SQLIdentifier("_table_"), // table name
			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* show = new SQLCommand(
			new SQLSpace(true),
			new SQLVariant(
				std::string("_command_"),
				new SQLLiteral("SHOW"),
				new SQLLiteral("LIST")
			),
			new SQLSpace,
			new SQLIdentifier("_target_")
		);

		SQLToken* select = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("SELECT", "_command_"), // command name
			new SQLSpace,

			// columns to select:
			new SQLVariant(
				std::string("_columns_"),
				new SQLLiteral("*"),
				new SQLList(
					new SQLIdentifier
				)
			),

			new SQLSpace(true),
			new SQLLiteral("FROM"),
			new SQLSpace,

			// source table(s):
			new SQLList(
				std::string("_sources_"),
				new SQLIdentifier
			),


			new SQLOptional(
				std::string("_sort_"),
				new SQLCommand(
					// new SQLSpace,
					new SQLVariant(
						new SQLLiteral("SORT"),
						new SQLLiteral("ORDER")
					),
					new SQLSpace,
					new SQLLiteral("BY"),
					new SQLSpace,
					new SQLIdentifier("_sort_column_"),
					new SQLSpace,
					new SQLVariant(
						std::string("_sort_direction_"),
						new SQLLiteral("ASC", " "),
						new SQLLiteral("DESC", " ")
					)
				)
			),

			new SQLSpace(true),
			new SQLOptional(
				std::string("_where_"),
				new SQLCommand(
					new SQLLiteral("WHERE"),
					new SQLSpace,
					new SQLIdentifier("_where_column_"),
					new SQLSpace(true),
					new SQLLiteral("="),
					new SQLSpace(true),
					new SQLString("_where_literal_")
				)
			),

			new SQLSpace(true),
			new SQLLiteral(";")
		);
	
		SQLToken* select_v2 = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("SELECT", "_command_"), // command name
			new SQLSpace,

			// columns to select:
			new SQLVariant(
				std::string("_columns_"),
				new SQLLiteral("*"),
				new SQLList(
					new SQLIdentifier
				)
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

			new SQLOptional(
				std::string("_where_"),
				new SQLCommand(
					new SQLLiteral("WHERE"),
					new SQLSpace,
					new SQLIdentifier("_where_column_"),
					new SQLSpace(true),
					new SQLLiteral("="),
					new SQLSpace(true),
					new SQLString("_where_literal_")
				)
			),

			new SQLSpace(true),

			new SQLOptional(
				std::string("_sort_"),
				new SQLCommand(
					// new SQLSpace,
					new SQLVariant(
						new SQLLiteral("SORT"),
						new SQLLiteral("ORDER")
					),
					new SQLSpace,
					new SQLLiteral("BY"),
					new SQLSpace,
					new SQLIdentifier("_sort_column_"),
					new SQLSpace,
					new SQLVariant(
						std::string("_sort_direction_"),
						new SQLLiteral("ASC", " "),
						new SQLLiteral("DESC", " ")
					)
				)
			),

			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* insert = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("INSERT", "_command_"), // Command Name
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
					// new SQLIdentifier
					new SQLVariant(
						new SQLString,
						new SQLInteger
					)
				)
			),

			new SQLSpace(true),
			new SQLLiteral(";")
		);

		SQLToken* create_index = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("CREATE", "_command_"), // Command Name
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


		SQLToken* find = new SQLCommand(
			new SQLSpace(true),
			new SQLLiteral("FIND", "_command_"),
			new SQLSpace,
			new SQLIdentifier("_where_column_"),
			new SQLSpace(true),
			new SQLLiteral("="),
			new SQLSpace(true),
			new SQLVariant(
				std::string("_where_literal_"),
				new SQLString,
				new SQLInteger
			)
		);

		commands = std::make_unique<SQLVariant>(
			exit,
			help,
			create_table,
			drop_table,
			show,
			select,
			select_v2,
			insert,
			create_index,
			find
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