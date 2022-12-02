#include "DbCLI.hpp"

#include "SQLParser.hpp"
#include "SQLExecutor.hpp"

#include "util.hpp"


void DbCLI::run() {
	std::cout << "BeneSQL CLI [version 1.0.0]\n";
	std::cout << "type HELP; for help\n\n";


	// lambda to read in a command line by line until a terminating semicolon is found:
	const auto getCmd =
		[]() -> std::string {
			static std::string input; // input persists across function calls, to ensure no input is lost
	
			for(size_t line = 0; ; line++) {
				// try finding command:
				for(size_t i = 0; i < input.size(); i++) {
					if(input[i] == ';') {
						const std::string& command = input.substr(0, i + 1); // extract command (including ;)
						input = input.substr(i + 1, input.size() - (i + 1));
						return command;
					}
				}

				// if no command available, read in new line:
				std::cout << (line == 0 ? ">> " : " > ");
				input += " " + readLine();
			}
		};


	SQLParser parser;
	for(bool stop = false; !stop; ) {
		const std::string commandStr = getCmd();
		// std::cout << "Command: " << commandStr << "\n";

		SQLParseResult result = parser.parse(commandStr, 0);
		const Command& command = *result.command();
		// const std::string& commandType = command.name;
	
		if(result.success()) {
			// std::cout << "<< " << "Success" << "\n";

			try {
				// Command is valid: execute:
				SQLExecutor::execute(db, command, stop);
			} catch(const std::exception& e) {
				std::cout << "<< Execution Error: " << e.what() << "\n";
			}
		} else {
			// Command is invalid: ERROR:
			std::cout << "<< Syntax Error" << "\n";
		}
	}

	std::cout << "Shutting down...\n";
}