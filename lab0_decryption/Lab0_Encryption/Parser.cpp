#pragma once

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Parser {
public:
	vector<int> parseFileLine(string line) {
		vector<string> tokens = Parser::strtok_(line, string(", "));
        vector<int> int_tokens(tokens.size());
        int vec_size = 0;

        std::transform(tokens.begin(), tokens.end(), int_tokens.begin(), [&vec_size, this](string tok) {
            if (tok != "") { 
                vec_size++;
                return this->atoi(tok); }
            }
        );

        return vector<int>(int_tokens.begin(),int_tokens.begin() + vec_size);
	}

	vector<string> strtok_(string text, string delim) {
        vector<string> tokens;
        string stemp;
        int start = 0;
        int index = text.find(delim);

        while (index != string::npos) {
            stemp.assign(text, start, index - start);
            tokens.push_back(stemp);
            start = index + delim.size();
            index = text.find(delim, start);
        }
        stemp.assign(text, start, text.length() - start);
        tokens.push_back(stemp);
        return tokens;
    }

	int atoi(string astring) {
		return stoi(astring, nullptr, 10);
	}

	double atof(string astring) {
		return stod(astring, nullptr);
	}
};

