// Taras Pridka
// CIS29
// Frequency Decoder - program that uses character relative frequencies to encode/decode strings into 
//					   compressed binary respresentations

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <bitset>
#include <memory>

using namespace std;

// --------- Node -------------
// Base class for Nodes

class Node {
public:
	virtual double frequency() { return 0; }
	virtual string symbol() { return string(""); }
	friend ostream& operator<<(ostream& o, Node* n);
};

ostream& operator<<(ostream& o, Node* n) {
	o << typeid(n).name() << ": <" << n->symbol() << "> " << n->frequency();
	return o;
}

// --------- Leaf Node -------------
// Endpoint node that contains symbol and its relative frequency

class LeafNode : public Node {
private:
	double f;
	string s;
public:
	LeafNode(string sym, double freq) :f(freq), s(sym) {}

	double frequency() { return f; }

	string symbol() { return s; }
};

// --------- Branch Node -------------
// Branch Node for the tree

class BranchNode : public Node {
private:
	shared_ptr<Node> l; // use shared_ptr to ensure that dynamic memory
	shared_ptr<Node> r; // is deallocated at the end of the program
public:
	BranchNode(shared_ptr<Node> ll, shared_ptr<Node> rr) : l(ll), r(rr) {}

	double frequency() {
		return l->frequency() + r->frequency();
	}

	string symbol() {
		return l->symbol() + r->symbol();
	}

	shared_ptr<Node> left() { return l; }
	shared_ptr<Node> right() { return r; }
};

// --------- Frequency -------------
// struct that contains symbol and its frequency
struct frequency
{
	char _c;
	double _f;
	frequency(char c, double f) { _c = c; _f = f; }
};

// --------- Frequency initializer -------------
// initializes all of the frequencies into a vector to be used later
class FrequencyInitializer {
private:
	vector<frequency> vfrequency;
public:
	FrequencyInitializer();
	vector <frequency> getFrequencies();
};

FrequencyInitializer::FrequencyInitializer() {
	this->vfrequency =
	{
	frequency(' ',8.0),
	frequency('!',0.0306942),
	frequency('"',0.000183067),
	frequency('#',0.00854313),
	frequency('$',0.00970255),
	frequency('%',0.00170863),
	frequency('&',0.00134249),
	frequency('\'',0.000122045),
	frequency('(',0.000427156),
	frequency(')',0.00115942),
	frequency('*',0.0241648),
	frequency('+',0.00231885),
	frequency(',',0.00323418),
	frequency('-',0.0197712),
	frequency('.',0.0316706),
	frequency('/',0.00311214),
	frequency('0',2.74381),
	frequency('1',4.35053),
	frequency('2',3.12312),
	frequency('3',2.43339),
	frequency('4',1.94265),
	frequency('5',1.88577),
	frequency('6',1.75647),
	frequency('7',1.621),
	frequency('8',1.66225),
	frequency('9',1.79558),
	frequency(':',0.00189169),
	frequency(';',0.00207476),
	frequency('<',0.000427156),
	frequency('=',0.00140351),
	frequency('>',0.000183067),
	frequency('?',0.00207476),
	frequency('@',0.0238597),
	frequency('A',0.130466),
	frequency('B',0.0806715),
	frequency('C',0.0660872),
	frequency('D',0.0698096),
	frequency('E',0.0970865),
	frequency('F',0.0417393),
	frequency('G',0.0497332),
	frequency('H',0.0544319),
	frequency('I',0.070908),
	frequency('J',0.0363083),
	frequency('K',0.0460719),
	frequency('L',0.0775594),
	frequency('M',0.0782306),
	frequency('N',0.0748134),
	frequency('O',0.0729217),
	frequency('P',0.073715),
	frequency('Q',0.0147064),
	frequency('R',0.08476),
	frequency('S',0.108132),
	frequency('T',0.0801223),
	frequency('U',0.0350268),
	frequency('V',0.0235546),
	frequency('W',0.0320367),
	frequency('X',0.0142182),
	frequency('Y',0.0255073),
	frequency('Z',0.0170252),
	frequency('[',0.0010984),
	frequency('\\',0.00115942),
	frequency(']',0.0010984),
	frequency('^',0.00195272),
	frequency('_',0.0122655),
	frequency('\'',0.00115942),
	frequency('a',7.52766),
	frequency('b',2.29145),
	frequency('c',2.57276),
	frequency('d',2.76401),
	frequency('e',7.0925),
	frequency('f',1.2476),
	frequency('g',1.85331),
	frequency('h',2.41319),
	frequency('i',4.69732),
	frequency('j',0.836677),
	frequency('k',1.96828),
	frequency('l',3.77728),
	frequency('m',2.99913),
	frequency('n',4.56899),
	frequency('o',5.17),
	frequency('p',2.45578),
	frequency('q',0.346119),
	frequency('r',4.96032),
	frequency('s',4.61079),
	frequency('t',3.87388),
	frequency('u',2.10191),
	frequency('v',0.833626),
	frequency('w',1.24492),
	frequency('x',0.573305),
	frequency('y',1.52483),
	frequency('z',0.632558),
	frequency('{',0.000122045),
	frequency('|',0.000122045),
	frequency('}',0.000122045),
	frequency('~',0.00152556)
	};
}

