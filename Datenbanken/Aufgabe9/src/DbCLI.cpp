#include "DbCLI.hpp"

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
	struct Token {
		enum Type {
			LITERAL,
			NAME
		};
		Type type;
		std::string _content;
		static inline Token Literal(const std::string& literal) { return { .type = LITERAL, ._content = literal }; }
		static inline Token Name() { return { .type = NAME }; }
		inline void match(const std::string& command, size_t index) {
			switch(type) {
				case LITERAL:
					break;
				case NAME:
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