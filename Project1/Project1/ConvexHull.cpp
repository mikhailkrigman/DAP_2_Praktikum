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

	double operator- (const Point& other) const {
		return this->EuclidDistanceTo(other);
	}

	Point& operator= (const Point& other) {
		if (this == &other) return *this;

		this->_pos = other._pos;
		return *this;
	}

public:
	double EuclidDistanceTo(const Point& other) const {
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

	void pop_back() {
		_points.pop_back();
	}

public:
	vector<Point> get_points() const {
		return _points;
	}

	size_t dimension() const { // return dimension of stored points
		return (size() != 0) ? _points[0].size() : 0;
	}

	Point min_x(size_t& min_index) const {
		double min = _points[0][0];
		min_index = 0;

		for (int i = 0; i < size(); i++) {
			if ((_points[i][0] < min) || (_points[i][0] == min && _points[i][1] < _points[min_index][1])) {
				min = _points[i][0];
				min_index = i;
			}
		}

		return _points[min_index];
	}

private:
	vector<Point> _points;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class Line : private PointArray {
public:
	using PointArray::operator[];

public:
	Line(const Point& P1, const Point& P2) :PointArray({ P1,P2 }) { ; }

public:
	double Length() { 
		return ((*this)[0] - (*this)[1]); 
	}

	bool ThisfPointLeftOfMe(const Point& To) {
		// Nutzt Hessesche Normalenform der Geradengleichung aus. Siehe
		// http://de.wikipedia.org/wiki/Hessesche_Normalform#Abstand

		Point S0 = Point{ (*this)[0][0] - To[0],(*this)[0][1] - To[1] };
		Point S1 = Point{ (*this)[1][0] - To[0],(*this)[1][1] - To[1] };
		double ax = S1[0] - S0[0];
		double ay = S1[1] - S0[1];
		double d = ay * S0[0] - ax * S0[1];

		return d <= 0.0; // if point is left of line or collinear!
	}

	int get_orientation(const Point& point) {
		// determines the orientation of way (*this)[0] --> (*this)[1] --> point
		
		//  1 == clockwise == point is right of vector <(*this)[0], (*this)[1]> or it's prolongation
		// -1 == counterclockwise == point is left of vector <(*this)[0], (*this)[1]> or it's prolongation
		//  0 == all three paoints are on the same line

		double orientation = ((*this)[1][1] - (*this)[0][1]) * (point[0] - (*this)[1][0]) - ((*this)[1][0] - (*this)[0][0]) * (point[1] - (*this)[1][1]);
		
		return (int)(orientation / abs(orientation));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------
PointArray sort_x(PointArray arr) {
	for (int i = 1; i < arr.size(); i++)
	{
		Point key = arr[i];
		int j = i - 1;

		while (j >= 0 && arr[j][0] > key[0])
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}

	return arr;
}
//----------------------------------------------------------------------------------------------------------------------------------------
PointArray sort_y(PointArray arr) {
	for (int i = 1; i < arr.size(); i++)
	{
		Point key = arr[i];
		int j = i - 1;

		while (j >= 0 && arr[j][1] > key[1])
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}

	return arr;
}
//----------------------------------------------------------------------------------------------------------------------------------------
size_t get_index_of_the_leftmost_point(const PointArray& AllPoints, const size_t relative_point_index) {
	size_t the_leftmost_point_index = (relative_point_index + 1) % AllPoints.size();

	for (int i = 0; i < AllPoints.size(); i++) {
		Line next_segment(AllPoints[relative_point_index], AllPoints[the_leftmost_point_index]);

		// if point[i] "more left", than point[next_index] relative to point[current_index]
		if (next_segment.get_orientation(AllPoints[i]) < 0) {
			the_leftmost_point_index = i;
		}
		// if point[i], point[next_index] and point[current_index] are on the same line
		else if (next_segment.get_orientation(AllPoints[i]) == 0) {
			Line alternative_next_segment(AllPoints[relative_point_index], AllPoints[i]);

			if (next_segment.Length() < alternative_next_segment.Length()) {
				the_leftmost_point_index = i;
			}
		}
	}

	return the_leftmost_point_index;
}
//----------------------------------------------------------------------------------------------------------------------------------------
PointArray CalculateHull(const PointArray & AllPoints) {
	if (AllPoints.size() < 3) throw "Convex Hull can be made with at least 3 points!";

	size_t index_of_min_x;
	Point p0 = AllPoints.min_x(index_of_min_x);

	PointArray convex_hull({AllPoints[index_of_min_x]});

	size_t current_index = index_of_min_x;
	size_t next_index;

	do {
		next_index = get_index_of_the_leftmost_point(AllPoints, current_index);
		convex_hull.push_back(AllPoints[next_index]);

		current_index = next_index;
	} while (current_index != index_of_min_x);

	convex_hull.pop_back(); // the starting point has been appended twice: by initialisation and by the last iteration of do-while loop

	return convex_hull;
}

int main() {
	PointArray arr({ Point{0, 0} });
	for int 
}