vector<frequency> FrequencyInitializer::getFrequencies() {
	return this->vfrequency;
}

// --------- myLess -------------
// A struct to be used inside of priority queue

struct myLess
{
	bool operator () (shared_ptr<Node> n1, shared_ptr<Node> n2)
	{
		return n1->frequency() > n2->frequency();
	}
};

// --------- Frequency Coder -------------
// Uses frequency vector to construct a priority queue
// and afterwards constructs a tree with those frequencies
// contains methods to encode and decode a tree

class FrequencyCoder {
private:
	// frequency table which is used to construct a freq tree
	priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, myLess> frequencyTable;
	vector<pair<string, string>> encodeMap; //after tree is constructed encode map is populated
	void constructTree();
public:
	FrequencyCoder();
	FrequencyCoder(vector<frequency>);
	vector<pair<string, string>> getEncodeMap() { return encodeMap; }
	priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, myLess> getPrioriotyQueue();
	string decode(unsigned char* chars, size_t s);
	string decode(string bits);
	string encode(string input);
	vector<pair<string, string>> generateEncodeMap(shared_ptr<Node> tree, string enc);

};

FrequencyCoder::FrequencyCoder() {
	FrequencyInitializer init;
	vector<frequency>freqsList = init.getFrequencies();
	for (frequency freq : freqsList) {
		shared_ptr<Node> p(new LeafNode(string(1, freq._c), freq._f));
		frequencyTable.push(p);
	}
	constructTree();
}

FrequencyCoder::FrequencyCoder(vector<frequency> vec) {
	for (frequency freq : vec) {
		shared_ptr<Node> p(new LeafNode(string(1, freq._c), freq._f));
		frequencyTable.push(p);
	}
	constructTree();
}

priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, myLess> FrequencyCoder::getPrioriotyQueue() {
	return frequencyTable;
}

void FrequencyCoder::constructTree() {
	while (frequencyTable.size() > 1) {
		shared_ptr<Node> qLeft = frequencyTable.top();
		frequencyTable.pop();
		shared_ptr<Node> qRight = frequencyTable.top();
		frequencyTable.pop();
		frequencyTable.push(shared_ptr<BranchNode>(new BranchNode(qLeft, qRight)));
	}
	this->encodeMap = generateEncodeMap(frequencyTable.top(), "");
}

string FrequencyCoder::encode(string input) {
	string output;
	for (char c : input) {
		for (auto p : encodeMap) {
			if (p.first == string(1, c)) {
				output += p.second;
			}
		}
	}
	return output;
}

