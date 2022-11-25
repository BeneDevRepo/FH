#pragma once

#include <initializer_list>
#include <cstdint>
#include <cctype> // tolower
#include <string>
#include <memory>
#include <vector>


struct Command {
    enum Type { NULL_=0, SELECT, INSERT, STRING, LIST };
    virtual inline Type type() const = 0;
	std::string name;
};

struct CommandNull : public Command {
	CommandNull() = default;
	inline CommandNull(const std::string& name) { this->name = name; }
	virtual inline Type type() const { return NULL_; }
};

struct CommandString : public Command {
	inline CommandString() = default;
	inline CommandString(const std::string& name) { this->name = name; }
	// inline CommandString(const std::string& str) { this->str = str; }
	// inline CommandString(const std::string& str, const std::string& name) { this->str = str; this->name = name; }
	virtual inline Type type() const { return STRING; }
	std::string str;
};

struct CommandList : public Command {
	inline CommandList() = default;
	inline CommandList(const std::string& name) { this->name = name; }
	virtual inline Type type() const { return LIST; }
	std::vector<std::unique_ptr<Command>> list;
};

// struct SelectCommand : public Command {
// 	inline SelectCommand() = default;
// 	inline SelectCommand(const std::string& name) { this->name = name; }
//     virtual inline Type type() const { return SELECT; }
//     CommandString table;
//     std::vector<CommandString> columns;
// };

// struct InsertCommand : public Command {
// 	inline InsertCommand() = default;
// 	inline InsertCommand(const std::string& name) { this->name = name; }
//     virtual inline Type type() const { return INSERT; }
//     CommandString table;
//     std::vector<CommandString> columns;
// };



class SQLParseResult {
private:
	bool success_;

	// on success:
	std::unique_ptr<Command> command_; // TODO: implement

	// on error:
	size_t index_;
	std::string error_;
	

private:
	inline SQLParseResult(std::unique_ptr<Command>&& command): success_(true), command_(std::move(command)) { }
	inline SQLParseResult(const std::string& error, const size_t index): success_(false), error_(error), index_(index) { }

public:
	inline SQLParseResult() { }
	inline static SQLParseResult Success(std::unique_ptr<Command>&& command) { return { std::move(command) }; }
	inline static SQLParseResult Success() { return Success(std::make_unique<CommandNull>()); }
	inline static SQLParseResult Error(const std::string& msg, const size_t index) { return { msg, index }; }

public:
	inline bool success() const { return success_; }

	// on success:
	inline std::unique_ptr<Command>& command() { return command_; }

	// on error:
	inline const std::string& error() const { return error_; }
	inline size_t errorIndex() const { return index_; }
};





class SQLToken {
protected:
	std::string name;
public:
	virtual ~SQLToken(){}
	// virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const = 0; // on error, index is left unchanged
	virtual SQLParseResult parse(const std::string& source, size_t& index) const = 0; // on error, index is left unchanged
	inline SQLToken* setName(const std::string& name) { this->name = name; return this; }
};



class SQLLiteral : public SQLToken {
private:
	std::string literal;

public:
	inline SQLLiteral(const std::string& literal): literal(literal) {}
	inline SQLLiteral(const std::string& literal, const std::string& name): literal(literal) { this->name = name; }
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
		for(size_t i = 0; i < literal.size(); i++) {
			if(index + i >= source.size()) {
				// return false;
				return SQLParseResult::Error("Could not parse Literal: input ended prematurely", index + i);
			}

			if(tolower(literal[i]) != tolower(source[index + i])) {
				// return false;
				return SQLParseResult::Error("Could not parse Literal: input mismatch", index + i);
			}
		}

		index += literal.size();

		// result.command() = std::make_unique<CommandNull>();

		// return true;

		// if Token is named:
		if(name.size() > 0) { 
			auto res = std::make_unique<CommandString>(name);
			res->str = literal;
			return SQLParseResult::Success(std::move(res));
		}

		return SQLParseResult::Success(std::make_unique<CommandNull>(name));
	}
};



class SQLIdentifier : public SQLToken {
	private:
		// std::string *output;
public:
	inline SQLIdentifier() {}
	inline SQLIdentifier(const std::string& name) { this->name = name; }
    // inline void setOutput(std::string& output) {
	// 	this->output = &output;
	// }
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
		constexpr auto validIDChar =
			[](const char c) -> bool {
				return (c >= 'a' && c <= 'z') || c == '_';
			};

		const size_t index_orig = index;

		// *output = ""; // clear output
		std::unique_ptr<CommandString> output(new CommandString);
	

		for(;;) {
			if(index >= source.size() || !validIDChar(tolower(source[index]))) {
				if(index == index_orig) {
					// return false; // an identifier hat to consist of at least 1 character
					return SQLParseResult::Error("Identifier: No valid identifier was found", index); // an identifier hat to consist of at least 1 character
				}

				// result.command() = std::move(output);
				// return true; // Identifier ended; success.

				// if identifier is named:
				if(name.size() > 0)
					output->name = name; // replace name with variant name

				return SQLParseResult::Success(std::move(output));
			}

			// *output += tolower(source[index++]); // write to output
			output->str += tolower(source[index++]); // write to output
		}
	}
};



