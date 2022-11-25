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
			std::cout << "[";
			for(const std::unique_ptr<Command>& item : ((CommandList*)command.get())->list)
				std::cout << item << ", ";
			std::cout << "]";
			break;
	}

	return cout;
}



class SQLParser {
private:
	std::vector<std::unique_ptr<SQLToken>> commands;

public:
	inline SQLParser() {
		commands.push_back(
			std::make_unique<SQLCommand>(
				std::string("Select"), // Command Name
				new SQLSpace(true),
				new SQLLiteral("SELECT"),
				new SQLSpace,
				new SQLVariant(
					std::string("Columns"), // Name of column group
					new SQLLiteral("*"),
					new SQLList(new SQLIdentifier)
				),
				new SQLSpace(true),
				new SQLLiteral("FROM"),
				new SQLSpace,
				new SQLLiteral("Buch"),
				new SQLSpace(true),
				new SQLLiteral(";")
			)
		);

		commands.push_back(
			std::make_unique<SQLCommand>(
				std::string("Insert"), // Command Name
				new SQLSpace(true),
				new SQLLiteral("INSERT"),
				new SQLSpace,
				new SQLLiteral("INTO"),
				new SQLSpace,
				new SQLLiteral("Buch"),

				new SQLVariant(
					std::string("Columns"), // Name of target column Group
					new SQLCommand(
						new SQLSpace(true),
						new SQLLiteral("("),
							new SQLList(new SQLIdentifier),
							// new SQLSpace(true),
						new SQLLiteral(")"),
						new SQLSpace(true)
					),
					new SQLSpace
				),

				new SQLLiteral("VALUES"),
				new SQLSpace(true),
				new SQLLiteral("("),
					new SQLList(new SQLIdentifier),
					// new SQLSpace(true),
				new SQLLiteral(")"),
				new SQLSpace(true),
				new SQLLiteral(";")
			)
		);
		// commands.push_back(std::make_unique<SQLLiteral>("INSERT"));
	}

	inline bool parse(SQLParseResult& result, const std::string& source, size_t start) {
		// for(; start < source.size() && source[start] == ' '; start++); // skip leading spaces

		for(const std::unique_ptr<SQLToken>& command : commands) {
			// if(command->parse(result, source, start)) {
			auto res = command->parse(source, start);
			if(res.success()) {
				// std::cout << "Command: " << result.command() << "\n";
				std::cout << "Command: " << res.command() << "\n";
				return true;
			}
		}

		return false;
	}
};