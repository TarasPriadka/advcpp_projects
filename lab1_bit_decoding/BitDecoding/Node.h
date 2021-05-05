#pragma once
#include <string>
#include <iostream>

using namespace std;


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

class LeafNode : public Node {
private:
	double f;
	string s;
public:
	LeafNode(string sym, double freq) :f(freq), s(sym) {}
	
	double frequency() { return f; }

	string symbol() { return s; }
};

class BranchNode : public Node {
private:
	shared_ptr<Node> l;
	shared_ptr<Node> r;
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

