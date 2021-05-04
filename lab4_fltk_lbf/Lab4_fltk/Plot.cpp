
//Taras Priadka
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
#include <memory>

using namespace std;

// ------------------------- ** POINT ** --------------------------

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

//compare objects for min, max function in points
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
	vector<Point>::iterator front() { return points.begin(); }
	vector<Point>::iterator end() { return points.end(); }
	Point get(int index) { return points[index]; }
	Point operator [] (int index) { return get(index); }
	void set(Point p) { points.push_back(p); }
	void set(double x, double y) {
		Point p(x, y);
		points.push_back(p);
	}
	double maxX() { return max_element(points.begin(), points.end(), PointCompareX())->getX(); }

	double minX() { return min_element(points.begin(), points.end(), PointCompareX())->getX(); }

	double maxY() { return max_element(points.begin(), points.end(), PointCompareY())->getY(); }

	double minY() { return min_element(points.begin(), points.end(), PointCompareY())->getY(); }
};

// ------------------------- ** File Processing ** --------------------------

class CSVReader {
public:
	static Points parseFile(std::string filename);
	static vector<string> split(string s, string del);
};

Points CSVReader::parseFile(std::string filename) {
	Points points;
	try {
		ifstream in(filename);
		if (in.is_open()) {
			string line;
			while (!in.eof()) {
				getline(in, line);
				vector<string> vs = CSVReader::split(line, string(","));
				if (vs.size() > 0) {
					double n1 = stod(vs[0], NULL);
					double n2 = stod(vs[1], NULL);
					points.set(n1, n2);
				}
			}
			in.close();
		} else {
			cout << "Unable to open file " << filename << endl;
			exit(0);
		}
	} catch (ifstream::failure e) {
		cout << e.what() << endl;
		exit(0);
	}
	return points;
}

vector<string> CSVReader::split(string s, string del) {
	if (s.size() <= 0)
		return vector<string>{};
	vector<string> splitText;
	std::regex rgx(del);
	std::sregex_token_iterator iter(s.begin(),
		s.end(),
		rgx,
		-1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter)
		splitText.push_back(*iter);
	return splitText;
}

// ------------------------- ** Calculations ** --------------------------
class LinearRegression {
public:
	static pair<Point, Point> fitLine(Points points) {
		double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_xy = 0;
		int n = points.size();
		for_each(points.front(), points.end(), [&](Point p) {
			cout << p.getX() << " " << p.getY() << endl;
			sum_x += p.getX();
			sum_y += p.getY();
			sum_x2 += p.getX() * p.getX();
			sum_xy += p.getX() * p.getY();
			});

		double b1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
		double b0 = (sum_y - b1 * sum_x) / n;

		//two points for the Linear Regression line
		Point p0 = Point(0, b0);
		Point p1 = Point(points.maxX(), b1 * points.maxX() + b0);
		cout << "y = " << b1 << "X + " << b0 << endl;
		return pair<Point, Point>(p0, p1);
	}
};

// ------------------------- ** Graph ** --------------------------
class Graph : public Fl_Widget {
	Points points;
	Points xtransform;
	Points ytransform;
	pair<double, double> axisrange;
	pair<double, double> minmaxX;
	pair<double, double> minmaxY;
	double border = 10;
	int CIRCLE_SIZE = 2;
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

	void scaleX() { // assumes evenly spaced X intervals	
		int index = 0;
		double interval = floor((double)w() / (points.size()));
		for (double i = interval; i < w() + interval; i += interval) {
			Point p = scalePointX(index++, i);
			xtransform.set(p);
		}
	}

	Point scalePointY(Point point) {
		//move all of the points so that the top circle isn't out of the screen
		Point py(point.getX(), ((1.0 - (((point.getY() - minmaxY.first))) / axisrange.second) * h()) + (double)CIRCLE_SIZE * 2);
		return py;
	}

	void scaleY() {
		for (int i = 0; i < points.size(); i++) {
			Point p = scalePointY(points[i]);
			ytransform.set(p.getX(), p.getY());
		}
	}

	void draw() {
		fl_color(FL_RED);
		fl_line_style(FL_SOLID, 3, NULL);

		for (int i = 0; i < size(); i++) {
			Point p1 = xtransform[i];
			fl_circle(p1.getX(), p1.getY(), CIRCLE_SIZE);
		}

		//linear regression line
		fl_color(FL_BLACK);
		fl_line_style(FL_SOLID, 3, NULL);
		auto fitLine = LinearRegression::fitLine(xtransform);
		fl_line(fitLine.first.getX(), fitLine.first.getY(), fitLine.second.getX(), fitLine.second.getY());
	}
};

class XYPlot {
	shared_ptr<Fl_Double_Window> window;
	shared_ptr<Graph> graph;
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
		window = move(shared_ptr<Fl_Double_Window>(new Fl_Double_Window(dimension.first, dimension.second, caption.data())));
		graph = move(shared_ptr<Graph>(new Graph(origin.first, origin.second, dimension.first, dimension.second)));
		window->resizable(graph.get());
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

// ------------------------- ** MAIN ** --------------------------

int main() {
	int width = 900;
	int height = 700;
	string caption = "XY Plot";
	Points points = CSVReader::parseFile("scatter1.csv");
	XYPlot plot(0, 0, width, height, caption);
	plot.start();
	plot.set(points);
	plot.draw();
	return 0;
}