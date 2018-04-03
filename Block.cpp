#include "Block.h"

using namespace std;

Block::Block() {
	this->valid = false;
	this->tag = 0;
	this->used = 0;
}

bool Block::getValid() {
	return this->valid;
}

unsigned int Block::getTag() {
	return this->tag;
}

unsigned int Block::getUsed() {
	return this->used;
}

void Block::setValid(bool valid) {
	this->valid = valid;
}

void Block::setTag(unsigned int tag) {
 	this->tag = tag;
}

void Block::setUsed(unsigned int used) {
 	this->used = used;
}
