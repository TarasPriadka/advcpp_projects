#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "Structs.h"

using namespace std;

class Hashtable {
private:
	int size;
	vector<shared_ptr<Product>> hashTable;
	unsigned long hash(string str_);

public:
	Hashtable();
	Hashtable(vector<shared_ptr<Product>> products);
	int getSize() { return size; }
	shared_ptr<Product> get(string barcode);
};

Hashtable::Hashtable() {}

Hashtable::Hashtable(vector<shared_ptr<Product>> products) {
	hashTable.resize((int)(products.size() * 5));
	int collisions = 0;
	for (auto prod : products) {
		auto hash = this->hash(prod->barcode);
		unsigned int loc = hash % hashTable.size();
		if (hashTable[loc]) {
			bool duplicate = false;
			while (!duplicate && hashTable[loc]) {
				if (hashTable[loc]->barcode == prod->barcode)
					duplicate = true;
				loc++;// keep going until hit an unoccupied spot
			}
			if (duplicate)
				continue;
		}
		hashTable[loc] = move(prod);
		this->size++;
	}
}

shared_ptr<Product> Hashtable::get(string barcode) {
	auto hash = this->hash(barcode);
	unsigned int loc = hash % hashTable.size();
	int missedEntries = 0;
	while (hashTable[loc]->barcode != barcode) {
		loc++;
		if (missedEntries++ > 6)
			return NULL;
	}
	return hashTable[loc];
}

//30 collission for about 1400 elements in the hashtable
unsigned long Hashtable::hash(string str_) {
	unsigned long hash = 5381;
	char* str = const_cast<char*>(str_.c_str());
	int c;
	while (c = *str++)
		hash = (((hash << 5) + hash) + c) ^ c; // ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}