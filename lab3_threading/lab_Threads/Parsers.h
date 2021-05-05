#pragma once

#include <string>
#include <vector>
#include <regex>
#include "Files.h"
#include "XMLProcessor.h"
#include "Structs.h"

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
