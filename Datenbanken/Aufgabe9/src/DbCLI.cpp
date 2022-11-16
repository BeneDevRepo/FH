#include "DbCLI.hpp"

#include "Parser.hpp"

#include "util.hpp"


DbCLI::DbCLI():
	db() {
}

DbCLI::~DbCLI() {
}

void DbCLI::run() {
	std::cout << "BeneSQL CLI [version 1.0.0]\n\n";

	std::string input;

	for(bool stop = false; !stop; ) {
		std::cout << " > ";
		input += readLine();

		// std::cout << " << " << input << "\n";
		COMMAND_STATE state = checkCMD(input);
		switch(state) {
			case COMMAND_STATE::OK:
				// Command is valid: execute:
				std::cout << " <<< " << "Success" << "\n";
				break;

			case COMMAND_STATE::WAITING:
				// Command is neither finished nor wrong: ignore
				break;
			case COMMAND_STATE::ERROR:
				// Command is invalid: ERROR:
				std::cout << " <<< " << "ERROR" << "\n";
				break;
		}
	}

	std::cout << "Shutting down...\n";
}

DbCLI::COMMAND_STATE DbCLI::checkCMD(const std::string& input) const {
	struct MatchResult {
		bool success;
		std::string error;
		size_t index; // problematic index
		inline MatchResult():
			success(true) {}
		inline MatchResult(const size_t index, const std::string& error):
			success(false), error(error), index(index) { }
	};

	struct Token {
		enum Type {
			LITERAL,
			NAME
		};
		Type type;
		std::string _content;
		static inline Token Literal(const std::string& literal) { return { .type = LITERAL, ._content = literal }; }
		static inline Token Name() { return { .type = NAME }; }
		inline MatchResult test(const std::string& command, size_t index) {
			constexpr auto isLetter = [](const char c){ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); };
			constexpr auto isNumber = [](const char c){ return c >= '0' && c <= '9'; };
			switch(type) {
				case LITERAL:
					for(size_t i = 0; i < _content.size(); i++)
						if(command [index + i] != _content[i])
							return MatchResult(index + i, "Substitution failed: missmatched literal");
					return MatchResult();

				case NAME:
					for(size_t i = 0; isLetter(command[index + i]) || (i > 0 && isNumber(index + i)); i++)
						;
					break;
			}
		}
	};

	static const std::string SELECT = "SELECT (column+:column|*) FROM Buch";
	size_t index;
	for(;;) {
		const char c = input.at(index++);
	}
	return COMMAND_STATE::WAITING;
}