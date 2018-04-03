#include "Instruction.h"

#include <string>

using namespace std;

Instruction::Instruction(string line) {
	this->flag = line[0];
	this->address = stoul(line.substr(2, 10), nullptr, 16);
}

char Instruction::getFlag() {
	return this->flag;
}

unsigned long Instruction::getAddress() {
	return this->address;
}
