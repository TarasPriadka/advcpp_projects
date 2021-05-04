#pragma once

#include <queue>
#include "Frequency1.h"
#include "QueueTree.h"
#include <string>
#include <vector>
#include <bitset>
#include <memory>
#include <iostream>

using namespace std;

// The compare function for the Priority_Queue
struct myLess
{
	bool operator () (shared_ptr<Node> n1, shared_ptr<Node> n2)
	{
		return n1->frequency() > n2->frequency();
	}
};

class FrequencyCoder {
private:
	priority_queue<shared_ptr<Node>,vector<shared_ptr<Node>>,myLess> frequencyTable;
	vector<pair<string, string>> encodeMap;
	//void constructTree();
public:
	FrequencyCoder();
	FrequencyCoder(vector<frequency>);
	void constructTree();
	vector<pair<string, string>> getEncodeMap(){ return encodeMap; }
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
		shared_ptr<Node> p (new LeafNode(string(1, freq._c), freq._f));
		frequencyTable.push(p);
	}
	constructTree();
}

FrequencyCoder::FrequencyCoder(vector<frequency> vec) {
	for (frequency freq : vec) {
		shared_ptr<Node> p (new LeafNode(string(1, freq._c), freq._f));
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

string FrequencyCoder::decode(unsigned char* chars, size_t s) {
	string decoded;
	shared_ptr<Node> curr = frequencyTable.top();
	for (int j = 0; j < s; j++) {
		bitset<8> bs(chars[j]);
		//cout << bs << endl;
		for (int i = 7; i >= 0; i--){//(int i = 0; i < 8; i++) { 
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

//010110100010001111011110 
//1010
//0101101000
//0101101000100011110111101010100101100011001011100111100100011101010000011010101000