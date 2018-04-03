#include "Instruction.h"
#include "Simulator.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {	
	// check if program has the right amount of command line arguments
	if(argc != 3) {
		return 1; // return 1 to indicate error
	}

	// handle input from file
	ifstream input(argv[1]); // open input file stream to read input

	string line; // string to store lines from input file
	getline(input, line);
	
	vector<Instruction> trace; // vector to store all the instructions from input file
	while(line != "") { // last line of input is "" so stop there
		trace.push_back(Instruction(line)); // make an Instruction object and put in vector
		getline(input, line); // get the next line
	}
	input.close(); // close the input file stream once done reading

	// calculate and write to file
	ofstream output(argv[2]); // open output file stream to store ouput
	
	Simulator cacheSim = Simulator(&trace); // use ptr to save time, dont want to copy and pass entire vector

	// Direct-Mapped Cache
	output << cacheSim.directMapped(1) << ' ';
	output << cacheSim.directMapped(4) << ' ';
	output << cacheSim.directMapped(16) << ' ';
	output << cacheSim.directMapped(32) << endl;

	// ​​Set-Associative​​ ​​Cache
	output << cacheSim.setAssociative(2) << ' ';
	output << cacheSim.setAssociative(4) << ' ';
	output << cacheSim.setAssociative(8) << ' ';
	output << cacheSim.setAssociative(16) << endl;

	// ​​Fully-Associative​​ ​Cache
	output << cacheSim.fullyAssocLRU() << endl;
	output << cacheSim.fullyAssocHotCold() << endl;

	// Set-Associative Cache with no Allocation on a Write Miss
	output << cacheSim.setAssocNoAlloc(2) << ' ';
	output << cacheSim.setAssocNoAlloc(4) << ' ';
	output << cacheSim.setAssocNoAlloc(8) << ' ';
	output << cacheSim.setAssocNoAlloc(16) << endl;

	// Set-Associative Cache with Next-line Prefetching
	output << cacheSim.setAssocFetchNextLine(2) << ' ';
	output << cacheSim.setAssocFetchNextLine(4) << ' ';
	output << cacheSim.setAssocFetchNextLine(8) << ' ';
	output << cacheSim.setAssocFetchNextLine(16) << endl;

	// Set-Associative Cache with Next-line Prefetching-on-a-Miss
	output << cacheSim.setAssocFetchMiss(2) << ' ';
	output << cacheSim.setAssocFetchMiss(4) << ' ';
	output << cacheSim.setAssocFetchMiss(8) << ' ';
	output << cacheSim.setAssocFetchMiss(16) << endl;

	output.close(); // close output file stream once done writing

	return 0; // return 0 to indicate program executed correctly
}
