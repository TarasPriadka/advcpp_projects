#pragma once

#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <io.h>

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

class FileInfo {
private:
	vector<_finddata_t> vfiles;
	string path;
public:
	FileInfo() : path("./*") { getFileList(path); }
	FileInfo(string path) : path(path) { getFileList(path); }
	void setdir(string sf) { path = sf; }
	vector<_finddata_t>& getFiles() { return vfiles; }
	_finddata_t operator [](int x) { return vfiles[x]; }

	int getFileList(string sf) {
		path = sf;
		_finddata_t read_file;
		intptr_t hFile;

		if ((hFile = _findfirst(path.data(), &read_file)) != -1L) {
			do {
				vfiles.push_back(read_file);
			} while (_findnext(hFile, &read_file) == 0);
			_findclose(hFile);
		}
		return vfiles.size();
	}

	vector<_finddata_t>::iterator begin() {
		return vfiles.begin();
	}
	vector<_finddata_t>::iterator end() {
		return vfiles.end();
	}
};