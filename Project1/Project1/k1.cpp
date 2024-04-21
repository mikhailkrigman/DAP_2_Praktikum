#include <cstdlib> 
#include <cstring> 
#include <assert.h> 
#include <ctime>
#include <chrono>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>

using namespace std;

const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// --- GLOBAL CONSTANTS ---
const int ALPHABET_SIZE = 26;

// ABCDEFGHIJKLMNOPQRSTUVWXYZ 
const char* INITIAL_CRYPT_TABLE = "JPGVOUMFYQBENHZRDKASXLICTW";

// ABCDEFGHIJKLMNOPQRSTUVWXYZ 
const char* INITIAL_DECRYPT_TABLE = "SKXQLHCNWARVGMEBJPTYFDZUIO";

//------------------------------------------------------------------------------------------------------------------------
bool validate_input_string(char* input) {
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] < 'A' || input[i] > 'Z')
			return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------------------
void rotate_table(string& current_crypt_table, int rotation_step) {

	rotation_step = abs((ALPHABET_SIZE + rotation_step)) % ALPHABET_SIZE; // represent right rotation as left

	// using std::rotate from <algorithm>
	// See https://en.cppreference.com/w/cpp/algorithm/rotate
	rotate(current_crypt_table.begin(), current_crypt_table.begin() + rotation_step, current_crypt_table.end());
}
//------------------------------------------------------------------------------------------------------------------------
char* crypt(const char* input, bool encrypt, int initial_rotation, int rotation_step)
{
	char* output;
	try { output = new char[strlen(input)]; }
	catch (...) { "No memory to crypt the input string!"; exit(1); }

	string current_crypt_table(INITIAL_CRYPT_TABLE);
	if (!encrypt) {
		rotate_table(current_crypt_table, initial_rotation);
		for (int i = 0; i < strlen(input); i++) {
			output[i] = current_crypt_table[input[i] - 'A'];
			rotate_table(current_crypt_table, rotation_step);
		}
		output[strlen(input)] = '\0';
	}
	else {
		rotate_table(current_crypt_table, initial_rotation);
		for (int i = 0; i < strlen(input) - 1; i++) {
			rotate_table(current_crypt_table, rotation_step);
		}

		for (int i = strlen(input) - 1; i >= 0 ; i--) {
			int j = 0;
			while (current_crypt_table[j] != input[i]) {
				j++;
			}
			output[i] = ALPHABET[j];
			rotate_table(current_crypt_table, -rotation_step);
		}
		output[strlen(input)] = '\0';
	}

	return output;
}
//------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	char* input = nullptr;

	int initial_rotation = 0;				// -i
	int rotation_step = 0;					// -r
	bool encrypt = false;					// -d
	bool measure_processing_time = false;	// -t

	if (argc == 1) {
		cout << "Usage: k1 [ -i <int Rotation> ] [ -i <int InitialRotation> ] [ -t ] [-d ] <string Input>" << endl
			<< "\nSimple Encryption\n" << endl << "Options:" << endl
			<< setw(3) << "-i " << setw(17) << left << "InitialRotation: " << "initial rotation the codebook" << endl
			<< setw(3) << "-r " << setw(17) << left << "Rotation: " << "rotation of codebook after every Letter" << endl
			<< setw(3) << "-d " << setw(17) << "" << "decrypt (instead of encrypt)" << endl
			<< setw(3) << "-t " << setw(17) << "" << "Calculate processing time" << endl
			<< "\nOptions may occure in any order\n" << endl;

		exit(1);
	}
	else {
		try {
			for (int i = 1; i < argc; i++) {
				if (!string(argv[i]).compare("-i")) {
					if (++i == argc)
						throw "-i Must be followed by some integer!";
					if (!(istringstream(argv[i]) >> dec >> initial_rotation))
						throw "Malformed number for Initial Rotation.";
					continue;
				}

				if (!string(argv[i]).compare("-r")) {
					if (++i == argc)
						throw "-r Must be followed by some integer!";
					if (!(istringstream(argv[i]) >> dec >> rotation_step))
						throw "Malformed number for Rotation.";
					continue;
				}

				if (!string(argv[i]).compare("-d")) {
					encrypt = true;
					continue;
				}

				if (!string(argv[i]).compare("-t")) {
					measure_processing_time = true;
					continue;
				}

				input = argv[i];
			}

			if (input == nullptr)
				throw "Missing input string!";
			if (!validate_input_string(input))
				throw "Invalid Chars in Input. Use only A-Z.";

			if (measure_processing_time) {
				auto start = std::chrono::high_resolution_clock::now();

				char* output = crypt(input, encrypt, initial_rotation, rotation_step);
				
				auto stop = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

				auto processing_time = duration.count();

				cout << output << endl
					<< "Calculated processing time: " << processing_time << "microseconds" << endl;
			}
			else
				cout << crypt(input, encrypt, initial_rotation, rotation_step) << endl;
		}
		catch (const char* what) { cout << what << endl; exit(1); }
	}

	return 0;
}