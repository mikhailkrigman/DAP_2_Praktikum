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
#include <limits.h>
#include <fstream>

typedef std::chrono::milliseconds time_precision;

using namespace std;

vector<double> GenerateNumbers(string Selection, size_t n) {
	vector<double> new_arr;
	try { new_arr.resize(n); }
	catch (...) { throw("No Memory."); }

	if (n != 0) {
		if (Selection == "random") {
			for (size_t i = 0; i < n; i++) {
				new_arr[i] = ((double)(rand()) / RAND_MAX);
			}
		}
		else if (Selection == "up") {
			for (size_t i = 0; i < n; i++) {
				new_arr[i] = ((double)(i));
			}
		}
		else if (Selection == "down") {
			new_arr[0] = 0; // double(0) != 0. it's very small number, so by mult with - 1 it returns -0
			for (size_t i = 1; i < n; i++) {
				new_arr[i] = (-(double)(i));
			}
		}
		else if (Selection == "constant") {
			for (size_t i = 0; i < n; i++) { // the same complexity as by using constructor vector<double> new_arr(n, 17);
				new_arr[i] = ((double)(17));
			}
		}
		else throw "Unexpected Selection choice";
	}
	else throw "The length of array must be positive integer.";

	return new_arr;
}
//--------------------------------------------------------------------------------------------------------------
template <typename T>
bool is_sorted(const vector<T>& a) {
	for (size_t i = 0; i < a.size() - 1; i++) {
		if (a[i] > a[i + 1])
			return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------
template<typename T>
void BubbleSort(vector<T>& a) {
	assert(a.size() > 0);
	for (size_t i = a.size() - 1; i > 0; i--)
		for (size_t j = 0; j < i; j++)
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);

	if (!is_sorted<T>(a)) throw "Sort failed!";
}
//--------------------------------------------------------------------------------------------------------------
template<typename T>
bool IsSortedAndNothingIsLost(vector <T>& Before, vector <T>& After) {
	if (Before.size() != After.size()) return false;

	// check if After contains same elements as Before
	vector<T> after_copy = After;
	for (size_t i = 0; i < Before.size(); i++) {
		for (size_t j = 0; j < after_copy.size(); j++) {
			if (after_copy[j] == Before[i]) {
				after_copy.erase(after_copy.begin() + j);
				break;
			}
		}
	}

	return after_copy.size() == 0 && is_sorted<T>(After);
}
//--------------------------------------------------------------------------------------------------------------
template <class T>
void merge(vector<T>& arr, size_t left, size_t mid, size_t right) {
	size_t n1 = mid - left + 1;
	size_t n2 = right - mid;

	vector<T> L(n1 + 1), R(n2 + 1); // memory check is missing

	for (size_t i = 0; i < n1; i++)
		L[i] = arr[left + i];

	for (size_t i = 0; i < n2; i++)
		R[i] = arr[mid + 1 + i];

	L[n1] = numeric_limits<T>::max();	// numeric limits sind scheisse, die lieber weiter vermeiden
	R[n2] = numeric_limits<T>::max();	// numeric limits sind scheisse, die lieber weiter vermeiden

	for (size_t i = 0, j = 0, k = left; k <= right; k++) {
		if (L[i] < R[j]) {
			arr[k] = L[i++];
		}
		else {
			arr[k] = R[j++];
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
template <class T>
void IterativeMergeSort(vector<T>& arr) {

	size_t curr_size;   // For current size of subarrays to be merged
						// curr_size varies from 1 to n/2
	size_t left_start;  // For picking starting index of left subarray
						// to be merged

						// Merge subarrays in bottom up manner.  First merge subarrays of
						// size 1 to create sorted subarrays of size 2, then merge subarrays
						// of size 2 to create sorted subarrays of size 4, and so on.
	for (curr_size = 1; curr_size <= arr.size() - 1; curr_size = 2 * curr_size)
	{
		// Pick starting point of different subarrays of current size
		for (left_start = 0; left_start < arr.size() - 1; left_start += 2 * curr_size)
		{
			// Find ending point of left subarray. mid+1 is starting 
			// point of right
			int mid = min(left_start + curr_size - 1, arr.size() - 1);

			int right_end = min(left_start + 2 * curr_size - 1, arr.size() - 1);

			// Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
			merge(arr, left_start, mid, right_end);
		}
	}

	/* My Solution

	// go throw array and merge it's different parts
	// step is multiply size of array parts by 2 after merging each of them
	size_t part_arr_size;
	for (part_arr_size = 2; part_arr_size <= arr.size(); part_arr_size *= 2) {
	// merge each part-array
	size_t left = 0, right = part_arr_size - 1;
	size_t mid = (left + right) / 2;
	while (left < arr.size() - 1) {
	merge(arr, left, mid, right);

	left += part_arr_size;
	right = min(left + part_arr_size - 1, arr.size() - 1);
	mid = (left + right) / 2;
	}
	}

	// merge two last arrays if original array size is not 2-pow
	merge(arr, 0, part_arr_size / 2 - 1, arr.size() - 1);

	*/
}
//--------------------------------------------------------------------------------------------------------------
template <typename T>
void QuickSortCore(vector<T>& arr, size_t left, size_t right) {
	if (left < right) {
		size_t i = left;
		size_t j = right;

		T pivot = arr[(left + right) / 2]; // int division is by default floor

		while (i <= j) {
			while (arr[i] < pivot)
				i++;

			while (arr[j] > pivot)
				j--;

			if (i <= j) {
				swap<T>(arr[i], arr[j]);
				i++;
				if (j > 0)
					j--;
			}
		}

		QuickSortCore<T>(arr, left, j);
		QuickSortCore<T>(arr, i, right);
	}
}
//--------------------------------------------------------------------------------------------------------------
template <typename T>
void QuickSort(vector<T>& a){
	assert(a.size() > 0);
	QuickSortCore<T>(a, 0, a.size() - 1);
}
//--------------------------------------------------------------------------------------------------------------
template <class T>
void InsertionSort(vector<T>& a) {
	assert(a.size() > 0);

	for (size_t i = 1; i < a.size(); i++)
	{
		double key = a[i];
		int j = (int)(i)-1;

		while (j >= 0 && a[j] > key)
		{
			a[j + 1] = a[j];
			j = j - 1;
		}
		a[j + 1] = key;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void parse_args(int argc, char* argv[], int& n, string& selection,
	bool& print_original, bool& measure_processing_time, bool& check_sort,
	bool& use_insertion_sort, bool& use_merge_sort, bool& use_quick_sort) {

	if (argc < 3) // if not enough args given
		throw "Usage: mysort n Direction [ -o ] [ -c ] [ -t ] [ -m | -i | -q ]";

	if (!(istringstream(argv[1]) >> dec >> n) || n < 1)
		throw "First Parameter n must be positive integer";

	selection = string(argv[2]);
	if (selection != "random" && selection != "up" &&
		selection != "down" && selection != "constant")
		throw "Second Parameter must be chosen from random|up|down|constant";

	for (int i = 3; i < argc; i++) {
		if (!string(argv[i]).compare("-m")) {
			use_insertion_sort = true;
			continue;
		}

		else if (!string(argv[i]).compare("-i")) {
			use_merge_sort = true;
			continue;
		}

		else if (!string(argv[i]).compare("-q")) {
			use_quick_sort = true;
			continue;
		}

		else if (!string(argv[i]).compare("-t")) {
			measure_processing_time = true;
			continue;
		}

		else if (!string(argv[i]).compare("-c")) {
			check_sort = true;
			continue;
		}

		else if (!string(argv[i]).compare("-o")) {
			print_original = true;
			continue;
		}
		else
			throw "Wrong Switch, use only one of -o|-c|-t|-m|-i";
	}


	// we need so many if-conditions to show correct 
	// error explanation in wrong case
	if (use_merge_sort && use_insertion_sort && use_quick_sort)
		throw "It's forbidden to use -m, -i and -q options at the same time";

	if (use_merge_sort && use_insertion_sort)
		throw "It's forbidden to use -m and -i options at the same time";

	if (use_insertion_sort && use_quick_sort)
		throw "It's forbidden to use -i and -q options at the same time";

	if (use_merge_sort && use_quick_sort)
		throw "It's forbidden to use -m and -q options at the same time";
}
//--------------------------------------------------------------------------------------------------------------
template<typename T>
vector<T> create_sorted(void(*sorting_algorithm)(vector<T>&), vector<T> to_sort_array) {
	sorting_algorithm(to_sort_array);
	return to_sort_array;
}
//--------------------------------------------------------------------------------------------------------------
template<typename T>
long long measure_sorting_time(void(*sorting_algorithm)(vector<T>&), vector<T>& to_sort_array) {
	auto start = std::chrono::high_resolution_clock::now();

	sorting_algorithm(to_sort_array);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<time_precision>(stop - start);

	return duration.count();
}


int main(int argc, char* argv[]) {
	srand(time(0));

	int n;  // make n as size_t is not the best way because while parsing 
			// negative numbers will be converted to the biggest positive (Number overflow)

	string selection;
	long long processing_time;

	bool print_original = false;            // -o
	bool measure_processing_time = false;   // -t
	bool check_sort = false;                // -c
	bool use_insertion_sort = false;        // -i
	bool use_merge_sort = false;            // -m
	bool use_quick_sort = false;			// -q

	try {
		parse_args(argc, argv, n, selection,
			print_original, measure_processing_time, check_sort,
			use_insertion_sort, use_merge_sort, use_quick_sort);
	}
	catch (const char* what) {
		cout << what << endl;
		exit(1);
	}

	vector<double> original_array;
	vector<double> working_array = GenerateNumbers(selection, size_t(n)); // after successful parsing it is possible to cast n to size_t

	if (check_sort || print_original) {
		original_array = working_array;
	}

	void(*sorting_algorithm)(vector<double>&);

	if (use_insertion_sort) {
		sorting_algorithm = InsertionSort<double>;
	}
	else if (use_merge_sort) {
		sorting_algorithm = InsertionSort<double>;
	}
	else if (use_quick_sort) {
		sorting_algorithm = QuickSort<double>;
	}
	else {
		sorting_algorithm = BubbleSort<double>;
	}

	processing_time = measure_sorting_time<double>(sorting_algorithm, working_array);

	if (check_sort) {
		if (!IsSortedAndNothingIsLost<double>(original_array, working_array)) {
			cout << "Check failed!" << endl;
			exit(1);
		}
		else {
			cout << "Success!" << endl;
		}
	}

	if (print_original) {
		for (size_t i = 0; i < size_t(n); i++) {
			cout << fixed << setprecision(12)
				<< original_array[i] << "\t"
				<< working_array[i] << endl;
		}
	}

	if (measure_processing_time)
		cout << "--- It took " << processing_time << " milliseconds to compute ---" << endl;

	return 0;
}

// Functions were used to create data for matlab plots
//--------------------------------------------------------------------------------------------------------------
template <typename T>
void save(string filename, const T& data) {
	ofstream file;
	file.open(filename, std::ios::app); // open file in append mode

	if (!file)
		throw "Error while file opening!";

	// write data to file
	file << data << endl;

	file.close();
}
//--------------------------------------------------------------------------------------------------------------
vector <double> linspace(double min, double max, size_t amount) {
	double dist = max - min;
	double step = 0;

	try {
		step = dist / (amount - 1);
	}
	catch (...) { throw ("Linspace vector can return at least 2 elements."); }


	vector<double> res;
	try { res.resize(amount); }
	catch (...) { throw("No Memory."); }

	res[0] = min;
	res[amount - 1] = max;

	for (size_t i = 1; i < amount - 1; i++) {
		res[i] = res[i - 1] + step;
	}

	return res;
}
//--------------------------------------------------------------------------------------------------------------
void run_and_save_data(void(*sorting_algorithm)(vector<double>&), string selection, int max_length, string filename) {
	vector<double> n = linspace(1, max_length, 15);
	for (size_t i = 0; i < n.size(); i++) {
		vector<double> working_array = GenerateNumbers(selection, static_cast<size_t>(n[i]));
		long long processing_time = measure_sorting_time<double>(sorting_algorithm, working_array);

		string data = to_string(static_cast<size_t>(n[i])) + ", " + to_string(processing_time);
		save<string>(filename, data);
	}
}
//--------------------------------------------------------------------------------------------------------------
void create_data_for_plots() {
	// all numbers were experimentally determined
	run_and_save_data(BubbleSort<double>, "random", 76000, "data_bs_random.txt");
	run_and_save_data(BubbleSort<double>, "up", 107000, "data_bs_up.txt");
	run_and_save_data(BubbleSort<double>, "down", 70000, "data_bs_down.txt");
	run_and_save_data(BubbleSort<double>, "constant", 107000, "data_bs_constant.txt");

	run_and_save_data(InsertionSort<double>, "random", 125000, "data_is_random.txt");
	run_and_save_data(InsertionSort<double>, "up", 1800000000, "data_is_up.txt");
	run_and_save_data(InsertionSort<double>, "down", 87000, "data_is_down.txt");
	run_and_save_data(InsertionSort<double>, "constant", 1800000000, "data_is_constant.txt");

	run_and_save_data(IterativeMergeSort<double>, "random", 13500000, "data_ms_random.txt");
	run_and_save_data(IterativeMergeSort<double>, "up", 14000000, "data_ms_up.txt");
	run_and_save_data(IterativeMergeSort<double>, "down", 13300000, "data_ms_down.txt");
	run_and_save_data(IterativeMergeSort<double>, "constant", 14000000, "data_ms_constant.txt");
}
