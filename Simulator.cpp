#include "Simulator.h"
#include "Instruction.h"
#include "Block.h"

#include <string>
#include <vector>

using namespace std;

Simulator::Simulator(vector<Instruction>* trace) {
	this->trace = trace;
}

string Simulator::result(unsigned int cacheHits) {
	return to_string(cacheHits) + ',' + to_string(this->trace->size()) + ';';
}

string Simulator::directMapped(unsigned int cacheSizeKB) {
	unsigned int cacheHits = 0;

	int cacheSets = (cacheSizeKB * 1024) / 32; // convert KB to B by multiplying by 1024
	vector<Block> cache(cacheSets, Block());
	unsigned int index;
	unsigned int tag;

	unsigned long address;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();

		index = (address / 32) % cacheSets;
		tag = (address / 32) / cacheSets;

		if(cache[index].getValid()) {
			if(cache[index].getTag() == tag) {
				cacheHits++;
			}
			else {
				cache[index].setTag(tag);
			}
		}
		else {
			cache[index].setValid(true);
			cache[index].setTag(tag);
		}
	}

	return this->result(cacheHits);
}

string Simulator::setAssociative(unsigned int associativity) {
	unsigned int cacheHits = 0;

	int cacheSets = 512 / associativity; // ((16KB * 1024) B) / (32 B) = 512
	vector<vector<Block> > cache(cacheSets, vector<Block>(associativity, Block()));
	unsigned int index;
	unsigned int tag;
	unsigned int used = 0;

	bool replace;
	Block* lruBlock;

	unsigned long address;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();

		index = (address / 32) % cacheSets;
		tag = (address / 32) / cacheSets;
		used++;

		replace = true;
		lruBlock = &cache[index].front();
		for(Block& block : cache[index]) {
			if(replace) {
				if(block.getValid()) {
					if(block.getTag() == tag) {
						cacheHits++;
						block.setUsed(used);
						replace = false;
					}
					else {
						if(block.getUsed() < lruBlock->getUsed()) {
							lruBlock = &block;
						}
					}
				}
				else {
					block.setValid(true);
					block.setTag(tag);
					block.setUsed(used);
					replace = false;
				}
			}
			else {
				break;
			}
		}

		if(replace) {
			lruBlock->setTag(tag);
			lruBlock->setUsed(used);
		}
	}

	return this->result(cacheHits);
}

string Simulator::fullyAssocLRU() {
	unsigned int cacheHits = 0;

	int cacheSets = 512; // ((16KB * 1024) B) / (32 B) = 512
	vector<Block> cache(cacheSets, Block());
	unsigned int tag;
	unsigned int used = 0;

	bool replace;
	Block* lruBlock;

	unsigned long address;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();

		tag = address / 32;
		used++;

		replace = true;
		lruBlock = &cache.front();
		for(Block& block : cache) {
			if(replace) {
				if(block.getValid()) {
					if(block.getTag() == tag) {
						cacheHits++;
						block.setUsed(used);
						replace = false;
					}
					else {
						if(block.getUsed() < lruBlock->getUsed()) {
							lruBlock = &block;
						}
					}
				}
				else {
					block.setValid(true);
					block.setTag(tag);
					block.setUsed(used);
					replace = false;
				}
			}
			else {
				break;
			}
		}

		if(replace) {
			lruBlock->setTag(tag);
			lruBlock->setUsed(used);
		}
	}

	return this->result(cacheHits);
}

string Simulator::fullyAssocHotCold() {
	bool hit;
	unsigned int cacheHits = 0;

	int cacheSets = 512; // ((16KB * 1024) B) / (32 B) = 512
	vector<Block> cache(cacheSets, Block());
	unsigned int tag;

	vector<bool> hotCold(cacheSets, false); // modeling hot cold binary tree as an array
	unsigned int hotColdIndex;
	unsigned int cacheIndex;
	bool hot;

	unsigned long address;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();

		tag = address / 32;

		hit = false;
		for(int i = 0; i < cacheSets; i++) {
			if(cache[i].getValid() && cache[i].getTag() == tag) {
				hit = true;
				cacheHits++;
				
				hot = ((i % 2) == 1);
				hotColdIndex = (i / 2) + 256;
				while(hotColdIndex >= 1) {
					hotCold[hotColdIndex] = hot;
					hot = ((hotColdIndex % 2) == 1);
					hotColdIndex /= 2;
				}
				break;
			}
		}

		if(!hit) {
			hotColdIndex = 1;
			while(hotColdIndex < 256) {
				if(hotCold[hotColdIndex]) {
					hotColdIndex *= 2;
				}
				else {
					hotColdIndex *= 2;
					hotColdIndex++;
				}
			}

			cacheIndex = 2 * (hotColdIndex - 256);
			if(!hotCold[hotColdIndex]) {
				cacheIndex++;
			}
			cache[cacheIndex].setValid(true);
			cache[cacheIndex].setTag(tag);
			
			hot = (cacheIndex % 2 == 1);
			while(hotColdIndex >= 1) {
				hotCold[hotColdIndex] = hot;
				hot = (hotColdIndex % 2 == 1);
				hotColdIndex /= 2;
			}
		}
	}

	return this->result(cacheHits);
}


