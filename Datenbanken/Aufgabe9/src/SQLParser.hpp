#pragma once

#include <string_view>
#include <cstdint>
#include <string>


class SQLParseResult {
private:
	bool success_;
	size_t index_;
	std::string error_;

private:
	inline SQLParseResult(const bool success): success_(success) { }
	inline SQLParseResult(const std::string& error, const size_t index): success_(false), error_(error), index_(index) { }

public:
	inline static SQLParseResult Success() { return { true }; }
	inline static SQLParseResult Error(const std::string& msg, const size_t index) { return { msg, index }; }

public:
	inline bool success() const { return success_; }
};


class SQLToken {
public:
	virtual bool parse(const std::string& source, const size_t start) const = 0;
};

class SQLLiteral : public SQLToken {
private:
	std::string literal;
public:
	inline SQLLiteral(const std::string& literal): literal(literal) {}
	virtual bool parse(const std::string& source, const size_t start) const {
	}
};


class SQLParser {
	enum Type: uint8_t {
	};
};