#pragma once

#include <string>
#include <vector>
#include <bitset>
#include <memory>
#include <sstream>
#include <bitset>

using namespace std;

struct Product {
	string name;
	string barcode = string(48,'@');
	float price;
};

struct XMLElement {
	string name;
	vector<shared_ptr<XMLElement>> children;
	string value;
	XMLElement() : name(""), children(), value("") {}
	XMLElement(string n, vector<shared_ptr<XMLElement>> c, string v) : name(n), children(c), value(v) {}
};

struct Cart {
	string name;
	vector<string> barcodes;
};

struct ProcessedCart {
	string name;
	vector<shared_ptr<Product>> products;
};