#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <assert.h>
#include <iomanip> 
#include <vector>
#include <algorithm>
#include <chrono>
#include <initializer_list>

using namespace std;

class JobSpecification {
private:
	int FirstTime;
	int SecondTime;
public:
	JobSpecification(int start_time, int end_time) {
		if (start_time < 0) throw "Job Start Time must be positive integer or 0.";
		if (end_time <= 0) throw "Job End Time must be positive integer.";
		if (start_time > end_time) throw "End of Job prior to Start of Job.";
		if (start_time == end_time) throw "End of Job at the same time as Start of Job.";

		FirstTime = start_time;
		SecondTime = end_time;
	}

public:
	int get_first_time() const {
		return FirstTime;
	}

	int get_second_time() const {
		return SecondTime;
	}

	bool operator< (JobSpecification other) {
		return this->SecondTime < other.SecondTime;
	}
};
//-----------------------------------------------------------------------------------------------
class JobVector : private vector<JobSpecification> {
public:
	using vector<JobSpecification>::size;
	using vector<JobSpecification>::erase;
	using vector<JobSpecification>::begin;
	using vector<JobSpecification>::end;
	using vector<JobSpecification>::operator[];

public:
	JobVector() {}
	JobVector(string Filename) {
		try {
			ifstream file(Filename);
			if (file.fail()) {
				throw "File not found.";
			}

			string line;
			while (std::getline(file, line, '\n')) {
				if (line.empty()) continue;

				istringstream buffer;
				try {
					buffer = istringstream(line);
				}
				catch (...) {
					throw "No memory.";
				}
				buffer.exceptions(istringstream::failbit | istringstream::badbit);

				int first_time, second_time;
				try {
					// parse first time
					buffer >> dec >> first_time;
					// skip all characters between time values
					// using numeric_limits allows to check whole stream until ',' or the end of line not found
					buffer.ignore(numeric_limits<streamsize>::max(), ',');
					// parse second time
					buffer >> dec >> second_time; // may be error due to spaces
				}
				catch (...) {
					throw "Garbage in file.";
				}

				JobSpecification new_job(first_time, second_time);
				push_back(new_job);
			}

			if (size() == 0) throw "Nothing usable in file.";
		}
		catch (const char* what) { throw what; }
	}

public:
	void push_back(JobSpecification& job) {
		try {
			vector<JobSpecification>::push_back(job);
		}
		catch (...) {
			throw "No memory.";
		}
	}

	friend ostream& operator<< (ostream& os, const JobVector& jobs) {
		os << "List of " << jobs.size() << " Jobs is:";
		for (size_t i = 0; i < jobs.size(); i++) {
			os << endl << "Job: " << i << " is from " << jobs[i].get_first_time()
				<< " to " << jobs[i].get_second_time();
		}
		return os;
	}

};

//-----------------------------------------------------------------------------------------------
JobVector IntervalScheduling(JobVector Jobs) {
	sort(Jobs.begin(), Jobs.end());
	return JobVector();
}
//-----------------------------------------------------------------------------------------------
JobVector LatenessScheduling(JobVector Jobs) {
	sort(Jobs.begin(), Jobs.end());
	return JobVector();
}
//-----------------------------------------------------------------------------------------------
int main(int argc, char* argv) {
	string filenames[] = { "datenBsp1.zahlen", "Err1", "Err2", "Err3", "Err4", "Err5", "Err6" };
	for (auto filename : filenames) {
		try {
			string filepath = "C:/Users/smmikrig/source/repos/Scheduler/Debug/" + filename;
			JobVector jobs(filepath);
			cout << jobs << endl;
		}
		catch (const char* what) {
			cout << filename << endl << what << endl;
		}
	}

	return 0;
}
