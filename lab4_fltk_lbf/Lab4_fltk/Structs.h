#pragma once

// DEMONSTRATE HOW TO XYPLOT IN FLTK
#include <Windows.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <string>	
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

#include "Processing.h"
#include "Graphing.h"

using namespace std;


class Point : pair<double, double> {
	double X = 0;
	double Y = 0;
public:
	Point(double a, double b) {
		first = a;
		second = b;
		X = first;
		Y = second;
	}
	double getX() { return X; }
	double getY() { return Y; }
};

class PointCompareX {
public:
	bool operator()(Point a, Point b) {
		return a.getX() < b.getX();
	}
};

class PointCompareY {
public:
	bool operator()(Point a, Point b) {
		return a.getY() < b.getY();
	}
};

class Points {
	vector<Point> points;
	pair<double, double> minimum;
	pair<double, double> maximum;
public:
	int size() { return points.size(); }
	Point get(int index) { return points[index]; }
	Point operator [] (int index) { return get(index); }
	void set(Point p) { points.push_back(p); }
	void set(double x, double y) {
		Point p(x, y);
		points.push_back(p);
	}
	double maxX() {
		return max_element(points.begin(), points.end(), PointCompareX())->getX();
	}

	double minX() {
		return min_element(points.begin(), points.end(), PointCompareX())->getX();
	}

	double maxY() {
		return max_element(points.begin(), points.end(), PointCompareY())->getY();
	}
	double minY() {
		return min_element(points.begin(), points.end(), PointCompareY())->getY();
	}
};