vector<pair<string, string>> FrequencyCoder::generateEncodeMap(shared_ptr<Node> tree, string enc) {
	static vector<pair<string, string>> output;
	if (dynamic_cast<BranchNode*>(tree.get())) {
		shared_ptr<BranchNode> node = dynamic_pointer_cast<BranchNode>(tree);
		generateEncodeMap(node->left(), enc + "0");
		generateEncodeMap(node->right(), enc + "1");
		return output;
	}
	else if (dynamic_cast<LeafNode*>(tree.get())) {
		output.push_back(pair<string, string>(tree->symbol(), enc));
		return vector<pair<string, string>>();
	}
}

//take in a list of chars read from the binary file. Assume bits are reversed
string FrequencyCoder::decode(unsigned char* chars, size_t s) {
	string decoded;
	shared_ptr<Node> curr = frequencyTable.top();
	for (int j = 0; j < s; j++) {
		bitset<8> bs(chars[j]);
		//cout << bs << endl;
		for (int i = 7; i >= 0; i--) {//(int i = 0; i < 8; i++) { 
			bool bit = bs[i];
			if (dynamic_cast<BranchNode*>(curr.get())) {
				BranchNode* node = dynamic_cast<BranchNode*>(curr.get());
				curr = bit ? node->right() : node->left();
				if (dynamic_cast<LeafNode*>(curr.get())) {
					decoded += curr->symbol();
					curr = frequencyTable.top();
				}
			}
		}
	}
	return decoded;
}
// Take in a string of bits. Assume bits are already in the right order(not reversed)
string FrequencyCoder::decode(string bits) {
	string decoded;
	shared_ptr<Node> curr = frequencyTable.top();
	for (char b : bits) {
		bool bit = b == '1';
		if (dynamic_cast<BranchNode*>(curr.get())) {
			BranchNode* node = dynamic_cast<BranchNode*>(curr.get());
			curr = bit ? node->right() : node->left();
			if (dynamic_cast<LeafNode*>(curr.get())) {
				decoded += curr->symbol();
				curr = frequencyTable.top();
			}
		}
	}
	return decoded;
}

// --------- FileUtil -------------
// Used to read and write binary files

class FileUtil {
public:
	int writeBinFile(string filename, string outs) {
		ofstream file(filename, ios::out | ios::binary);
		if (file.is_open()) {
			file.write(outs.c_str(), outs.size());
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
};

int main() {
	FrequencyCoder coder;
	FileUtil util;

	unsigned char* chars;	//binary representations of chars
	int s = 0;				//size of chars array
	util.readBinFile("Compress.bin", &chars, s);
	string out = coder.decode(chars, s);
	cout << out << endl;
	delete[]chars;	// chars are dynamically allocated
	return 0;
}

/*
	Output:
	Stay Hungry, Stay Foolish.  I am honored to be with you today at your commencement from one of the finest universities in the world. I never graduated from college. Truth be told, this is the closest I've ever gotten to a college graduation. Today I want to tell you three stories from my life. That's it. No big deal. Just three stories... When I was young, there was an amazing publication called The Whole Earth Catalog, which was one of the bibles of my generation. It was created by a fellow named Stewart Brand not far from here in Menlo Park, and he brought it to life with his poetic touch. This was in the late 1960s, before personal computers and desktop publishing, so it was all made with typewriters, scissors, and polaroid cameras. It was sort of like Google in paperback form, 35 years before Google came along it was idealistic, and overflowing with neat tools and great notions.  Stewart and his team put out several issues of The Whole Earth Catalog, and then when it had run its course, they put out a final issue. It was the mid 1970s, and I was your age. On the back cover of their final issue was a photograph of an early morning country road, the kind you might find yourself hitchhiking on if you were so adventurous. Beneath it were the words "Stay Hungry. Stay Foolish." It was their farewell message as they signed off. Stay Hungry. Stay Foolish. And I have always wished that for myself. And now, as you graduate to begin anew, I wish that for you. Steve Jobs  h
*/