class SQLSpace : public SQLToken {
private:
	bool optional; // if optional, 0..n spaces can be present. otherwise, there has to be at least one space
public:
	inline SQLSpace(const bool optional = false): optional(optional) {}
	// inline static SQLSpace Optional() { return SQLSpace(true); }
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
		constexpr auto isSpace = 
			[](const char c) -> bool {
				return c == ' ' || c == '\t';
			};

		if(!optional) {
			if(index >= source.size() || !isSpace(source[index]))
				// return false;
				return SQLParseResult::Error("Space: space not found", index);

			index++;
		}

		while(index < source.size() && isSpace(source[index])) {
			index++;
		}

		// result.command() = std::make_unique<CommandNull>();

		// return true;

		return SQLParseResult::Success();
	}
};



class SQLCommand : public SQLToken {
private:
	std::vector<std::unique_ptr<SQLToken>> parts;

public:
	template<typename ... Args>
	inline SQLCommand(Args ... parts) {
		// (this->parts.emplace_back(parts), ...);
		(
			[&]() {
				if constexpr (std::is_same_v<decltype(parts), std::string> == false)
					this->parts.emplace_back(parts);
				else
					this->name = parts;
			}()
		, ...);
	}
	inline SQLCommand(std::vector<std::unique_ptr<SQLToken>> parts):
			parts(std::move(parts)) {}
	inline SQLCommand(std::vector<std::unique_ptr<SQLToken>> parts, const std::string& name):
			parts(std::move(parts)) { this->name = name; }
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
		const size_t index_orig = index;

		std::unique_ptr<CommandList> output = std::make_unique<CommandList>();
		
		for(auto part = parts.begin(); part != parts.end(); ++part) {
			// SQLParseResult subResult;
			// if(!(*part)->parse(subResult, source, index)) {
			auto subResult = (*part)->parse(source, index);
			if(!subResult.success()) {
				index = index_orig;
				// return false;
				return subResult;
			}

			if(subResult.command()->type() != Command::NULL_)
				output->list.emplace_back(std::move(subResult.command()));
		}

		// result.command() = std::move(output);

		// return true;

		output->name = name;
		return SQLParseResult::Success(std::move(output));
	}
};



class SQLVariant : public SQLToken {
private:
	std::vector<std::unique_ptr<SQLToken>> parts;

public:
	template<typename ... Args>
	inline SQLVariant(Args ... parts) {
		// (this->parts.emplace_back(parts), ...);
		(
			[&]() {
				if constexpr (std::is_same_v<decltype(parts), std::string> == false)
					this->parts.emplace_back(parts);
				else
					this->name = parts;
			}()
		, ...);
	}
	inline SQLVariant(std::vector<std::unique_ptr<SQLToken>> parts):
			parts(std::move(parts)) {}
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
		// const size_t index_orig = index;
		for(auto part = parts.begin(); part != parts.end(); ++part) {
			// if((*part)->parse(result, source, index)) {
			auto res = (*part)->parse(source, index);
			if(res.success()) {
				// if variant is named:
				if(name.size() > 0)
					res.command()->name = name;

				return res;
			}
		}

		return SQLParseResult::Error("Variant: Could not find match", index);
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
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
		const size_t index_orig = index;

		// if(token->parse(result, source, index)) {
		auto res = token->parse(source, index);
		if(res.success()) {
			// return true;
			return res;
		}

		index = index_orig; // exit without altering index

		return SQLParseResult::Success();
	}
};



class SQLList : public SQLToken { // consumes following spaces
private:
	std::unique_ptr<SQLIdentifier> token;
	SQLSpace optSpace;
    SQLLiteral comma;

	// std::vector<std::string>* output;

public:
	inline SQLList(SQLIdentifier *const token):
			token(token), optSpace(true), comma(",") {}
	inline SQLList(SQLIdentifier *const token, const std::string& name):
			token(token), optSpace(true), comma(",") { this->name = name; }
	inline SQLList(std::unique_ptr<SQLIdentifier> token):
			token(std::move(token)), optSpace(true), comma(",") {}
	inline SQLList(std::unique_ptr<SQLIdentifier> token, const std::string& name):
			token(std::move(token)), optSpace(true), comma(",") { this->name = name; }

	// inline void setOutput(std::vector<std::string>& output) {
	// 	// this->output = &output;
	// }
	// inline virtual bool parse(SQLParseResult& result, const std::string& source, size_t& index) const {
	inline virtual SQLParseResult parse(const std::string& source, size_t& index) const {
		const size_t index_orig = index;

		std::unique_ptr<CommandList> output(new CommandList);

        // for(size_t num_elements = 0; ; num_elements++) {
        for(;;) {
			// SQLParseResult subResult;

            // if(!token->parse(subResult, source, index)) { // try extracting identifier
            auto res = token->parse(source, index); // try extracting identifier
			if(!res.success()) {
                return res;
            }

			if(res.command()->type() != Command::NULL_)
				output->list.push_back(std::move(res.command()));

            // optSpace.parse(subResult, source, index); // optional space before comma
            optSpace.parse(source, index); // optional space before comma

            if(!(comma.parse(source, index).success())) { // comma
				// result.command() = std::move(output);
                // return true;

				// if list is named:
				if(name.size() > 0)
					output->name = name;

				return SQLParseResult::Success(std::move(output));
			}

            // optSpace.parse(subResult, source, index); // optional space after comma
            optSpace.parse(source, index); // optional space after comma
        }
	}
};