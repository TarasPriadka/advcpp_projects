#include "Files.h"
#include "XMLProcessor.h"
#include "CartProcessor.h"
#include "Decoder.h"
#include "Hashtable.h"
#include "Parsers.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main1() {
	CSVProcessor csvProcessor;
	XMLProcessor xmlProcessor;
	BarcodeDecoder decoder;

	auto xmls = xmlProcessor.processXMLFile("ProductPrice.xml");
	vector<shared_ptr<Cart>> carts = csvProcessor.processCSVFile("Carts.csv");
	
	CartProcessor cp(xmls);
	cp.processCarts(carts);

	return 0;
}





