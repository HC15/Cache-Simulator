all: cache-sim

cache-sim: main.o Simulator.o Block.o Instruction.o
	g++ *.o -std=c++17 -o cache-sim

main.o: main.cpp
	g++ -c -g -Wall -std=c++17 main.cpp -o main.o

Simulator.o: Simulator.h Simulator.cpp
	g++ -c -g -Wall -std=c++17 Simulator.cpp -o Simulator.o

Block.o: Block.h Block.cpp
	g++ -c -g -Wall -std=c++17 Block.cpp -o Block.o

Instruction.o: Instruction.h Instruction.cpp
	g++ -c -g -Wall -std=c++17 Instruction.cpp -o Instruction.o

clean:
	rm -f *.o cache-sim
