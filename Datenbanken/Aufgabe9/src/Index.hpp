#pragma once


#include "Buch.hpp"

#include "util.hpp"
#include "bvector.hpp"
// #include <vector>
#include <string>



class IndexEntry {
private:
	// char Ordnungsbegriff[41]{};
	std::string ordnungsbegriff;
	size_t position;

public:
	inline IndexEntry() {}
	inline IndexEntry(const std::string& ordnungsbegriff, const size_t position):
		ordnungsbegriff(ordnungsbegriff), position(position) {}
	const std::string& value() const {
		return ordnungsbegriff;
	}
	size_t pos() const {
		return position;
	}
};


class Index {
private:
	std::string column;
	// std::vector<IndexEntry> index;
	bvector<IndexEntry> index;

public:
	inline Index() {}
	inline Index(const std::string& column):
			column(column) {
		if(!Buch::hasColumn(column))
			throw std::runtime_error("Error: Index: can't create index for invalid column");
		if(Buch::columnInfo(column).type != Buch::Column::STRING)
			throw std::runtime_error("Error: Index: can't create index for non-string column");
	}

	inline const std::string& targetColumn() const {
		return column;
	}

	inline void insert(const Buch& row, const size_t position) {
		if(column.size() == 0)
			throw std::runtime_error("Error: tried using empty(invalid) Index");

		index.push_back(IndexEntry((std::string)row.getString(column), position));

		std::sort(
			index.begin(),
			index.end(),
			[](const IndexEntry& a, const IndexEntry& b){
				return a.value() < b.value();
			}
		);
	}

	inline size_t find(const std::string& key) const { // TODO: implement
		for(const IndexEntry& ind : index)
			if(ind.value() == key)
				return ind.pos();

		throw std::runtime_error("ERROR: findBook(): key \"" + key + "\" not present in Database!");

		// if(column.size() == 0)
		// 	throw std::runtime_error("Error: tried using empty(invalid) Index");
		// size_t min = 0; // minimum index (inclusive)
		// size_t max = index.size() - 1; // maximum index (inclusive)

		// for(;;) {

		// 	const size_t test_i = (max + min) / 2;
		// 	const IndexEntry& cur = index[test_i];

		// 	// const int comp = strncmp(key, cur.value(), Buch::columns.at(indexed_column).size);
		// 	// const int comp = strncmp(key.c_str(), cur.value().c_str(), Buch::columnInfo(this->column).size);

		// 	if(cur.value() == key)
		// 		return cur.pos();
		// 		// return buchDB[cur.Position];

		// 	if(min >= max) // no index left to test
		// 		// return -1;
		// 		throw std::runtime_error("ERROR: findBook(): key \"" + key + "\" not present in Database!");

		// 	if(cur.value() < key)
		// 		max = test_i;
		// 	else
		// 		min = test_i + 1;
		// }
	}

	inline const IndexEntry* begin() const {
		// return index.begin().base();
		return index.begin();
	}

	inline const IndexEntry* end() const {
		// return index.end().base();
		return index.end();
	}
};