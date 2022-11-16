#pragma once

#include "Database.hpp"

#include <string>


class DbCLI {
	enum class COMMAND_STATE: uint8_t {
		OK,
		WAITING,
		ERROR
	};

private:
	Database db;

public:
	DbCLI();
	~DbCLI();

	void run();

private:
	COMMAND_STATE checkCMD(const std::string& input) const;
};