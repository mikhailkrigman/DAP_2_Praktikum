#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <assert.h>
#include <iomanip> 
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <initializer_list>

using namespace std;

class Point {
private:
	vector<double> coords;

public:
	Point(initializer_list<double> args) {
		for (auto iter = args.begin(); iter != args.end(); iter++) {
			push_back(*iter);
		}
	}

public: // overwritten vector methods
	void push_back(double x) {
		try {
			coords.push_back(x);
		}
		catch(...) {
			throw "No Memory";
		}
	}

	size_t size() const {
		return coords.size();
	}

	double& operator[] (const size_t i) {
		return coords[i];
	}

	const double operator[](const size_t i) const {
		return coords[i];
	}

	double operator- (const Point& other) const {
		return EuclidDistanceTo(other);
	}

public: // other methods
	double EuclidDistanceTo(const Point& Other) const {
		double square_dist = 0;
		for (size_t i = 0; i < size(); i++) {
			square_dist += pow(Other[i] - coords[i], 2);
		}

		return sqrt(square_dist);
	}

};

class PointArray {
private:
	vector<Point> points;

public:
	PointArray(initializer_list<Point> args) {
		for (auto iter = args.begin(); iter != args.end(); iter++) {
			push_back(*iter);
		}
	}

public:
	size_t dimension() const {
		return (points.size()) ? points[0].size() : 0;
	}

	size_t size() const {
		return points.size();
	}

	Point& operator[] (const size_t i) {
		return points[i];
	}

	const Point operator[](const size_t i) const {
		return points[i];
	}

	void push_back(Point p) {
		if (p.size() != dimension() && dimension() != 0)
			throw "Points must have the same dimension";

		try {
			points.push_back(p);
		}
		catch (...) {
			throw "No Memory";
		}
	}
};

class Simplex : private PointArray {
public:
	using PointArray::size;
	using PointArray::operator[];
private:
	using PointArray::dimension;
public:
	Simplex(const PointArray& ThePointArray) : PointArray(ThePointArray) { // call default copy-constructor for PointArray
		if (size() != dimension() + 1)
			throw "To create a Simplex object the amount of points has to be 1 more than there dimension!";
	}
};

class Triangle : public Simplex {
public:
	Triangle(const Simplex &TheSimplex) : Simplex(TheSimplex) {
		if (size() != 3)
			throw "Incorrrect Amount of Points to create a Triangle. Should be given 3.";
	}

public:
	double Girth() {
		Point a = (*this)[0];
		Point b = (*this)[1];
		Point c = (*this)[2];

		return a.EuclidDistanceTo(b) + a.EuclidDistanceTo(c) + b.EuclidDistanceTo(c);
	}
};

void test_classes() {
	Point Eins({ 1.1 });
	Point Zwei{1.1, 2.2};
	Point Drei = Point{ 1.1, 2.2, 3.3 };

	PointArray Beide = PointArray{ Zwei, {3.3, 4.4} };

	try {
		PointArray Feinde = PointArray{ Eins, Zwei };
	}
	catch (const char* what) { cout << what << endl; }

	Point ErsterPunkt = Beide[0];
	double VierKommaVier = Beide[1][1];

	Simplex DerGeht(PointArray{ Point{ 1,2 },Point{ 5,6 },Point{ 8,9 } });
	try {
		Simplex DerGehtNicht(PointArray{ Point{ 1,2 },Point{ 5,6 } });
	}
	catch (const char* what) { cout << what << endl; }

	Triangle Dreieck(DerGeht);
	try {
		Triangle ZweiEck(Beide); // Verhindern!
		PointArray DreiPunkteIm1DRaum =
			PointArray{ Point{ 1 },Point{ 2 },Point{ 3 } };
		Triangle Eindimensional(DreiPunkteIm1DRaum);
	}
	catch (const char* what) { cout << what << endl; }

	PointArray DreiPunkteIm2DRaum =
		PointArray{ Point{ 0, 0 },Point{ 0, 1 },Point{ 1, 0 } };
	Triangle Zweidimensional(DreiPunkteIm2DRaum);
	cout << Zweidimensional.Girth() << endl;

}

int main1(int argc, char* argv[]) {
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
			if (!(istringstream(argv[1]) >> dec >> x1)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> dec >> y1)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> dec >> x2)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> dec >> y2)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> dec >> x3)) throw "Input value must be a number!";
			if (!(istringstream(argv[1]) >> dec >> y3)) throw "Input value must be a number!";
		}
		else {
			throw "Usage: dr [ x1 y1 x2 y2 x3 y3 ]";
		}

		Point a{ x1, y1 };
		Point b{ x2, y2 };
		Point c{ x3, y3 };

		Triangle t{ PointArray{ a, b, c } };

		cout << t.Girth() << endl;
	}

	catch (const char* what) {
		cerr << what << endl;
		exit(1);
	}

	return 0;
}
