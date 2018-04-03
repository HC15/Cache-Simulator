#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

class Instruction {
	private:
		char flag;
		unsigned long address;

	public:
		Instruction(std::string line);

		char getFlag();
		unsigned long getAddress();
};

#endif
