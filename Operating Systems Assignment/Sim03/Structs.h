// Program Information ////////////////////////////////////////////////////////
/** @file PCB.h
 * @brief PCB header file
 * Holds all of the information for a process
 * @author Tyler DeFoor
 * @date 10/29/2015
 * @version 1.0
 */

// Precompiler directives /////////////////////////////////////////////////////
#ifndef STRUCTS_H
#define STRUCTS_H

 // Header files ///////////////////////////////////////////////////////////////

#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

struct Operation
{
    public:
        string processType;                 //The type of process i.e "monitor"
        char operationType;                 //I, O, P, A, or S
        int cycles;                         //Number of cycles needed
        int processTime;                    //Process Time
};

struct PCB
{
    public:
        /* Variable Declarations */
        int totalTime;                      //Total time to complete process
        vector<Operation> operations;       //Vector of operations
        int id;                             //ID of the PCB
        bool isBlocked;                     //Is the process blocked
        bool isFinished;                    //Is the process finished
};

struct Interrupt
{
    int processID;                          //The ID of the process associated
                                            //   with the interrupt
    char operationType;                     //The type of operation
    string processType;                     //The type of process
};


#endif