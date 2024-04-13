#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <cstring>
#include<initializer_list>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------------------------
class Point {
public:
	Point(initializer_list<double> args) {
		for (auto iter = args.begin(); iter != args.end(); ++iter)
			push_back(*iter);
	}

public:
	void push_back(double x) { 
		_pos.push_back(x); 
	}

	size_t size() const { 
		return _pos.size(); 
	}

	double& operator[] (const size_t i) { 
		return _pos[i]; 
	}

	const double operator[] (const size_t i) const { 
		return _pos[i]; 
	}

public:
	double EuclidDistanceTo(const Point& other) {
		double square_dist = 0;

		for (int i = 0; i < size(); i++) {
			square_dist += pow(other[i] - _pos[i], 2);
		}

		return sqrt(square_dist);
	}

private:
	vector <double> _pos;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class PointArray {
public:
	PointArray(initializer_list<Point> args) {
		for (auto iter = args.begin(); iter != args.end(); ++iter) {
			push_back(*iter);
		}
	}

public: // redefined vector methods
	size_t size() const { 
		return _points.size(); 
	}

	Point& operator[] (const size_t i) { 
		return _points[i]; 
	}

	const Point operator[] (const size_t i) const { 
		return _points[i]; 
	}

	auto begin() {
		return _points.begin();
	}

	auto end() {
		return _points.end();
	}

	void push_back(Point p) {
		if (p.size() == dimension() || dimension() == 0) {
			_points.push_back(p);
		}
		else {
			throw "Points must be same dimension!";
		}
	}

public:
	vector<Point> get_points() const {
		return _points;
	}

	size_t dimension() const { // return dimension of stored points
		return (size() != 0) ? _points[0].size() : 0;
	}

private:
	vector<Point> _points;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class Simplex : private PointArray {
public:
	using PointArray::size;
	using PointArray::operator[];
	using PointArray::begin;
	using PointArray::end;

	Simplex(const PointArray& ThePointArray) : PointArray(ThePointArray) {
		if (ThePointArray.size() - ThePointArray.dimension() != 1) {
			throw "Object of Simplex-Class must have n+1 points of dimension n!";
		}
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------
class Triangle : public Simplex {
public:
	Triangle(const Simplex& TheSimplex) :Simplex(TheSimplex) {
		if (TheSimplex.size() != 3) {
			throw "Object of Triangle-Class must have exactly 3 points!";
		}
	}

public:
	double Girth() {
		Point a = (*this)[0];
		Point b = (*this)[1];
		Point c = (*this)[2];

		return a.EuclidDistanceTo(b) + a.EuclidDistanceTo(c) + b.EuclidDistanceTo(c);
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------
int dr(int argc, char* argv[]) {
	srand(time(0));

	try {
		double x1, y1, x2, y2, x3, y3;
		if (argc == 1) {
			x1 = (double)rand() / RAND_MAX;
			y1 = (double)rand() / RAND_MAX;
			x2 = (double)rand() / RAND_MAX;
			y2 = (double)rand() / RAND_MAX;
			x3 = (double)rand() / RAND_MAX;
			y3 = (double)rand() / RAND_MAX;
		}
		else if (argc == 7) {
			if (!(istringstream(argv[1]) >> x1)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> y1)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> x2)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> y2)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> x3)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> y3)) throw "Input value must be a number!";
		}
		else {
			throw "Usage: dr [ x1 y1 x2 y2 x3 y3 ] or dr";
		}

		Point a{ x1, y1 };
		Point b{ x2, y2 };
		Point c{ x3, y3 };

		Triangle t{ PointArray{a, b, c} };

		cout << t.Girth() << endl;
	}
	
	catch (const char* e) {
		cout << e;
	}

	return 0;
}