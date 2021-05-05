#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

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
		}
		else {
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