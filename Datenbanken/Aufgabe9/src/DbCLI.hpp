#pragma once

#include "Database.hpp"

#include <string>


class DbCLI {
	enum class COMMAND_STATE: uint8_t {
		OK,
		ERROR
	};

private:
	Database db;

public:
	inline DbCLI(): db() {}

	void run();

private:
	COMMAND_STATE checkCMD(const std::string& input) const;
};