#include "Files.h"
#include "XMLProcessor.h"
#include "CartProcessor.h"
#include "Decoder.h"
#include "Hashtable.h"
#include "Parsers.h"
#include "Checkout.h"

#include <iostream>
#include <string>
#include <vector>

#define DEBUG false;

using namespace std;

int main() {
	CSVProcessor csvProcessor;
	XMLProcessor xmlProcessor;
	BarcodeDecoder decoder;

	auto xmls = xmlProcessor.processXMLFile("ProductPrice.xml");
	//vector<shared_ptr<Cart>> carts = csvProcessor.processCSVFile("Carts.csv");
	vector<shared_ptr<Cart>> carts = csvProcessor.processCSVFile("carts_test.csv");
	
	Checkout checkout(xmls,10);
	
	checkout.processCarts(carts);
	//CartProcessor cp(xmls);
	//cp.processCarts(carts);

	return 0;
}





