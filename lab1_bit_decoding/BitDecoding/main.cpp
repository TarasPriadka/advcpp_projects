#include <iostream>
#include "FrequencyCoder.h"
#include "Node.h"
#include "FileUtil.h"

int main() {

	FrequencyCoder coder;
	FileUtil util;

	//string message("Message to encode");
	////string message("ococ");
	//cout << "message: " << message << endl;
	//string encoded = coder.encode(message);
	//vector<bitset<8>> bs_vec;
	//string bit_output;
	//bitset<8> bs;
	//for (int c = 0, i = 0; i < encoded.size(); i++,c++) {
	//	if (c == 8) {
	//		bs_vec.push_back(bs);
	//		bit_output += static_cast<unsigned char>(bs.to_ulong());
	//		bs.set().flip();
	//		c = 0;
	//	}
	//	bs[c] = encoded[i] == '1';
	//}
	//bs_vec.push_back(bs);
	//bit_output += static_cast<unsigned char>(bs.to_ulong());
	//cout << "Encoded message: " << encoded << endl << "Encoded message: ";

	//util.writeBinFile("EncodeTest.bin", bit_output);


	//cout << "Encoded message: " << encoded << endl;
	////cout << "Decoded message: " << coder.decode(reinterpret_cast<unsigned char*>(const_cast<char*>(encoded.c_str())), encoded.size()) << endl;
	//cout << "Decoded message: " << coder.decode(encoded) << endl;

	//auto queue = coder.getPrioriotyQueue();
	//auto map = coder.getEncodeMap();

	//for (auto elem : map) {
	//	cout << elem.first << " " << elem.second << endl;
	//}

	unsigned char* chars;
	int s = 0;
	util.readBinFile("Compress.bin",&chars, s);
	string out = coder.decode(chars, s);
	cout << out << endl;
	delete[]chars;

	//for (int i = 0; !queue.empty(); i++) {
	//	//cout << typeid(queue.top()).name() << endl;
	//	cout << bool(dynamic_cast<BranchNode*>(queue.top().get())) << endl;
	//	cout << queue.top() << endl;
	//	//const LeafNode* n = dynamic_cast<LeafNode*>(const_cast<Node*>(&queue.top()));
	//	//cout << n << endl;
	//	queue.pop();
	//}
	return 0;
}
