#pragma once

#include <initializer_list>
#include <cstdint>
#include <cctype> // tolower
#include <string>
#include <memory>
#include <vector>


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



struct Command {
    enum Type { SELECT, INSERT };
    virtual inline Type type() const = 0;
};

struct SelectCommand : public Command {
    virtual inline Type type() const { return SELECT; }
    std::string table;
    std::vector<std::string> columns;
};

struct InsertCommand : public Command {
    virtual inline Type type() const { return INSERT; }
    std::string table;
    std::vector<std::string> columns;
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
    inline void setOutput(std::string& output) {}
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



class SQLList : public SQLToken { // consumes following spaces
private:
	std::unique_ptr<SQLToken> token;
	SQLSpace optSpace;
    SQLLiteral comma;

public:
	inline SQLList(SQLToken *const token):
			token(token), optSpace(true), comma(",") {}
	inline SQLList(std::unique_ptr<SQLToken> token):
			token(std::move(token)), optSpace(true), comma(",") {}
	inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		const size_t index_orig = index;

        // for(size_t num_elements = 0; ; num_elements++) {
        for(;;) {
            if(!token->parse(result, source, index)) {
                return false;
            }

            optSpace.parse(result, source, index); // optional space before comma

            if(!comma.parse(result, source, index)) // comma
                return true;

            optSpace.parse(result, source, index); // optional space after comma
        }

		index = index_orig; // exit without altering index

		return true;
	}
};