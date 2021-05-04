#pragma once
#include "Frequency.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class FileUtil {
public:
	int writeBinFile(string filename, string outs) {
		ofstream file(filename, ios::out | ios::binary);
		if (file.is_open()) {
			file.write(outs.c_str(),outs.size());
		}
		else {
			cerr << "Unable to open file:  " << filename << endl;
			return -1;
		}
		return 1;
	}

	int readBinFile(string filename, unsigned char** slist, int& s) {
		ifstream file(filename, ios::in | ios::binary);
		if (file.is_open()) {
			file.seekg(0, ios::end);
			int size = file.tellg();
			unsigned char* memblock = new unsigned char[size];
			file.seekg(0, ios::beg);
			file.read((char*)memblock, size);
			file.close();
			*slist = memblock;
			s = size;
		}
		else {
			cerr << "Unable to open file:  " << filename << endl;
			return -1;
		}
		return 1;
	}

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