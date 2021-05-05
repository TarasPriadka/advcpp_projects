//#pragma once
//
//#include "Node.h"
//#include <queue>
//#include <vector>
//#include "Frequency.h"
//#include "FrequencyCoder.h"
//
//using namespace std;
//
//class QueueTree {
//private:
//	shared_ptr<Node> tree;
//	vector<pair<string, string>> encodeMap;
//	shared_ptr<Node> constructTree(queue<shared_ptr<Node>> queue);
//	vector<pair<string, string>> generateEncodeMap(shared_ptr<Node> tree, string enc);
//public:
//	QueueTree(queue<shared_ptr<Node>> frequencyQueue);
//	shared_ptr<Node> getTree();
//	vector<pair<string, string>> getEncodeMap();
//};
//
//QueueTree::QueueTree(queue<shared_ptr<Node>> frequencyQueue) {
//	tree = constructTree(frequencyQueue);
//	encodeMap = generateEncodeMap(tree,"");
//}
//
//shared_ptr<Node> QueueTree::constructTree(queue<shared_ptr<Node>> queue) {
//
//	return NULL
//}
//
//vector<pair<string, string>> QueueTree::generateEncodeMap(shared_ptr<Node> tree, string enc) {
//	static vector<pair<string, string>> output;
//	if (dynamic_cast<BranchNode*>(tree.get())) {
//		shared_ptr<BranchNode> node = dynamic_pointer_cast<BranchNode>(tree);
//		generateEncodeMap(node->left(), enc + "0");
//		generateEncodeMap(node->right(), enc + "1");
//		return output;
//	}
//	else if (dynamic_cast<LeafNode*>(tree.get())) {
//		output.push_back(pair<string, string>(tree->symbol(), enc));
//		return vector<pair<string, string>>();
//	}
//}
//
//shared_ptr<Node> QueueTree::getTree() { return tree; }
//vector<pair<string, string>> QueueTree::getEncodeMap() { return encodeMap; }