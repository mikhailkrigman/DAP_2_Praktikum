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
		catch (...) {
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
		return this->EuclidDistanceTo(other);
	}

	Point& operator= (const Point& other) {
		if (this == &other) return *this;

		try {
			this->coords = other.coords; // other point may have bigger dimension
		}
		catch (...) {
			throw "No Memory";
		}
		return *this;
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

	void pop_back() {
		points.pop_back();
	}

public:
	Point min_x(size_t& min_index) const {
		// Find point with the smallest x - coordinate
		// and save it's index into min_index

		double min = points[0][0];
		min_index = 0;

		for (size_t i = 0; i < size(); i++) {
			if ((points[i][0] < min) || (points[i][0] == min && points[i][1] < points[min_index][1])) {
				min = points[i][0];
				min_index = i;
			}
		}

		return points[min_index];
	}
};

class Line : private PointArray {
public:
	using PointArray::operator[];

public:
	Line(const Point& P1, const Point& P2): PointArray({ P1,P2 }) { ; }

public:
	double Length() {
		return ((*this)[0] - (*this)[1]);
	}

	int get_orientation(const Point& point) {
		// determines the orientation of way (*this)[0] --> (*this)[1] --> point

		//  1 == clockwise == point is right of vector <(*this)[0], (*this)[1]> or it's prolongation
		// -1 == counterclockwise == point is left of vector <(*this)[0], (*this)[1]> or it's prolongation
		//  0 == all three paoints are on the same line

		// see https://www.geeksforgeeks.org/orientation-3-ordered-points/
		double orientation = ((*this)[1][1] - (*this)[0][1]) * (point[0] - (*this)[1][0])
			- ((*this)[1][0] - (*this)[0][0]) * (point[1] - (*this)[1][1]);

		return (int)(orientation / abs(orientation));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------
size_t get_index_of_the_leftmost_point(const PointArray& all_points, const size_t relative_point_index) {
	size_t the_leftmost_point_index = (relative_point_index + 1) % all_points.size();

	for (size_t i = 0; i < all_points.size(); i++) {
		Line next_segment(all_points[relative_point_index], all_points[the_leftmost_point_index]);

		// if point[i] "more left", than point[next_index] relative to point[current_index]
		if (next_segment.get_orientation(all_points[i]) < 0) {
			the_leftmost_point_index = i;
		}

		// if point[i], point[next_index] and point[current_index] are on the same line
		else if (next_segment.get_orientation(all_points[i]) == 0) {
			Line alternative_next_segment(all_points[relative_point_index], all_points[i]);

			if (next_segment.Length() < alternative_next_segment.Length()) {
				the_leftmost_point_index = i;
			}
		}

		// eventually add check for duplicate point
	}

	return the_leftmost_point_index;
}
//----------------------------------------------------------------------------------------------------------------------------------------
PointArray CalculateHull(const PointArray & AllPoints) {
	if (AllPoints.size() < 3)
		throw "Convex Hull can be made with at least 3 points!";

	size_t index_of_min_x;
	Point p0 = AllPoints.min_x(index_of_min_x);

	PointArray convex_hull({ AllPoints[index_of_min_x] });

	size_t current_index = index_of_min_x;
	size_t next_index;

	do {
		next_index = get_index_of_the_leftmost_point(AllPoints, current_index);
		convex_hull.push_back(AllPoints[next_index]);

		current_index = next_index;
	} while (current_index != index_of_min_x);

	convex_hull.pop_back(); // the starting point has been appended twice: 
							// by initialisation and by the last iteration of do-while loop

	return convex_hull;
}

int main(int argc, char* argv[]) {
	srand(time(0));
	try {
		
	}

	catch (const char* what) {
		cerr << what << endl;
		exit(1);
	}
}
