#pragma once

#include <sstream>
#include "Structs.h"
#include "CartProcessor.h"
#include "Parsers.h"

using namespace std;

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
		}
		else {
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
	vector<string> tokenizedText = Parser::split(decodingScheme,"\n");
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