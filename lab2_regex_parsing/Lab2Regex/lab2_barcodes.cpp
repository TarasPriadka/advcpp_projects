#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <bitset>
#include <stdexcept>

using namespace std;

struct Product {
	string name;
	string barcode = string(48, '@');
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

class FileUtil {
public:
	int readFile(string filename, vector<string>& slist) {
		ifstream file(filename);
		string buffer = "";
		if (file.is_open()) {
			while (!file.eof()) {
				getline(file, buffer);
				if (buffer.length() > 0) {
					slist.push_back(buffer);  // array notation
					buffer = "";
				}
			}
		} else {
			cerr << "Unable to open file:  " << filename << endl;
			return -1;
		}
		file.close();
		return slist.size();
	}

	template<typename T>
	void writeFile(string sfile, vector<T> sdata) { // or slist[]
		ofstream outFile(sfile);
		for (auto output : sdata)
			outFile << output << endl;
	}

	template<typename T>
	void writeFile(string sfile, T sdata) { // or slist[]
		ofstream outFile(sfile);
		outFile << sdata << endl;
	}
};

class XMLProcessor {
	regex tag;
	regex tagOpen;
	regex tagClose;
public:
	XMLProcessor() {
		tag = regex("<\/?[A-Za-z0-9]*>");
		tagOpen = regex("<[A-Za-z0-9]*>");
		tagClose = regex("<\/[A-Za-z0-9]*>");
	}
	vector<shared_ptr<XMLElement>> processXMLFile(string path);
	shared_ptr<XMLElement> parseStringToProduct(string& joinedLines);
};

vector<shared_ptr<XMLElement>> XMLProcessor::processXMLFile(string path) {
	vector<shared_ptr<XMLElement>> extractedTags;
	FileUtil files;
	vector<string> lines;
	files.readFile(path, lines);
	string meshedLines;
	for (string line : lines)
		meshedLines += line + '\n';
	// Start -> find the first matching open tag -> extract name into a new xmltag
	// continue until reach closing tag
	// capture all of the other tags in between open and closed tag

	while (!meshedLines.empty()) { // algorithm will move through the string, when no string left, you are done
		try {
			shared_ptr<XMLElement> newElem = parseStringToProduct(meshedLines);
			if (newElem)
				extractedTags.push_back(newElem);
		} catch (const std::exception& e) {
			cout << "Encountered an error while parsing XML." << endl;
			cout << e.what() << endl;
			return vector<shared_ptr<XMLElement>>{};
		}
	}
	return extractedTags;
}

// Alg:
// Open new tag-> find next tag -> check if it is a closing tag or a new tag
// if new tag recurse substring starting from the new tag
// Assumption, node which includes other nodes(not a leaf node), will not include any data value
shared_ptr<XMLElement> XMLProcessor::parseStringToProduct(string& lines) {
	shared_ptr<XMLElement> elem(new XMLElement);
	smatch match;
	regex_search(lines, match, this->tag);
	string tag_str = match.str();

	// if the found text an open tag
	if (regex_search(tag_str.begin(), tag_str.end(), this->tagOpen)) {
		elem->name = tag_str.substr(1, tag_str.length() - 2);
		shared_ptr<XMLElement> child;
		bool didLoop = false; //if you already looped, need to not cut the tag since it wasn't yet processed
		while (true) {
			regex_search(lines, match, this->tag);
			string next_section = lines.substr(didLoop ? match.position() : match.position() + match.length());
			child = parseStringToProduct(next_section);
			if (child->name == "/" + elem->name) { // current node is a leaf node
				if (elem->children.empty()) {
					elem->value = child->value;
				}
				lines = next_section;
				return elem;
			} else if (regex_search(child->name.begin(), child->name.end(), regex("\/.*"))) {
				throw std::invalid_argument("Improperly formated XML file. Tag " + tag_str + " doesn't match with <" + child->name + ">");
			} else {
				elem->children.push_back(move(child));
				lines = next_section;
				didLoop = true;
			}
		}
	} else if (regex_search(tag_str.begin(), tag_str.end(), this->tagClose)) {
		elem->name = tag_str.substr(1, tag_str.length() - 2);
		elem->value = lines.substr(0, match.position());
		//elem->name = "END";
		lines = lines.substr(match.position() + match.length(), lines.length());
		return elem;
	} else {
		//didn't find any tags left, you are done
		lines = string();
		return NULL;
	}
	return elem;
}

class Parser {
public:
	static vector<string> split(string str, string pattern);
	static string convertToBits(string h);
};
string Parser::convertToBits(string h) {
	h = "0x" + h;
	stringstream ss;
	ss << std::hex << h;
	long long n;
	ss >> n;
	bitset<48> b(n);
	return b.to_string();

}

vector<string> Parser::split(string s, string p) {
	vector<string> splitText;
	std::regex rgx(p);
	std::sregex_token_iterator iter(s.begin(),
		s.end(),
		rgx,
		-1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter)
		splitText.push_back(*iter);
	return splitText;
}

class CSVProcessor {
public:
	vector<shared_ptr<Cart>> processCSVFile(string path);
};

vector<shared_ptr<Cart>> CSVProcessor::processCSVFile(string path) {
	vector<shared_ptr<Cart>> extractedCarts;
	FileUtil files;
	vector<string> lines;
	files.readFile(path, lines);

	bool nameLine = true;

	for (string line : lines) {
		if (nameLine) {
			shared_ptr<Cart> newCart(new Cart);
			newCart->name = line;
			extractedCarts.push_back(move(newCart));
		} else {
			for (string barcode : Parser::split(line, ","))
				extractedCarts.back()->barcodes.push_back(barcode);
		}
		nameLine = !nameLine;
	}
	return extractedCarts;
}

string decodingScheme = "\
' '\n\
011000100\n\
' '\n\
000000000\n\
'-'\n\
010000101\n\
'+'\n\
010001010\n\
'$'\n\
010101000\n\
'%'\n\
000101010\n\
'*'\n\
010010100\n\
'.'\n\
110000100\n\
'/'\n\
010100010\n\
'0'\n\
000110100\n\
'1'\n\
100100001\n\
'2'\n\
001100001\n\
'3'\n\
101100000\n\
'4'\n\
000110001\n\
'5'\n\
100110000\n\
'6'\n\
001110000\n\
'7'\n\
000100101\n\
'8'\n\
100100100\n\
'9'\n\
001100100\n\
'A'\n\
100001001\n\
'B'\n\
001001001\n\
'C'\n\
101001000\n\
'D'\n\
000011001\n\
'E'\n\
100011000\n\
'F'\n\
001011000\n\
'G'\n\
000001101\n\
'H'\n\
100001100\n\
'I'\n\
001001100\n\
'J'\n\
000011100\n\
'K'\n\
100000011\n\
'L'\n\
001000011\n\
'M'\n\
101000010\n\
'N'\n\
000010011\n\
'O'\n\
100010010\n\
'P'\n\
001010010\n\
'Q'\n\
000000111\n\
'R'\n\
100000110\n\
'S'\n\
001000110\n\
'T'\n\
000010110\n\
'U'\n\
110000001\n\
'V'\n\
011000001\n\
'W'\n\
111000000\n\
'X'\n\
010010001\n\
'Y'\n\
110010000\n\
'Z'\n\
011010000\n\
";

struct TreeElem {
	char c = '\0';
	shared_ptr<TreeElem> left = NULL;
	shared_ptr<TreeElem> right = NULL;
};

class BarcodeDecoder {
	shared_ptr<TreeElem> tree;
	void createNameTree();
public:
	BarcodeDecoder() {
		tree = shared_ptr<TreeElem>(new TreeElem);
		tree->c = '@';
		createNameTree();
	};

