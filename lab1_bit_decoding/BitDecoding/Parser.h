#pragma once

#include "Frequency.h"
#include <vector>
#include <string>

using namespace std;

class Parser {
public:
	vector<Frequency> parseFrequencyFile(vector<string> lines);
	Frequency getFrequency(string line);
	vector<string> strtok_(string text, string delim);
	int atoi(string astring);
	double atof(string astring);
};