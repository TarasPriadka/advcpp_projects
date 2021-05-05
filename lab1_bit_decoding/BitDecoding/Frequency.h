#pragma once
#include <utility>
#include <string>

using namespace std;

class Frequency {
private:
	pair<string, int> pair;
public:
	Frequency(string str, int freq);
	void increment();
	string getString();
	int getFrequency();
	friend bool operator==(Frequency& a, Frequency& b);
	friend bool operator<(Frequency& a, Frequency& b);
	friend bool operator>(Frequency& a, Frequency& b);
};

Frequency::Frequency(string str, int freq) {
	this->pair.first = str;
	this->pair.second = freq;
}

void Frequency::increment() {
	this->pair.second++;
}

string Frequency::getString() {
	return this->pair.first;
}

int Frequency::getFrequency() {
	return this->pair.second;
}

bool operator==(Frequency& a, Frequency& b) { return a.getFrequency() == b.getFrequency(); }
bool operator<(Frequency& a, Frequency& b) { return a.getFrequency() < b.getFrequency(); }
bool operator>(Frequency& a, Frequency& b) { return a.getFrequency() > b.getFrequency(); }