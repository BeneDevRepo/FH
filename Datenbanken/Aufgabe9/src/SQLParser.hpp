#pragma once

#include <initializer_list>
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
	virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const = 0; // on error, index is left unchanged
};



class SQLLiteral : public SQLToken {
private:
	std::string literal;

public:
	inline SQLLiteral(const std::string& literal): literal(literal) {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		for(size_t i = 0; i < literal.size(); i++) {
			if(index + i >= source.size()) {
				return false;
			}

			if(tolower(literal[i]) != tolower(source[index + i])) {
				return false;
			}
		}

		index += literal.size();

		return true;
	}
};



class SQLIdentifier : public SQLToken {
public:
	inline SQLIdentifier() {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		constexpr auto validIDChar =
			[](const char c) -> bool {
				return (c >= 'a' && c <= 'z') || c == '_';
			};
		
		const size_t index_orig = index;

		for(;;) {
			if(index >= source.size() || !validIDChar(tolower(source[index]))) {
				if(index == index_orig) {
					return false; // an identifier hat to consist of at least 1 character
				}

				return true; // Identifier ended; success.
			}

			tolower(source[index++]); // TODO: use
		}
	}
};



class SQLSpace : public SQLToken {
private:
	bool optional; // if optional, 0..n spaces can be present. otherwise, there has to be at least one space
public:
	inline SQLSpace(const bool optional = false): optional(optional) {}
	// inline static SQLSpace Optional() { return SQLSpace(true); }
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		constexpr auto isSpace = 
			[](const char c) -> bool {
				return c == ' ' || c == '\t';
			};

		if(!optional) {
			if(index >= source.size() || !isSpace(source[index]))
				return false;

			index++;
		}

		while(index < source.size() && isSpace(source[index])) {
			index++;
		}

		return true;
	}
};



class SQLCommand : public SQLToken {
private:
	std::vector<std::unique_ptr<SQLToken>> parts;

public:
	template<typename ... Args>
	inline SQLCommand(Args ... parts) {
		(this->parts.emplace_back(parts), ...);
	}
	inline SQLCommand(std::vector<std::unique_ptr<SQLToken>> parts):
			parts(std::move(parts)) {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		const size_t index_orig = index;
		for(auto part = parts.begin(); part != parts.end(); ++part) {
			if(!(*part)->parse(result, source, index)) {
				index = index_orig;
				return false;
			}
		}

		return true;
	}
};



class SQLVariant : public SQLToken {
private:
	std::vector<std::unique_ptr<SQLToken>> parts;

public:
	template<typename ... Args>
	inline SQLVariant(Args ... parts) {
		(this->parts.emplace_back(parts), ...);
	}
	inline SQLVariant(std::vector<std::unique_ptr<SQLToken>> parts):
			parts(std::move(parts)) {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		// const size_t index_orig = index;
		for(auto part = parts.begin(); part != parts.end(); ++part) {
			if((*part)->parse(result, source, index)) {
				return true;
			}

			// index = index_orig; // try again or exit without altering index
		}

		return false;
	}
};



class SQLOptional : public SQLToken {
private:
	std::unique_ptr<SQLToken> token;

public:
	inline SQLOptional(SQLToken *const token):
			token(token) {}
	inline SQLOptional(std::unique_ptr<SQLToken> token):
			token(std::move(token)) {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		const size_t index_orig = index;

		if(token->parse(result, source, index)) {
			return true;
		}

		index = index_orig; // exit without altering index

		return true;
	}
};



class SQLList : public SQLToken { // TODO: implement
private:
	std::unique_ptr<SQLToken> token;

public:
	inline SQLList(SQLToken *const token):
			token(token) {}
	inline SQLList(std::unique_ptr<SQLToken> token):
			token(std::move(token)) {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		const size_t index_orig = index;

		if(token->parse(result, source, index)) {
			return true;
		}

		index = index_orig; // exit without altering index

		return true;
	}
};



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
					new SQLIdentifier
					// new SQLLiteral("Autor")
				),
				new SQLSpace,
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