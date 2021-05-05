#pragma once
#include "KeyValue.cpp"
#include <vector>

class Dictionary {
private:
	vector<KeyValue> dict;
	int dict_size;
public:
	Dictionary(int d) {
		dict_size = d;
		dict.resize(dict_size);
	};

	KeyValue& operator [](int key) {
		if (key == dict_size) {
			dict_size = key + 1;
			dict.push_back(KeyValue(key,""));
			
		}
		return dict[key];
	}
};