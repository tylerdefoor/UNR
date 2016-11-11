// Program Information ////////////////////////////////////////////////////////
/** @file PCB.h
 * @brief PCB header file
 * Holds all of the information for a process
 * @author Tyler DeFoor
 * @date 10/29/2015
 * @version 1.0
 */

// Precompiler directives /////////////////////////////////////////////////////
#ifndef PCB_H
#define PCB_H

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
        double cycles;                      //Number of cycles needed
};

struct PCB
{
    public:
        /* Variable Declarations */
        int totalTime;                      //Total time to complete process
        vector<Operation> operations;       //Vector of operations
        int id;                             //ID of the PCB
};


#endif