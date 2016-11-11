// Program Information ////////////////////////////////////////////////////////
/** @file Simulation.h
 * @brief Simulator Header File
 * Operates the simulation
 * @author Tyler DeFoor
 * @date 10/29/2015
 * @version 1.0
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef SIMULATION_H
#define SIMULATION_H

// Header files ///////////////////////////////////////////////////////////////

#include "PCB.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "SimpleTimer.h"
#include <unistd.h>
#include <thread>
#include <vector>

class Simulator
{
    public:
        /* Method Declarations */
        Simulator ( string configFile );
        ~Simulator (  );
        void readMetaFile (  );
        void run (  );
        void calcTime ( PCB& source );

        /* Variable Declarations */
        string logType;
        string logPath;
        string metaFile;
        string scheduleType;
        double version;
        double totalElapsed;
        bool toFile;
        bool toScreen;
        int processerTime;
        int monitorTime;
        int hardDriveTime;
        int printerTime;
        int keyboardTime;
        vector<PCB> start;
        vector<PCB> ready;
};

#endif