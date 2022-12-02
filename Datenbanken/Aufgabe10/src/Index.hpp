#pragma once


#include "Buch.hpp"

#include "util.hpp"
// #include "bvector.hpp"
// #include <vector>
#include <string>
#include <array>



class IndexEntry {
private:
	// char Ordnungsbegriff[41]{};
	// std::string ordnungsbegriff;
	size_t ordnungsbegriff;
	size_t position;

public:
	inline IndexEntry(): position(-1) {}
	// inline IndexEntry(const std::string& ordnungsbegriff, const size_t position):
	// 	ordnungsbegriff(ordnungsbegriff), position(position) {}
	inline IndexEntry(const size_t ordnungsbegriff, const size_t position):
		ordnungsbegriff(ordnungsbegriff), position(position) {}
	// const std::string& value() const {
	// 	return ordnungsbegriff;
	// }
	const size_t value() const {
		return ordnungsbegriff;
	}
	const size_t pos() const {
		return position;
	}
	inline bool used() const {
		return position != -1;
	}
};


class IndexBucket {
private:
	std::array<IndexEntry, 5> indices;

public:
	inline bool full() const {
		for(const IndexEntry& index : indices)
			if(!index.used())
				return false;
		return true;
	}
	inline void insert(const size_t ordnungsbegriff, const size_t position) {
		size_t i = 0;
		for(; indices[i].used(); i++); // skip used entries
		indices[i] = IndexEntry(ordnungsbegriff, position);
	}
	inline bool hasKey(const size_t key) const {
		for(const IndexEntry& entry : indices)
			if(entry.value() == key)
				return true;
		return false;
	}
	inline const IndexEntry& get(const size_t key) const {
		for(const IndexEntry& entry : indices)
			if(entry.value() == key)
				return entry;
		throw std::runtime_error("Error: Index not present in bucket");
	}
};


class Index {
static constexpr size_t NUM_BUCKETS = 10000;
private:
	std::string column;
	// std::vector<IndexEntry> index;
	std::array<IndexBucket, NUM_BUCKETS> buckets;

	static size_t hash(size_t value) {
		size_t res = 0;
		for(size_t i = 0; i < 4; i++) {
			res += value % 10;
			value /= 10;
		}
		return res;
	};

public:
	inline Index() {}
	inline Index(const std::string& column):
			column(column) {
		if(!Buch::hasColumn(column))
			throw std::runtime_error("Error: Index: can't create index for invalid column");
		// if(Buch::columnInfo(column).type != Buch::Column::STRING)
		// 	throw std::runtime_error("Error: Index: can't create index for non-string column");
		if(Buch::columnInfo(column).type != Buch::Column::INTEGER)
			throw std::runtime_error("Error: Index: can't create index for non-string column");
	}

	inline const std::string& targetColumn() const {
		return column;
	}

	inline void insert(const Buch& row, const size_t position) {
		if(column.size() == 0)
			throw std::runtime_error("Error: tried using empty(invalid) Index");

		size_t index = hash(row.getInt(column)) % buckets.size();

		while(buckets[index].full())
			index = (index + 1) % buckets.size();
		
		buckets[index].insert(row.getInt(column), position);

		// index.push_back(IndexEntry((std::string)row.getString(column), position));

		// std::sort(
		// 	buckets.begin(),
		// 	buckets.end(),
		// 	[](const IndexEntry& a, const IndexEntry& b){
		// 		return a.value() < b.value();
		// 	}
		// );
	}

	inline size_t find(const std::string& key) const { // TODO: implement
		throw std::runtime_error("wrong index type");
	}

	inline size_t find(const size_t key) const { // TODO: implement
		// for(const IndexEntry& ind : buckets)
		// 	if(ind.value() == key)
		// 		return ind.pos();

		size_t index = hash(key) % buckets.size();

		while(!buckets[index].hasKey(key))
			index = (index + 1) % buckets.size();
		
		return buckets[index].get(key).pos();

		// throw std::runtime_error("ERROR: findBook(): key \"" + std::to_string(key) + "\" not present in Database!");

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
		// return index.begin();
	}

	inline const IndexEntry* end() const {
		// return index.end().base();
		// return index.end();
	}
};