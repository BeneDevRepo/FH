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
	} else
		return; // index without book does not make sense


	std::ifstream indexFile("Index.db", std::ios::binary | std::ios::ate);

	if(indexFile.is_open()) {
		size_t indexSize = indexFile.tellg();
		indexFile.seekg(std::ios::beg);

		if(indexSize==0)
			throw std::runtime_error("Invalid Index DB File");

		for(;;) {
			const char c = indexFile.get();

			indexSize--;

			if(c == ';') break; // done

			if(indexSize==0)
				throw std::runtime_error("Invalid Index DB File (file ended unexpectedly)");

			indexed_column += c;
		}

		indexDB.resize(indexSize / sizeof(Index));
		indexFile.read((char*)indexDB.data(), indexSize);

		std::cout << "Loaded " << indexDB.size() << " Indices.\n";
		std::cout << "Indexed Column: " << indexed_column << "\n";
	}
}


Database::~Database() {
	std::ofstream buchFile("Buch.db", std::ios::binary);
	buchFile.write((char*)buchDB.data(), buchDB.size() * sizeof(Buch));

	if(indexed_column.size() > 0) {
		std::ofstream indexFile("Index.db", std::ios::binary);
		indexFile.write(indexed_column.data(), indexed_column.size());
		indexFile.put(';');
		indexFile.write((char*)indexDB.data(), indexDB.size() * sizeof(Index));
	}
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
	if(indexed_column.size() != 0) {
		Index newInd;
		// strncpy(newInd.Ordnungsbegriff, book.Titel, keyLength);
		strncpy(newInd.Ordnungsbegriff, book.Titel, Buch::columns.at(indexed_column).size);
		newInd.Position = buchDB.size();
		indexDB.push_back(newInd);
	}

	buchDB.push_back(book);
}

void Database::insert(std::initializer_list<Buch> books) {
	for(const Buch& book : books)
		insert(book);
}


void Database::createIndex(const std::string& column) {
	indexed_column = toLowerCase(column);
	const auto column_meta = Buch::columns.at(indexed_column);

	if(indexDB.size() != buchDB.size())
		indexDB.resize(buchDB.size());

	for(size_t i = 0; i < buchDB.size(); i++) {
		// strncpy(indexDB[i].Ordnungsbegriff, buchDB[i].Titel, keyLength);
		strncpy(indexDB[i].Ordnungsbegriff, ((char*)(buchDB.data() + i)) + column_meta.offset, column_meta.size);
		indexDB[i].Position = i;
	}

	std::sort(
		indexDB.begin(),
		indexDB.end(),
		[&](const Index& a, const Index& b) -> bool {
			// return strncmp(a.Ordnungsbegriff, b.Ordnungsbegriff, keyLength) < 0;
			return strncmp(a.Ordnungsbegriff, b.Ordnungsbegriff, column_meta.size) < 0;
		}
	);
}

Buch& Database::findOne(const char *const key) {
	size_t min = 0; // minimum index (inclusive)
	size_t max = indexDB.size(); // maximum index (exclusive)

	for(;;) {
		const size_t test_i = (max + min) / 2;
		const Index& cur = indexDB[test_i];
		// const int comp = strncmp(key, cur.Ordnungsbegriff, keyLength);
		const int comp = strncmp(key, cur.Ordnungsbegriff, Buch::columns.at(indexed_column).size);

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

// void Database::printAllDesc() const {
// 	for(size_t i = indexDB.size(); i > 0; i--)
// 		std::cout << buchDB[indexDB[i - 1].Position] << "\n";
// }


std::ostream& operator<<(std::ostream& cout, const Database::Index& i) {
	cout << "Position: " << i.Position << ", " << " Key: " << std::string_view(i.Ordnungsbegriff, strnlen(i.Ordnungsbegriff, 41));
	return cout;
}