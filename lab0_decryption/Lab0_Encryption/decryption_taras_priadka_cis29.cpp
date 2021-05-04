// Taras Priadka
// CIS29
// Decryptor - program that reads in text from a file and decrypts it

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

// ################# HEADERS #################

struct KeyValue : private pair<int, string> {
	int key;
	string value;

	KeyValue() : KeyValue(0, "") {}

	KeyValue(int k, string v) : pair(k, v), key(k), value(v) {}
};

class Dictionary {
private:
	vector<KeyValue> dict;
	int dict_size;
public:
	Dictionary(int d) {
		dict_size = d;
		dict.resize(dict_size);
	};

	KeyValue& operator [](int key);
};

class Parser {
public:
	vector<int> parseFileLine(string line);

	vector<string> strtok_(string text, string delim);

	int atoi(string astring) {
		return stoi(astring, nullptr, 10);
	}

	double atof(string astring) {
		return stod(astring, nullptr);
	}
};

class Decryptor {
public:
	string decrypt(vector<int>::iterator begin, vector<int>::iterator end);
};

class FileUtil {
public:
	int readFile(string filename, vector<string>& slist);

	//two overloads for the method
	//one to write out data in a vector
	//separating each element with a new line
	template<typename T>
	void writeFile(string sfile, vector<T> sdata) {
		ofstream outFile(sfile);
		for (auto output : sdata)
			outFile << output << endl;
	}

	//overload to just write out the incoming object
	//just write out string for example
	template<typename T>
	void writeFile(string sfile, T sdata) {
		ofstream outFile(sfile);
		outFile << sdata << endl;
	}
};

class Driver {
private:
	FileUtil fileUtil;
	Parser parser;
	Decryptor decryptor;
public:
	Driver() {}

	void decode_file(string in_name, string out_name);
};

// ################# Implementations #################

KeyValue& Dictionary::operator[](int key) {
	if (key == dict_size) {
		dict_size = key + 1;
		dict.push_back(KeyValue(key, ""));
	}
	return dict[key];
}

string Decryptor::decrypt(vector<int>::iterator begin, vector<int>::iterator end) {
	// create a dictionary and initialize it
	int dictSize = 256;
	Dictionary dictionary(dictSize);
	for (int i = 0; i < dictSize; i++) {
		dictionary[i].key = i;
		dictionary[i].value = string(1, i);
	}
	// setup some local variables
	string word(1, *begin++);
	string result = word;
	string entry;
	// loop through each character in the iterator
	while (begin != end) {
		int key = *begin;
		if (key >= 0 && key < dictSize)
			entry = dictionary[key].value;  // found it
		else if (key == dictSize)
			entry = word + word[0];  // create new entry
		else
			throw "Invalid key";
		result += entry;
		dictionary[dictSize].value = word + entry[0]; // assumes vector implementation
		dictSize++;
		word = entry;
		begin++;
	}
	return result;
}

vector<int> Parser::parseFileLine(string line) {
	vector<string> tokens = this->strtok_(line, string(", "));
	vector<int> int_tokens(tokens.size());
	int vec_size = 0;

	std::transform(tokens.begin(), tokens.end(), int_tokens.begin(), [&vec_size, this](string tok) {
		if (tok != "") {
			vec_size++;
			return this->atoi(tok);
		}
		}
	);

	return vector<int>(int_tokens.begin(), int_tokens.begin() + vec_size);
}

vector<string> Parser::strtok_(string text, string delim) {
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

int FileUtil::readFile(string filename, vector<string>& slist) {
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

void Driver::decode_file(string in_name, string out_name) {
	std::vector<string> lines;
	fileUtil.readFile(in_name, lines);
	std::vector<int> file_nums = parser.parseFileLine(lines[0]);
	string out = decryptor.decrypt(file_nums.begin(), file_nums.end());
	cout << out << endl;
	fileUtil.writeFile(out_name, out);
}

int main() {
	Driver driver;
	driver.decode_file("Encrypted.txt", "Decrypter.txt");
	return 0;
}

/*
	OUTPUT:
	Margaret Hamilton American Computer Scientist.  Margaret Hamilton, maiden name Margaret Heafield, (born August 17, 1936, Paoli, Indiana, U.S.), American computer scientist who was one of the first computer software programmers; she created the term software engineer to describe her work. She helped write the computer code for the command and lunar modules used on the Apollo missions to the Moon in the late 1960s and early '70s.  While studying mathematics and philosophy at Earlham College in Richmond, Indiana, she met James Hamilton, and they subsequently married. After graduating in 1958, she taught high school mathematics for a short time. The couple then moved to Boston. Although Margaret planned to study abstract mathematics at Brandeis University, she accepted a job at the Massachusetts Institute of Technology (MIT) while her husband attended Harvard Law School. At MIT she began programming software to predict the weather and did postgraduate work in meteorology.  In the early 1960s Hamilton joined MIT's Lincoln Laboratory, where she was involved in the Semi-Automatic Ground Environment (SAGE) project, the first U.S. air defense system. She notably wrote software for a program to identify enemy aircraft. Hamilton next worked at MIT's Instrumentation Laboratory (now the independent Charles Stark Draper Laboratory), which provided aeronautical technology for the National Aeronautics and Space Administration (NASA). She led a team that was tasked with developing the software for the guidance and control systems of the in-flight command and lunar modules of the Apollo missions. At the time, no schools taught software engineering, so the team members had to work out any problems on their own. Hamilton herself specifically concentrated on software to detect system errors and to recover information in a computer crash. Both those elements were crucial during the Apollo 11 mission (1969), which took astronauts Neil Armstrong and Buzz Aldrin to the Moon.  Hamilton left MIT in the mid-1970s to work in the private sector. She cofounded the company Higher Order Software in 1976 and established Hamilton Technologies 10 years later.  Hamilton was the recipient of various honours, including NASA's Exceptional Space Act Award (2003). Pres. Barack Obama presented her with the Presidential Medal of Freedom in 2016.
*/