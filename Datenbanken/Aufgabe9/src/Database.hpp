#pragma once

#include "Buch.hpp"

#include <initializer_list>
#include <iostream>
#include <vector>


class Database {
private:
	struct Index {
		char Ordnungsbegriff[41]{};
		size_t Position;
	};

	std::vector<Buch> buchDB;

	std::vector<Index> indexDB;
	size_t keyLength = 41;

public:
	Database();
	~Database();

public:
	void drop(); // Datenbank leeren
	void insert(const Buch& book); // Datensatz einfügen
	void insert(std::initializer_list<Buch> books); // Datensätze einfügen
	void createIndex();
	Buch& findOne(const char *const key);
	void printAllDesc() const;

public:
	friend std::ostream& operator<<(std::ostream& cout, const Index& i);
};