string Simulator::setAssocNoAlloc(unsigned int associativity) {
	unsigned int cacheHits = 0;

	int cacheSets = 512 / associativity; // ((16KB * 1024) B) / (32 B) = 512
	vector<vector<Block> > cache(cacheSets, vector<Block>(associativity, Block()));
	unsigned int index;
	unsigned int tag;
	unsigned int used = 0;

	bool replace;
	Block* lruBlock;

	char flag;
	unsigned long address;
	for(Instruction instruct : *trace) {
		flag = instruct.getFlag();
		address = instruct.getAddress();
		
		index = (address / 32) % cacheSets;
		tag = (address / 32) / cacheSets;
		used++;

		replace = true;
		lruBlock = &cache[index].front();
		for(Block& block : cache[index]) {
			if(replace) {
				if(block.getValid()) {
					if(block.getTag() == tag) {
						cacheHits++;
						block.setUsed(used);
						replace = false;
					}
					else {
						if(block.getUsed() < lruBlock->getUsed()) {
							lruBlock = &block;
						}
					}
				}
				else {
					if(flag == 'L') {
						block.setValid(true);
						block.setTag(tag);
						block.setUsed(used);
						replace = false;
					}
				}
			}
			else {
				break;
			}
		}

		if(replace && flag == 'L') {
			lruBlock->setTag(tag);
			lruBlock->setUsed(used);
		}
	}

	return this->result(cacheHits);
}

string Simulator::setAssocFetchNextLine(unsigned int associativity) {
	unsigned int cacheHits = 0;

	int cacheSets = 512 / associativity; // ((16KB * 1024) B) / (32 B) = 512
	vector<vector<Block> > cache(cacheSets, vector<Block>(associativity, Block()));
	unsigned int index;
	unsigned int tag;
	unsigned int used = 0;

	bool replace;
	Block* lruBlock;

	unsigned long line;
	unsigned long address;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();
		
		for(line = 0; line <= 32; line += 32) {
			index = ((address + line) / 32) % cacheSets;
			tag = ((address + line) / 32) / cacheSets;
			used++;

			replace = true;
			lruBlock = &cache[index].front();
			for(Block& block : cache[index]) {
				if(replace) {
					if(block.getValid()) {
						if(block.getTag() == tag) {
							if(line == 0) {
								cacheHits++;
							}
							block.setUsed(used);
							replace = false;
						}
						else {
							if(block.getUsed() < lruBlock->getUsed()) {
								lruBlock = &block;
							}
						}
					}
					else {
						block.setValid(true);
						block.setTag(tag);
						block.setUsed(used);
						replace = false;
					}
				}
				else {
					break;
				}
			}

			if(replace) {
				lruBlock->setTag(tag);
				lruBlock->setUsed(used);
			}
		}
	}

	return this->result(cacheHits);
}

string Simulator::setAssocFetchMiss(unsigned int associativity) {
	bool hit;
	unsigned int cacheHits = 0;

	int cacheSets = 512 / associativity; // ((16KB * 1024) B) / (32 B) = 512
	vector<vector<Block> > cache(cacheSets, vector<Block>(associativity, Block()));
	unsigned int index;
	unsigned int tag;
	unsigned int used = 0;

	bool replace;
	Block* lruBlock;

	unsigned long line;
	unsigned long address;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();
		
		hit = false;
		for(line = 0; line <= 32 && !hit; line += 32) {
			index = ((address + line) / 32) % cacheSets;
			tag = ((address + line) / 32) / cacheSets;
			used++;

			replace = true;
			lruBlock = &cache[index].front();
			for(Block& block : cache[index]) {
				if(replace) {
					if(block.getValid()) {
						if(block.getTag() == tag) {
							hit = true;
							if(line == 0) {
								cacheHits++;
							}
							block.setUsed(used);
							replace = false;
						}
						else {
							if(block.getUsed() < lruBlock->getUsed()) {
								lruBlock = &block;
							}
						}
					}
					else {
						block.setValid(true);
						block.setTag(tag);
						block.setUsed(used);
						replace = false;
					}
				}
				else {
					break;
				}
			}

			if(replace) {
				lruBlock->setTag(tag);
				lruBlock->setUsed(used);
			}
		}
	}

	return this->result(cacheHits);
}
