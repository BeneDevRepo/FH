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

	inline bool empty() const {
		return position == -1;
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
static constexpr size_t NUM_BUCKETS = 36;
private:
	std::string column;

	std::array<IndexBucket, NUM_BUCKETS> buckets;
	// size_t numOccupied; // number of elements in hash table

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
		// numOccupied++;
	}

	inline size_t find(const std::string& key) const { // TODO: implement
		throw std::runtime_error("wrong index type");
	}

	inline size_t find(const size_t key) const { // TODO: implement
		if(column.size() == 0)
			throw std::runtime_error("Error: tried using empty(invalid) Index");

		size_t index = hash(key) % buckets.size();

		while(!buckets[index].hasKey(key) && buckets[index].full())
			index = (index + 1) % buckets.size();

		// if(!buckets[index].hasKey(key))
		// 	return -1;

		return buckets[index].get(key).pos();
	}

	inline const IndexEntry* begin() const {
		// return index.begin().base();
		// return index.begin();
		return nullptr;
	}

	inline const IndexEntry* end() const {
		// return index.end().base();
		// return index.end();
		return nullptr;
	}
};