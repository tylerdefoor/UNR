// Program Information ////////////////////////////////////////////////////////
/** @file sim01.h
 * @brief Simulator Assignment 1
 * Simulates a single program in the operating system
 * @author Tyler DeFoor
 * @date 10/6/2015
 * @version 1.0
 */

// Precompiler directives /////////////////////////////////////////////////////
#ifndef SIM01_H
#define SIM01_H

// Header files ///////////////////////////////////////////////////////////////

#include <queue>
#include <string>
#include <iostream>
#include "SimpleTimer.h"

using namespace std;

// Struct definition  /////////////////////////////////////////////////////////

struct Operation
{
 	public:
 		string processType;
 		char operationType;
 		int cycles;
};

// Class definition  //////////////////////////////////////////////////////////

class Simulator
{
	public:
		/* Method Declarations */
		Simulator ( string configFile );
		~Simulator (  );
		void readMetaFile (  );
		void run ( queue<Operation>& ready );

		/* Variable Declarations */
		string logType;
		string logPath;
		string metaFile;
		double version;
		double totalElapsed;
		bool toFile;
		bool toScreen;
		int processerTime;
		int monitorTime;
		int hardDriveTime;
		int printerTime;
		int keyboardTime;
		queue<Operation> ready;
};




#endif