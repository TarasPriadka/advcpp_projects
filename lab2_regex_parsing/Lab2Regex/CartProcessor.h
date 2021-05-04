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