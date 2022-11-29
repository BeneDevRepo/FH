#include "Database.hpp"

#include "util.hpp"

#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <cstring>


Database::Database() {
	std::ifstream buchFile("Buch.db", std::ios::binary | std::ios::ate);

	if(buchFile.is_open()) {
		const size_t buchSize = buchFile.tellg();
		buchDB.resize(buchSize / sizeof(Buch));
		buchFile.seekg(std::ios::beg);
		buchFile.read((char*)buchDB.data(), buchSize);
	}


	std::ifstream indexFile("Index.db", std::ios::binary | std::ios::ate);

	if(indexFile.is_open()) {
		size_t size = indexFile.tellg();
		indexFile.seekg(std::ios::beg);

		std::string column;
		while(size > 0) {
			const char c = indexFile.get(); --size;

			if(c == ';') {
				createIndex(column);
				column.clear();
			} else {
				column += c;
			}
		}
	}
	
	// createIndex("Autor");

	std::cout << "Loaded " << indices.size() << " Indices:\n";
	for(const auto& index : indices)
		std::cout << " - Index on " << index.first << "\n";
}


Database::~Database() {
	std::ofstream buchFile("Buch.db", std::ios::binary);

	if(buchFile.is_open())
		buchFile.write((char*)buchDB.data(), buchDB.size() * sizeof(Buch));

	if(indices.size() == 0) return;

	std::ofstream indexFile("Index.db", std::ios::binary);

	if(indexFile.is_open()) {
		for(const auto& index : indices) {
			indexFile.write(index.first.data(), index.first.size());
			indexFile.put(';');
		}
	}
}


void Database::drop() {
	// Arrays leeren:
	buchDB.clear();
	// indexDB.clear();

	// Dateien löschen:
	remove("Buch.db");
	remove("Index.db");
}


void Database::insert(const Buch& book) {
	// if(indexed_column.size() != 0) {
	// 	// Index newInd;
	// 	// // strncpy(newInd.Ordnungsbegriff, book.Titel, keyLength);
	// 	// strncpy(newInd.Ordnungsbegriff, book.Titel, Buch::columns.at(indexed_column).size);
	// 	// newInd.Position = buchDB.size();
	// 	// indexDB.push_back(newInd);

	// 	indexDB.insert(book, buchDB.size());
	// }

	for(auto& index : indices)
		index.second.insert(book, buchDB.size());


	buchDB.push_back(book);

	// debuging...
	// std::cout << "Index states:\n";
	// for(const auto& index : indices) {
	// 	std::cout << "Index on " << index.first << ":\n";
	// 	for(const auto& entry : index.second) {
	// 		std::cout << "  Entry: " << entry.value() << " At pos: " << entry.pos() << "\n";
	// 	}
	// }
}

void Database::insert(std::initializer_list<Buch> books) {
	for(const Buch& book : books)
		insert(book);
}

const Buch& Database::getRow(const size_t position) const {
	if(position >= buchDB.size())
		throw std::runtime_error("Error: Database::getRow(): index out of bounds");

	return buchDB[position];
}


void Database::createIndex(std::string column) {
	column = toLowerCase(column);

	indices.insert({column, Index(column)});

	for(size_t i = 0; i < buchDB.size(); i++)
		indices[column].insert(buchDB[i], i);
}

// const Buch& Database::findOne(const char *const key) const {
const Buch& Database::findOne(const std::string& column, const std::string& key) const {
	// size_t min = 0; // minimum index (inclusive)
	// size_t max = indexDB.size(); // maximum index (exclusive)

	// for(;;) {
	// 	const size_t test_i = (max + min) / 2;
	// 	const Index& cur = indexDB[test_i];
	// 	// const int comp = strncmp(key, cur.Ordnungsbegriff, keyLength);
	// 	const int comp = strncmp(key, cur.Ordnungsbegriff, Buch::columns.at(indexed_column).size);

	// 	if(min >= max) // no index left to test
	// 		throw std::runtime_error("ERROR: findBook(): key not present in Database!");

	// 	if(comp == 0)
	// 		return buchDB[cur.Position];

	// 	if(comp < 0)
	// 		max = test_i;
	// 	else
	// 		min = test_i + 1;
	// }

	// const size_t ind = indexDB.find(key);

	if(indices.find(column) == indices.end())
		throw std::runtime_error("Could not find row where " + column + "=" + key + ": column is not indexed");
	
	const size_t ind = indices.at(column).find(key);

	if(ind < 0)
		// throw std::runtime_error("Could not find row with " + indexDB.targetColumn() + "=" + key);
		throw std::runtime_error("Could not find row where " + column + "=" + key + ": key not found");

	return buchDB[ind];
}

bool Database::hasIndex(const std::string& column) const {
	// return indexDB.targetColumn() == toLowerCase(column);
	return indices.find(toLowerCase(column)) != indices.end();
}

const Index& Database::getIndex(const std::string& column) const {
	if(indices.find(column) == indices.end())
		throw std::runtime_error("Error: getIndex(): Column <" + column + "> is not indexed");

	// return indexDB;
	return indices.at(column);
}
