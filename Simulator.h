#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Instruction.h"

#include <string>
#include <vector>

class Simulator {
	private:
		std::vector<Instruction>* trace;
		
		std::string result(unsigned int cacheHits);

	public:
		Simulator(std::vector<Instruction>* trace);

		std::string directMapped(unsigned int cacheSizeKB);
		std::string setAssociative(unsigned int associativity);
		std::string fullyAssocLRU();
		std::string fullyAssocHotCold();
		std::string setAssocNoAlloc(unsigned int associativity);
		std::string setAssocFetchNextLine(unsigned int associativity);
		std::string setAssocFetchMiss(unsigned int associativity);
};

#endif
