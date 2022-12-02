#include "Database.hpp"

#include "util.hpp"

#include <string_view>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <cstring>


Database::Database() {
	// load Database:
	std::ifstream buchFile("Buch.db", std::ios::binary | std::ios::ate);

	if(buchFile.is_open()) {
		const size_t buchSize = buchFile.tellg();
		buchDB.resize(buchSize / sizeof(Buch));
		buchFile.seekg(std::ios::beg);
		buchFile.read((char*)buchDB.data(), buchSize);
	}


	// create indices:
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

	// std::cout << "Loaded " << indices.size() << " Indices:\n";
	// for(const auto& index : indices)
	// 	std::cout << " - Index on " << index.first << "\n";
}


Database::~Database() {
	// Store Database:
	std::ofstream buchFile("Buch.db", std::ios::binary);

	if(buchFile.is_open())
		buchFile.write((char*)buchDB.data(), buchDB.size() * sizeof(Buch));

	if(indices.size() == 0) return;

	// Indexed columns:
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
	indices.clear();

	// Dateien löschen:
	remove("Buch.db");
	remove("Index.db");
}


void Database::insert(const Buch& book) {
	for(auto& index : indices)
		index.second.insert(book, buchDB.size());

	buchDB.push_back(book);
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

	if(indices.find(column) != indices.end())
		throw std::runtime_error("Could not create index: index already present");

	indices.insert({column, Index(column)});

	for(size_t i = 0; i < buchDB.size(); i++)
		indices[column].insert(buchDB[i], i);
}

// const Buch& Database::findOne(const char *const key) const {
const Buch& Database::findOne(const std::string& column, const std::string& key) const {
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
