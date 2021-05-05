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

#include "Structs.h"
#include "Processing.h"

using namespace std;

class LinearRegression {
public:
	pair<Point, Point> fitLine(Points points) {
		pair<Point, Point> linePoints(Point(), Point());

	}
};

class Graph : public Fl_Widget {
	Points points;
	Points xtransform;
	Points ytransform;
	pair<double, double> axisrange;
	pair<double, double> minmaxX;
	pair<double, double> minmaxY;
	double border = 10;
public:
	Graph(int X, int Y, int W, int H, string S = "") : Fl_Widget(X, Y, W, H, S.data()) {}
	int size() { return points.size(); }
	void add(double x, double y) { points.set(x, y); }
	void add(Point& p) { points.set(p); }
	void bounds() {
		minmaxX.first = points.minX();
		minmaxX.second = points.maxX();
		minmaxY.first = points.minY();
		minmaxY.second = points.maxY();
		axisrange.first = minmaxX.second - minmaxX.first;
		axisrange.second = minmaxY.second - minmaxY.first;
	}
	Point scalePointX(int index, double interval) {
		Point px(interval, ytransform[index].getY());
		return px;
	}
	void scaleX() // assumes evenly spaced X intervals
	{
		int index = 0;
		double interval = floor((double)w() / (points.size()));
		for (double i = interval; i <= w() + interval; i += interval) {
			Point p = scalePointX(index++, i);
			xtransform.set(p);
		}
	}
	Point scalePointY(Point point) {
		Point py(point.getX(), (1.0 - (((point.getY() - minmaxY.first))) / axisrange.second) * h());
		return py;
	}
	void scaleY() {
		for (int i = 0; i < points.size(); i++) {
			Point p = scalePointY(points[i]);
			ytransform.set(p.getX(), p.getY());
		}
	}
	void draw() {
		fl_color(FL_BLACK);
		fl_line_style(FL_SOLID, 3, NULL);
		for (int i = 1; i < size(); i++) {
			Point p1 = xtransform[i - 1];
			Point p2 = xtransform[i];
			fl_line(p1.getX(), p1.getY(), p2.getX(), p2.getY());
		}
		fl_circle(100, 50, 2);
		fl_line(0, 0, 100, 100);
	}
};

class XYPlot {
	Fl_Double_Window* window;
	Graph* graph;
	pair<int, int> origin;
	pair<int, int> dimension;
	string caption;
public:
	XYPlot(int x, int y, int w, int h, string s) {
		origin.first = x;
		origin.second = y;
		dimension.first = w;
		dimension.second = h;
		caption = s;
	}
	void start() {
		window = new Fl_Double_Window(dimension.first, dimension.second, caption.data());
		graph = new Graph(origin.first, origin.second, dimension.first, dimension.second);
		window->resizable(graph);
	}
	void set(Points& p) {
		for (int i = 0; i < p.size(); i++) {
			Point pt = p[i];
			graph->add(pt);
		}
		graph->bounds();
		graph->scaleY();
		graph->scaleX();
	}
	int draw() {
		window->show();
		return(Fl::run());
	}
};