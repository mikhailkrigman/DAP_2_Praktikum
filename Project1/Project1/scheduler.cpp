#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------------------------
class JobSpecification {
public:
	int first_time;
	int second_time;

public:
	JobSpecification() = default;
	JobSpecification(int first, int second) {
		first_time = first;
		second_time = second;
	}

public:
	bool operator< (const JobSpecification& other) const {
		return this->second_time < other.second_time;
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------
class JobVector : private vector<JobSpecification> {
public:
	using vector<JobSpecification>::size;
	using vector<JobSpecification>::push_back;
	using vector<JobSpecification>::begin;
	using vector<JobSpecification>::end;

public:
	JobVector() { ; }

	JobVector(char* filename) {
		ifstream file(filename);
		string line;
		while (std::getline(file, line, '\n')) {
			if (line.empty()) continue;

			istringstream buffer(line);
			buffer.exceptions(istringstream::failbit | istringstream::badbit);

			try {
				int first_time, second_time;

				if (!(buffer >> dec >> first_time)) throw "First time value must be a number!";
				buffer.ignore(1, ',');
				if (!(buffer >> dec >> second_time)) throw "Second time value must be a number!";

				JobSpecification new_job(first_time, second_time);
				push_back(new_job);
			}
			catch (const char* what) { throw what; }
			catch (...) { throw "UNDEFINED ERROR WHILE PARSING FILE"; }
		}
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------
JobVector IntervalScheduling(JobVector& Jobs) {
	sort(Jobs.begin(), Jobs.end());
}
//----------------------------------------------------------------------------------------------------------------------------------------
JobVector LatenessScheduling(JobVector& Jobs) {
	sort(Jobs.begin(), Jobs.end());
}