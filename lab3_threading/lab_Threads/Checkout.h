#pragma once

#include "Structs.h"
#include "CartProcessor.h"
#include "Hashtable.h"
#include "Decoder.h"

#include <vector>
#include <mutex>
#include <string>
#include <thread>
#include <deque>
#include <memory>
#include <algorithm>
#include <condition_variable>

using namespace std;

mutex mtx; //mutex to access database
mutex coutM; //cout mutex
condition_variable cv;

class Lane {
private:
	string _name;
	deque<shared_ptr<Cart>> carts;
	BarcodeDecoder decoder;
	Hashtable database;
	bool _finished = false;
public:
	Lane(string name, Hashtable& dbp, BarcodeDecoder& decoder);
	void addCart(shared_ptr<Cart> cart) { carts.push_back(cart); }
	shared_ptr<ProcessedCart> processCart(shared_ptr<Cart>);
	void processCarts();
	void finished(bool f) { _finished = f; }
	int size() { return carts.size(); }
	string name() { return _name; }
};

Lane::Lane(string n, Hashtable& dbp, BarcodeDecoder& decoder) : _name(n), decoder{ decoder }, database{ dbp }{}

void Lane::processCarts() {
	{
		lock_guard<mutex> coutL(coutM);
		cout << "Started " << name() << endl;
	}
	while (!_finished || carts.size() > 0) {
		unique_lock<mutex> locker(mtx);
		cv.wait(locker, [this]() {return carts.size() > 0; });
		locker.unlock();
		shared_ptr<Cart> cart = carts.front();
		carts.pop_front();
		shared_ptr<ProcessedCart> c = processCart(cart);

		double total_price = 0;
		{
			lock_guard<mutex> coutL(coutM);
			cout << "Cart Name: " << c->name << endl;
			cout << "Cart Items:" << endl;
			for (auto p : c->products) {
				cout << "\t" << p->name << " | " << p->price << endl;
				total_price += p->price;
			}
			cout << endl << "Total: " << total_price << endl << endl;
		}
	}
}

shared_ptr<ProcessedCart> Lane::processCart(shared_ptr<Cart> cart) {
	shared_ptr<ProcessedCart> processedCart(new ProcessedCart);
	processedCart->name = cart->name;
	for (auto hexcode : cart->barcodes) {
		if (hexcode.empty())
			continue;

		string barcode = Parser::convertToBits(hexcode);
		unique_lock<mutex> mque(mtx);
		shared_ptr<Product> prod = database.get(barcode);
		mque.unlock();
		if (prod)
			processedCart->products.push_back(prod);
	}
	return move(processedCart);
}

class Checkout : public CartProcessor {
	const int DEFAULT_LANE_SIZE = 10;
	vector<shared_ptr<Lane>> lanes;
	int laneSize;
private:
	using CartProcessor::processCart;
public:
	Checkout(vector<shared_ptr<XMLElement>> xmlElems);
	Checkout(vector<shared_ptr<XMLElement>> xmlElems, int numLanes);

	void processCarts(vector<shared_ptr<Cart>> carts);
};

Checkout::Checkout(vector<shared_ptr<XMLElement>> xmlElems) : CartProcessor(xmlElems), laneSize(DEFAULT_LANE_SIZE) {
	for (int i = 0; i < 5; i++)
		lanes.push_back(shared_ptr<Lane>(new Lane(string("lane ") + to_string(i), this->hashTable, this->decoder)));
}

Checkout::Checkout(vector<shared_ptr<XMLElement>> xmlElems, int numLanes) : CartProcessor(xmlElems), laneSize(DEFAULT_LANE_SIZE) {
	for (int i = 0; i < numLanes; i++)
		lanes.push_back(shared_ptr<Lane>(new Lane(string("lane ") + to_string(i), this->hashTable, this->decoder)));
}

// Plan:
// Initialize n lanes
// start lanes
//    - Lane awaits for carts to process until done flag is set
// Checkout thread to add x number of carts to each lane
// finish when ran out of carts to process and all lanes have finished
void Checkout::processCarts(vector<shared_ptr<Cart>> carts) {
	deque<thread> runningThreads;

	for_each(lanes.begin(), lanes.end(), [&](shared_ptr<Lane> lane) {
		runningThreads.push_back(thread(&Lane::processCarts, ref(*lane)));
		});

	while (!carts.empty()) {
		for (auto lane : lanes) {
			if (carts.empty())
				break;
			if (lane->size() < this->laneSize) {
				unique_lock<mutex> l2(mtx);
				lane->addCart(carts.back());
				carts.pop_back();
			}
			//notify all threads that new carts have been added to lanes so that they can start processing
			cv.notify_all();
		}
	}

	for (auto lane : lanes)
		lane->finished(true);

	for_each(runningThreads.begin(), runningThreads.end(), [&](thread &t) { t.join(); });
}