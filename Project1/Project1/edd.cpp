#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <vector> 
#include <algorithm>
using namespace std;


class Transformation {
private:
	size_t _cost = 0;

	stringstream transformation_steps;

public: 
	Transformation(const string &From, const string &To) {
		int rows = From.length() + 1, cols = To.length() + 1;
		CostOperation** matrix;

		// allocate memory
		try {
			matrix = new CostOperation*[rows];
		}
		catch (...) {
			throw "No Memory.";
		}

		if (rows) {
			try {
				matrix[0] = new CostOperation[rows * cols];
			}
			catch (...) {
				delete[] matrix;
				throw "No Memory.";
			}

			for (int i = 1; i < rows; ++i)
				matrix[i] = matrix[0] + i * cols;
		}

		// algorithm implementation
		// (1) calculate length
		for (size_t i = 1; i < rows; i++) {
			matrix[i][0].cost = i;
			matrix[i][0].operation = "delete";
			matrix[i][0].position = i - 1;
			matrix[i][0].before = From[i - 1];
			matrix[i][0].after = '\NUL';
		}
		for (size_t j = 1; j < cols; j++) {
			matrix[0][j].cost = j;
			matrix[0][j].operation = "insert";
			matrix[0][j].position = j - 1;
			matrix[0][j].before = '\NUL';
			matrix[0][j].after = To[j - 1];
		}

		for (size_t i = 1; i < rows; i++) {
			for (size_t j = 1; j < cols; j++) {
				if (From[i - 1] == To[j - 1]) {
					matrix[i][j] = matrix[i - 1][j - 1];
				}


				const int* min_cell = &min(matrix[i - 1][j - 1].cost, matrix[i][j - 1].cost, matrix[i - 1][j].cost);
				matrix[i][j].cost += *min_cell + 1;
				
				if (min_cell == &(matrix[i - 1][j - 1].cost)) {
					// Exchange From[i - 1] -> To[j - 1]
					matrix[i][j].operation += "exchange";
				}
				else if (min_cell == &(matrix[i][j - 1].cost)){
					// Insertion Position i To[j]
					cout << "insert " << To[j - 1] << " at position " << i - 1 << endl;
				}
				else if (min_cell == &(matrix[i - 1][j].cost)) {
					// Deletion Position i
					cout << "Delete " << From[i - 1] << " at position " << i - 1 << endl;
				}
			}
		}

		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < cols; j++) {
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}


		string Result;
		size_t lcs_length = static_cast<size_t>(matrix[rows - 1][cols - 1]);
		try { Result.reserve(lcs_length); }
		catch (...) {
			if (rows) delete[] matrix[0];
			delete[] matrix;
			throw "No Memory";
		}

		// free memory
		if (rows) delete[] matrix[0];
		delete[] matrix;
	}

private:
	const int& min(const int& a, const int& b, const int& c) {
		return std::min(std::min(a, b), c);
	}
	struct CostOperation {
		int cost = 0;
		string operation = "do nothing";
		int position = 0;
		char before = '\NUL';
		char after = '\NUL';
	};

public: 
	size_t Cost() {
		return _cost;
	}

	friend ostream& operator<< (ostream &TheOstream, Transformation &me) {

		/* Implemet your Solution here! */

		throw ("Long output not implemented yet");

		return TheOstream;
	};

		/* You must not edit below this line */
};


void usage() { throw "Usage: edd String1 String2 [ -o ]"; }

int main(int argc, char* argv[]) {

	bool LongOutput = false; // If true the complete edit steps are printed line by line

	try {
		if (argc<3 || argc>4) usage();

		if (argc == 4) {
			string arg = string(argv[3]);
			if (!arg.compare("-o")) LongOutput = true; else usage();
		}

		Transformation Try = Transformation(string(argv[1]), string(argv[2])); // Calculate edit steps
		cout << "Transformation from " << argv[1] << " to " << argv[2] << " takes " << Try.Cost() << " Operations" << endl;
		if (LongOutput) cout << endl << Try << endl;// optional long output

	}
	catch (const char *Reason) { cerr << Reason << endl; exit(1); }

	return 0;
}
