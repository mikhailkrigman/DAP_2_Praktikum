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

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------------=
template <typename T>
bool is_sorted(vector<T> a)
{
	for (size_t i = 0; i < a.size() - 1; i++)
	{
		if (a[i] > a[i + 1])
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------=
template <typename T>
vector<T> BubbleSort(vector<T> a, long long& exec_time) {
	assert(a.size() > 0);

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = a.size() - 1; i > 0; i--)
		for (size_t j = 0; j < i; j++)
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	exec_time = duration.count();

	return a;
}
//---------------------------------------------------------------------------------------------------------------------------------------=
template <typename T>
vector<T> InsertionSort(vector<T> a, long long& exec_time)
{
	assert(a.size() > 0);

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i < a.size(); i++)
	{
		double key = a[i];
		int j = i - 1;

		while (j >= 0 && a[j] > key)
		{
			a[j + 1] = a[j];
			j = j - 1;
		}
		a[j + 1] = key;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	exec_time = duration.count();

	return a;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void merge()
{ // merge two sorted arrays in one sorted array

}
//---------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
vector<T> MergeSort(vector<T> a, long long& exec_time)
{
	assert(a.size() > 0);

	auto start = std::chrono::high_resolution_clock::now();



	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	exec_time = duration.count();

	return a;
}
//---------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool IsSortedAndNothingIsLost(vector<T> Before, vector<T> After)
{
	if (Before.size() != After.size())
		return false;

	if (!is_sorted<T>(After))
		return false;

	for (int i = 0; i < After.size(); i++)
	{
		for (int j = 0; j < Before.size(); j++)
		{
			if (After[i] == Before[j])
				Before.erase(Before.begin() + j);
		}
	}

	if (Before.size() != 0)
		return false;

	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------
vector<double> GenerateNumbers(string Selection, size_t n)
{
	vector<double> new_array(n);

	if (Selection == "random")
	{
		for (int i = 0; i < n; i++)
		{
			new_array.push_back((double)rand() / RAND_MAX); // add random between 0 and 1
		}
	}
	else if (Selection == "up")
	{
		for (int i = 0; i < n; i++)
		{
			new_array.push_back(i);
		}
	}
	else if (Selection == "down")
	{
		for (int i = 0; i < n; i++)
		{
			new_array.push_back((-1) * i);
		}
	}
	else if (Selection == "constant")
	{
		for (int i = 0; i < n; i++)
		{
			new_array.push_back(17.0);
		}
	}
	else
	{ // error argument input
		throw "Undefined Selection";
	}

	return new_array;
}
//---------------------------------------------------------------------------------------------------------------------------------------
void parse_args(int argc, char* argv[],
	size_t& n, string& selection,
	bool& print_original, bool& measure_time,
	bool& check_sorting, bool& use_insertion_sort, bool& use_merge_sort)
{
	if (argc == 1) // if no args given
		throw "mysort n Direction [ -o ] [ -c ] [ -t ] [ -m | -i ]";


	istringstream(argv[1]) >> n;

	if (n <= 0)
		throw "First Parameter n must be positive integer";

	istringstream(argv[2]) >> selection;

	if (selection != "random" && selection != "up" &&
		selection != "down" && selection != "constant")
		throw "Second Parameter must be chosen from random|up|down|constant";

	for (int i = 0; i < argc; i++)
	{
		string arg = string(argv[i]);

		if (arg == "-o") print_original = true;
		else if (arg == "-t") measure_time = true;
		else if (arg == "-c") check_sorting = true;
		else if (arg == "-i") use_insertion_sort = true;
		else if (arg == "-m") use_merge_sort = true;
		else throw "Wrong Switch, use only one of -o|-c|-t|-m|-i";
	}

	if (use_insertion_sort && use_merge_sort) throw "It's forbidden to use -o and -i options at the same time";
}
//---------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	srand(time(0)); // initialize random numbers creator

	size_t n;
	string selection;
	long long sorting_time;

	bool print_original, measure_time, check_sorting, use_insertion_sort, use_merge_sort;

	try
	{
		parse_args(argc, argv, n, selection,
			print_original, measure_time, check_sorting, use_insertion_sort, use_merge_sort);
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

	vector<double> original_array = GenerateNumbers(selection, n);
	vector<double> sorted_array;

	if (use_insertion_sort)
	{
		sorted_array = InsertionSort<double>(original_array, sorting_time);
	}
	else if (use_merge_sort)
	{
		sorted_array = MergeSort<double>(original_array, sorting_time);
	}
	else
	{
		sorted_array = BubbleSort<double>(original_array, sorting_time);
	}

	if (check_sorting)
	{
		if (!IsSortedAndNothingIsLost<double>(original_array, sorted_array))
		{
			cout << "Check failed!" << endl;
			exit(1);
		}
		else
		{
			cout << "Success!" << endl;
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (print_original)
		{
			cout << original_array[i] << "\t";
		}

		cout << sorted_array[i] << endl;
	}

	if (measure_time)
		cout << "--- It took " << sorting_time << " microseconds to compute ---" << endl;


	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------