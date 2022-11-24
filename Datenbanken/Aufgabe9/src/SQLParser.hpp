#pragma once

#include "SQLTokens.hpp"

#include <cstdint>
#include <string>
#include <memory>
#include <vector>


class SQLParser {
private:
	std::vector<std::unique_ptr<SQLToken>> commands;

public:
	inline SQLParser() {
		std::unique_ptr SELECT = std::make_unique<SQLCommand>(
				new SQLSpace(true),
				new SQLLiteral("SELECT"),
				new SQLSpace,
				new SQLVariant(
					new SQLLiteral("*"),
					new SQLList(new SQLIdentifier)
					// new SQLLiteral("Autor")
				),
				new SQLSpace(true),
				new SQLLiteral("FROM"),
				new SQLSpace,
				new SQLLiteral("Buch"),
				new SQLSpace(true),
				new SQLLiteral(";")
			);
		commands.push_back(std::move(SELECT));

		commands.push_back(
			std::make_unique<SQLCommand>(
				new SQLSpace(true),
				new SQLLiteral("INSERT"),
				new SQLSpace,
				new SQLLiteral("INTO"),
				new SQLSpace,
				new SQLLiteral("Buch"),

				new SQLVariant(
					new SQLCommand(
						new SQLSpace(true),
						new SQLLiteral("("),
							new SQLSpace(true),
						new SQLLiteral(")"),
						new SQLSpace(true)
					),
					new SQLSpace
				),

				new SQLLiteral("VALUES"), new SQLSpace(true),
				new SQLLiteral("("),
					new SQLSpace(true),
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
			if(command->parse(result, source, start)) {
				return true;
			}
		}

		return false;
	}
};