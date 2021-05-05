#pragma once

#include <utility>
#include <string>

using namespace std;

struct KeyValue : private pair<int, string> {
	int key;
	string value; 
	
	KeyValue() : KeyValue(0,"") {}

	KeyValue(int k, string v) : pair(k,v), key(k), value(v) {}
};