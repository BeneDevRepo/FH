#pragma once

#include "bvector.hpp"
#include "Buch.hpp"
#include "Index.hpp"

#include <initializer_list>
#include <unordered_map>
#include <iostream>
#include <vector>


class Database {
private:
	// std::string indexed_column;
	// struct Index {
	// 	char Ordnungsbegriff[41]{};
	// 	size_t Position;
	// };

	// std::vector<Buch> buchDB;
	bvector<Buch> buchDB;

	// std::vector<Index> indexDB;
	// bvector<Index> indexDB;
	// Index indexDB;
	std::unordered_map<std::string, Index> indices;

public:
	Database();
	~Database();

public:
	void drop(); // Datenbank leeren
	void insert(const Buch& book); // Datensatz einfügen
	void insert(std::initializer_list<Buch> books); // Datensätze einfügen
	const Buch& getRow(const size_t position) const; // get row by "physical" location

	void createIndex(std::string column);
	bool hasIndex(const std::string& column) const; // returns if specified column is indexed
	const Index& getIndex(const std::string& column) const; // returns index for specified column

	// const Buch& findOne(const char *const key) const;
	const Buch& findOne(const std::string& column, const std::string& key) const; // binary search over sorted index

public:
	class BookIterator {
		const Database* db;
		const Buch* it;
	public:
		inline BookIterator(const Database* db, const Buch* it): db(db), it(it) { }
		inline BookIterator operator++() { ++it; return *this; }
		inline BookIterator operator++(int) { BookIterator tmp(*this); ++(*this); return tmp; }
		inline bool operator==(const BookIterator& other) const { return it == other.it; }
		inline bool operator!=(const BookIterator& other) const { return it != other.it; }
		inline const Buch& operator*() const { return *it; }
	};

	inline BookIterator begin() const { return BookIterator(this, buchDB.data()); }
	inline BookIterator end() const { return BookIterator(this, buchDB.data() + buchDB.size()); }

	// class BookIterator {
	// 	Database* db;
	// 	Index* it;
	// public:
	// 	inline BookIterator(Database* db, Index* it): db(db), it(it) { }
	// 	inline BookIterator operator++() { ++it; return *this; }
	// 	inline BookIterator operator++(int) { BookIterator tmp(*this); ++(*this); return tmp; }
	// 	inline bool operator==(const BookIterator& other) const { return it == other.it; }
	// 	inline bool operator!=(const BookIterator& other) const { return it != other.it; }
	// 	inline Buch& operator*() { return db->buchDB[it->Position]; }
	// };

	// inline BookIterator begin() { return BookIterator(this, indexDB.data()); }
	// inline BookIterator end() { return BookIterator(this, indexDB.data() + indexDB.size()); }

public:
	// friend std::ostream& operator<<(std::ostream& cout, const Index& i);
};