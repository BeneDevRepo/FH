#pragma once


#include "util.hpp"

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <cstring>
#include <string>


struct Buch {
	char Autor[21];
	char Titel[41];
	char Verlagsname[21];
	int Erscheinungsjahr;
	char Erscheinungsort[41];
	char ISBN[15];

	Buch();
	Buch(
		const char *const Autor,
		const char *const Titel,
		const char *const Verlagsname,
		const int Erscheinungsjahr,
		const char *const Erscheinungsort,
		const char *const ISBN);

public:
	struct Column {
		std::string name;
		enum { STRING, INTEGER } type;
		size_t offset;
		size_t size;
	};

private:
	static const std::unordered_map<std::string, Column> columns;

public:
	inline static bool hasColumn(std::string columnName) {
		columnName = toLowerCase(columnName);
		return columns.find(columnName) != columns.end();
	}

	inline static const Column& columnInfo(std::string columnName) {
		columnName = toLowerCase(columnName);
		return columns.at(columnName);
	}

	inline const std::string_view getString(std::string columnName) const {
		columnName = toLowerCase(columnName);
		if(columns.at(columnName).type != Column::STRING)
			throw std::runtime_error("Error: tried to get string contents of non-string column");

		const char *const cstr = ((char*)this) + columns.at(columnName).offset; // extract start of column

		return std::string_view (cstr, strnlen(cstr, columns.at(columnName).size)); // create length-limited string_view for fields without null-terminator
	}

	inline int getInt(std::string columnName) const {
		columnName = toLowerCase(columnName);

		if(columns.at(columnName).type != Column::INTEGER)
			throw std::runtime_error("Error: tried to get int contents of non-int column");

		const char *const start = ((char*)this) + columns.at(columnName).offset; // extract start of column

		return *(int*)start;
	}
};


std::ostream& operator<<(std::ostream& cout, const Buch& b);