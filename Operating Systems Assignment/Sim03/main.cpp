// Program Information ////////////////////////////////////////////////////////
/** @file main.cpp
 * @brief Main Driver
 * Runs the simulation
 * @author Tyler DeFoor
 * @date 10/29/2015
 * @version 1.0
 */

#include <vector>
#include <string>
#include <iostream>
#include "Structs.h"
#include "SimpleTimer.h"
#include "Simulation.h"
#include <algorithm>

using namespace std;

int SUCCESS = 0;


 /** Compare
*@fn bool compare ( const PCB& lhs, const PCB& rhs )
*@brief Compares two PCBs and returns if the lhs is less than the rhs
*@param lhs The left hand side
*@param rhs The right hand side
*@pre Both PCBs are initialized 
*@post Nothing in either PCB is changed
*/
bool compare ( const PCB& lhs, const PCB& rhs )
{
    return lhs.totalTime < rhs.totalTime;
}

int main ( int argc, char* argv[] )
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/

    string fileName;
    Operation tempOperation; 

    /*=====  End of Variable Declaration  ======*/
    
    //Check to see if the correct number of arguments was given
    if ( argc == 2 )
    {
        //Set fileName to the second argument
        fileName = argv[1];
    }
    //If there was an incorrect number of arguments
    else
    {
        //Terminate the program
        cout << "Error: Incorrect number of input files. PROGRAM TERMINATED\n";
        return -1;
    }

    //Start the simulation and pass it fileName
    Simulator currentSim ( fileName );

    //Read in the metadata file
    currentSim.readMetaFile (  );

    //Put the new queue into the ready queue
    currentSim.ready = currentSim.start;

    //Run the simulation
    currentSim.runFIFOP (  );

    //Return successful program
    return SUCCESS;
}