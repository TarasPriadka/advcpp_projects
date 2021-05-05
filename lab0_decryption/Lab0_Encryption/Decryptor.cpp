#pragma once

#include <string>
#include <vector>
#include "Dictionary.cpp"

using namespace std;

class Decryptor {
public:
	string decrypt(vector<int>::iterator begin, vector<int>::iterator end) {
		// create a dictionary and initialize it
		int dictSize = 256;
		Dictionary dictionary(dictSize);
		for (int i = 0; i < dictSize; i++) {
			dictionary[i].key = i;
			dictionary[i].value = string(1, i);
		}
		// setup some local variables
		string word(1, *begin++);
		string result = word;
		string entry;
		// loop through each character in the iterator
		while (begin != end) {
			int key = *begin;
			if (key >= 0 && key < dictSize)
				entry = dictionary[key].value;  // found it
			else if (key == dictSize)
				entry = word + word[0];  // create new entry
			else
				throw "Invalid key";
			result += entry;
			dictionary[dictSize].value = word + entry[0] ; // assumes vector implementation
			dictSize++;
			word = entry;
			begin++;
		}
		return result;
	}
};