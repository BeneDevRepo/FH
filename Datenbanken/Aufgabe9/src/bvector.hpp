#pragma once

#include <stdexcept>
#include <algorithm>
#include <cstring>


template<typename T>
class bvector {
private:
	size_t capacity;
	size_t count;
	T* content;

	static constexpr size_t MIN_CAPACITY = 2;

public:
	inline bvector():
			capacity(MIN_CAPACITY),
			count(0),
			content(new T[MIN_CAPACITY]) {}
	inline bvector(const size_t size):
			bvector() {
		resize(size);
	}
	inline ~bvector() {
		delete[] content;
	}
	inline bvector(const bvector& other):
			capacity(other.capacity),
			count(other.count),
			content(new T[other.capacity]) {
		memcpy(content, other.content, count * sizeof(T));
	}
	inline bvector& operator=(const bvector& other) {
		if(this == &other) return *this; // prevent self-assignment

		capacity = other.capacity;
		count = other.count;
		content = new T[other.capacity];
		memcpy(content, other.content, count * sizeof(T));

		return *this;
	}

public:
	inline size_t size() const {
		return count;
	}

	inline void push_back(const T& element) {
		resize(count + 1);
		content[size() - 1] = element;
	}

	inline const T& operator[](const size_t index) const {
		if(index >= size())
			throw std::runtime_error("bvector: index out of bounds");
		return content[index];
	}

	inline T& operator[](const size_t index) {
		if(index >= size())
			throw std::runtime_error("bvector: index out of bounds");
		return content[index];
	}

	inline const T* data() const {
		return content;
	}

	inline T* data() {
		return content;
	}

	inline void clear() {
		// reset capacity to default:
		if(capacity != MIN_CAPACITY) {
			delete[] content;
			content = new T[MIN_CAPACITY];
			capacity = MIN_CAPACITY;
		}
		// reset count:
		count = 0;
	}

	inline void resize(const size_t newSize) {
		// in case of 0 reset vector:
		if(newSize == 0) {
			clear();
			return; // exit function
		}

		// else adjust capacity:
		if(newSize > capacity || newSize < capacity / 2) {
			size_t newCapacity = capacity;
			// adjust capacity upwards:
			while(newSize > newCapacity)
				newCapacity *= 2;
			// adjust capacity downwards:
			while(newSize < newCapacity / 2 && capacity > MIN_CAPACITY)
				newCapacity /= 2;

			// swap buffers:
			T *newContent = new T[newCapacity];
			for(size_t i = 0; i < std::min<size_t>(count, newSize); i++)
				newContent[i] = std::move(content[i]);
			// memcpy(newContent, content, std::min<size_t>(count, newSize) * sizeof(T));
			delete[] content;
			content = newContent;
			capacity = newCapacity;
		}

		count = newSize;
	}

	inline const T* begin() const {
		return content;
	}

	inline const T* end() const {
		return content + count;
	}

	inline T* begin() {
		return content;
	}

	inline T* end() {
		return content + count;
	}
};