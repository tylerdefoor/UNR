#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
using namespace std;

enum InterruptType
{
	IO = 0,
	QUANTUM = 1
};


struct Interrupt
{
	int processNumber;
	string endLogMessage;
	
};

struct PcbThread
{
	// Currently, some variables are unused depending on version of simulator
	// Once all 3 versions are working, I'll try to refactor this to use polymorphism
	// to dynamically figure out which type of node is currently being used
public:
	unsigned int processTime;
	string startLogMessage;
	string blockLogMessage;
	string endLogMessage;
	unsigned int timePerCycle;
	unsigned int numCyclesRemaining;
	InterruptType interruptType; // The type of interrupt this thread will trigger
};

struct Process
{
	char component;
	string operation;
	unsigned int numberOfCycles;
};

#endif // STRUCTS_H