	char barcodeToChar(string);

	vector<shared_ptr<Product>> convertXMLtoProducts(vector<shared_ptr<XMLElement>>);
};

char BarcodeDecoder::barcodeToChar(string barcode) {
	shared_ptr<TreeElem> curr = this->tree;
	for (int i = 0; i < 9; i++) {
		if (barcode[i] == '0') {
			if (!curr->left)
				throw invalid_argument("Invalid code");
			curr = curr->left;
		} else {
			if (!curr->right)
				throw invalid_argument("Invalid code");
			curr = curr->right;
		}
	}
	return curr->c;
}

// Alg: go through each elem in the decoding schema
// create nodes left and right
void BarcodeDecoder::createNameTree() {
	vector<string> tokenizedText = Parser::split(decodingScheme, "\n");
	char current = '\0';
	bool currentChar = true;
	for (auto elem : tokenizedText) {
		if (currentChar) {
			current = elem[1];
		} else {
			shared_ptr<TreeElem> curr = this->tree;
			for (char bit : elem) {
				if (bit == '0') {
					if (!curr->left) {
						curr->left = shared_ptr<TreeElem>(new TreeElem);
						curr->left->c = '@';
					}
					curr = curr->left;
				} else {
					if (!curr->right) {
						curr->right = shared_ptr<TreeElem>(new TreeElem);
						curr->right->c = '@';
					}
					curr = curr->right;
				}
			}
			curr->c = current;
		}
		currentChar = !currentChar;
	}
}



vector<shared_ptr<Product>> BarcodeDecoder::convertXMLtoProducts(vector<shared_ptr<XMLElement>> xmlProducts) {
	vector<shared_ptr<Product>> products;
	int i_ = 0;
	bool badXML = false;
	for (auto elem : xmlProducts) {
		//cout << i_++ << endl;
		try {
			shared_ptr<Product> product(new Product);
			for (auto child : elem->children) {
				if (child->name == "Price") {
					if (child->value.empty())
						throw invalid_argument("Price in an element is empty...");
					product->price = atof(child->value.c_str());
				}
				if (child->name == "Barcode") {
					if (child->value.length() < 48) {
						badXML = true;
						throw invalid_argument("Barcode is empty, cannot process...");
					}

					for (int i = 0; i < child->value.length(); i++)
						product->barcode[i] = child->value[i];
					string name;
					for (int i = 0; i < 45; i += 9) {
						try {
							name += this->barcodeToChar(product->barcode.substr(i, 9));
						} catch (exception e_) {
							cout << e_.what() << endl;
							break;
						}
					}
					product->name = name;
				}

			}
			products.push_back(move(product));
		} catch (const std::exception e) {
			cout << "Couldn't process node with name " << elem->name << endl;
			cout << e.what() << endl;
			continue;
		}
	}

	return products;
}

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

#pragma once
#include <string>
#include <vector>
#include <regex>
#include "Files.h"
#include "XMLProcessor.h"
#include "Structs.h"
#include "Parsers.h"
#include "Hashtable.h"
#include "Decoder.h"

using namespace std;

class CartProcessor {
	BarcodeDecoder decoder;
	Hashtable hashTable;

public:
	CartProcessor(vector<shared_ptr<XMLElement>> xmlElems);
	shared_ptr<ProcessedCart> processCart(shared_ptr<Cart> cart);
	void processCarts(vector<shared_ptr<Cart>> carts);
};

CartProcessor::CartProcessor(vector<shared_ptr<XMLElement>> xmlElems) : decoder(BarcodeDecoder{}) {
	hashTable = Hashtable(this->decoder.convertXMLtoProducts(xmlElems));
}

shared_ptr<ProcessedCart> CartProcessor::processCart(shared_ptr<Cart> cart) {
	shared_ptr<ProcessedCart> processedCart(new ProcessedCart);
	processedCart->name = cart->name;
	for (auto hexcode : cart->barcodes) {
		if (hexcode.empty())
			continue;
		string barcode = Parser::convertToBits(hexcode);
		shared_ptr<Product> prod = hashTable.get(barcode);
		if (prod)
			processedCart->products.push_back(prod);
	}
	return move(processedCart);
}

void CartProcessor::processCarts(vector<shared_ptr<Cart>> carts) {
	for (auto cart : carts) {
		shared_ptr<ProcessedCart> c = processCart(cart);
		double total_price = 0;
		cout << "Cart Name: " << c->name << endl;
		cout << "Cart Items:" << endl;
		for (auto p : c->products) {
			cout << "\t" << p->name << " | " << p->price << endl;
			total_price += p->price;
		}
		cout << endl << "Total: " << total_price << endl << endl;
	}
}

int main() {
	CSVProcessor csvProcessor;
	XMLProcessor xmlProcessor;
	BarcodeDecoder decoder;

	auto xmls = xmlProcessor.processXMLFile("ProductPrice.xml");
	vector<shared_ptr<Cart>> carts = csvProcessor.processCSVFile("Carts.csv");

	CartProcessor cp(xmls);
	cp.processCarts(carts);

	return 0;
}