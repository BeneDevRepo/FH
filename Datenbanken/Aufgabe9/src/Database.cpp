#include "Database.hpp"

#include <string_view>
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
		const size_t indexSize = indexFile.tellg();
		indexDB.resize(indexSize / sizeof(Index));
		indexFile.seekg(std::ios::beg);
		indexFile.read((char*)indexDB.data(), indexSize);
	}
}


Database::~Database() {
	std::ofstream buchFile("Buch.db", std::ios::binary);
	buchFile.write((char*)buchDB.data(), buchDB.size() * sizeof(Buch));

	std::ofstream indexFile("Index.db", std::ios::binary);
	indexFile.write((char*)indexDB.data(), indexDB.size() * sizeof(Index));
}


void Database::drop() {
	// Arrays leeren:
	buchDB.clear();
	indexDB.clear();

	// Dateien löschen:
	remove("Buch.db");
	remove("Index.db");
}


void Database::insert(const Buch& book) {
	buchDB.push_back(book);
}

void Database::insert(std::initializer_list<Buch> books) {
	for(const Buch& book : books)
		insert(book);
}


void Database::createIndex() {
	if(indexDB.size() != buchDB.size())
		indexDB.resize(buchDB.size());

	for(size_t i = 0; i < buchDB.size(); i++) {
		strncpy(indexDB[i].Ordnungsbegriff, buchDB[i].Titel, keyLength);
		indexDB[i].Position = i;
	}

	std::sort(
		indexDB.begin(),
		indexDB.end(),
		[&](const Index& a, const Index& b) -> bool {
			return strncmp(a.Ordnungsbegriff, b.Ordnungsbegriff, keyLength) < 0;
		}
	);
}

Buch& Database::findOne(const char *const key) {
	size_t min = 0; // minimum index (inclusive)
	size_t max = indexDB.size(); // maximum index (exclusive)

	for(;;) {
		const size_t test_i = (max + min) / 2;
		const Index& cur = indexDB[test_i];
		const int comp = strncmp(key, cur.Ordnungsbegriff, keyLength);

		if(min >= max) // no index left to test
			throw std::runtime_error("ERROR: findBook(): key not present in Database!");

		if(comp == 0)
			return buchDB[cur.Position];

		if(comp < 0)
			max = test_i;
		else
			min = test_i + 1;
	}
}

void Database::printAllDesc() const {
	for(size_t i = indexDB.size(); i > 0; i--)
		std::cout << buchDB[indexDB[i - 1].Position] << "\n";
}


std::ostream& operator<<(std::ostream& cout, const Database::Index& i) {
	cout << "Position: " << i.Position << ", " << " Key: " << std::string_view(i.Ordnungsbegriff, strnlen(i.Ordnungsbegriff, 41));
	return cout;
}