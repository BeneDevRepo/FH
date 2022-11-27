#pragma once

#include "Buch.hpp"

#include <initializer_list>
#include <iostream>
#include <vector>


class Database {
private:
	std::string indexed_column;
	struct Index {
		char Ordnungsbegriff[41]{};
		size_t Position;
	};

	std::vector<Buch> buchDB;

	std::vector<Index> indexDB;
	// size_t keyLength = 41;

public:
	Database();
	~Database();

public:
	void drop(); // Datenbank leeren
	void insert(const Buch& book); // Datensatz einfügen
	void insert(std::initializer_list<Buch> books); // Datensätze einfügen
	void createIndex(const std::string& column);
	Buch& findOne(const char *const key);
	// void printAllDesc() const;

public:
	class BookIterator {
		Database* db;
		Index* it;
	public:
		inline BookIterator(Database* db, Index* it): db(db), it(it) { }
		inline BookIterator operator++() { ++it; return *this; }
		inline BookIterator operator++(int) { BookIterator tmp(*this); ++(*this); return tmp; }
		inline bool operator==(const BookIterator& other) const { return it == other.it; }
		inline bool operator!=(const BookIterator& other) const { return it != other.it; }
		inline Buch& operator*() { return db->buchDB[it->Position]; }
	};

	inline BookIterator begin() { return BookIterator(this, indexDB.data()); }
	inline BookIterator end() { return BookIterator(this, indexDB.data() + indexDB.size()); }

public:
	friend std::ostream& operator<<(std::ostream& cout, const Index& i);
};