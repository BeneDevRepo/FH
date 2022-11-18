#pragma once

#include <string_view>
#include <cstdint>
#include <string>
#include <cctype> // tolower
#include <memory>


class SQLParseResult {
private:
	bool success_;
	size_t index_;
	std::string error_;

private:
	inline SQLParseResult(const bool success): success_(success) { }
	inline SQLParseResult(const std::string& error, const size_t index): success_(false), error_(error), index_(index) { }

public:
	inline SQLParseResult() { }
	inline static SQLParseResult Success() { return { true }; }
	inline static SQLParseResult Error(const std::string& msg, const size_t index) { return { msg, index }; }

public:
	inline bool success() const { return success_; }
	inline const std::string& error() const { return error_; }
	inline size_t errorIndex() const { return index_; }
};


class SQLToken {
public:
	virtual ~SQLToken(){}
	virtual bool parse(SQLParseResult& result, const std::string& source, const size_t start) const = 0;
};

class SQLLiteral : public SQLToken {
private:
	std::string literal;

public:
	inline SQLLiteral(const std::string& literal): literal(literal) {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, const size_t start) const {
		for(size_t i = 0; i < literal.size(); i++) {
			if(start + i >= source.size()) {
				return false;
			}

			if(tolower(literal[i]) != tolower(source[start + i])) {
				return false;
			}
		}

		return true;
	}
};


class SQLParser {
private:
	std::vector<std::unique_ptr<SQLToken>> commands;

public:
	inline SQLParser() {
		commands.push_back(std::make_unique<SQLLiteral>("SELECT"));
		commands.push_back(std::make_unique<SQLLiteral>("INSERT"));
	}

	inline bool parse(SQLParseResult& result, const std::string& source, const size_t start) {
		for(const std::unique_ptr<SQLToken>& command : commands) {
			if(command->parse(source, start)) {
				return true;
			}
		}
	}
};