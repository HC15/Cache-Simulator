#ifndef BLOCK_H
#define BLOCK_H

#include <string>

class Block {
	private:
		bool valid;
		unsigned int tag;
		unsigned int used;
		
	public:
		Block();
		
		bool getValid();
		unsigned int getTag();
		unsigned int getUsed();

		void setValid(bool valid);
		void setTag(unsigned int tag);
		void setUsed(unsigned int used);
};

#endif
