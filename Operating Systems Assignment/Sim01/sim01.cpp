// Program Information ////////////////////////////////////////////////////////
/** @file sim01.cpp
 * @brief Simulator Assignment 1
 * Simulates a single program in the operating system
 * @author Tyler DeFoor
 * @date 10/6/2015
 * @version 1.0
 */

// Header files ///////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "sim01.h"
#include "SimpleTimer.h"
#include <queue>
#include <unistd.h>
#include <thread>

int SUCCESS = 0;

using namespace std;

/**main
*@fn int main ( int argc, char** argv )
*@brief The main method
*@param argc Number of arguments
*@param argv List of arguments
*@return Successful or terminating int
*@pre N/A
*@post N/A
*/

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

    currentSim.readMetaFile (  );

    currentSim.run ( currentSim.ready );


    return SUCCESS;
}


 /**Constructor
*@fn Simulator::Simulator ( string configFile );
*@brief Constructor for class Simulator
*@param configFile The path to the config file
*@pre N/A
*@post A Simulator is constructed
*/
Simulator::Simulator ( string configFile )
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/
    
    ifstream fin;
    char* tempString = new char[50];

    /*=====  End of Variable Declaration  ======*/
    
    //Open the file
    fin.open ( configFile.c_str (  ) );

    //Get the version
    fin.ignore( 256, ':' );
    fin >> version;

    //Get the file path
    fin.ignore( 256, ':' );
    fin >> metaFile;

    //Get the processor cycle time
    fin.ignore( 256, ':' );
    fin >> processerTime;

    //Get the monitor cycle time
    fin.ignore( 256, ':' );
    fin >> monitorTime;

    //Get the hard drive cycle time
    fin.ignore( 256, ':' );
    fin >> hardDriveTime;

    //Get the printer cycle time
    fin.ignore( 256, ':' );
    fin >> printerTime;

    //Get the keyboard cycle time
    fin.ignore( 256, ':' );
    fin >> keyboardTime;

    //Get the log type
    fin.ignore( 256, ':' );
    fin.getline ( tempString, 256, '\n' );
    logType = tempString;
    logType = logType.substr ( 1 );

    if ( logType == "Log to Both")
    {
        cout << "Log to Both" << endl;
        toFile = true;
        toScreen = true;
    }
    else if ( logType == "Log to Monitor")
    {
        cout << "Log to Monitor" << endl;
        toFile = false;
        toScreen = true;
    }
    else
    {
        cout << "Log to File" << endl;
        toFile = true;
        toScreen = false;
    }

    //Get the log path
    fin.ignore( 256, ':' );
    fin.getline( tempString, 256, '\n' );
    logPath = tempString;
    logPath = logPath.substr ( 1 );

    //Close out the file
    fin.close (  );
}

 /**
*@fn Destructor
*@brief Deallocates all memory used by Simulator
*@param N/A
*@pre A Simulator exists
*@post A Simulator no longer has any memory allocated to it
*/
Simulator::~Simulator (  )
{
    //Do nothing
}

 /** readMetaFile
*@fn void Simulator::readMetaFile (  )
*@brief Reads the meta file and executes commands
*@param N/A
*@pre A simulator is instantiated
*@post Nothing in metaFile is changed
*/
void Simulator::readMetaFile (  )
{
    /*=============================================
    =            Variable Declarations            =
    =============================================*/
    
    fstream fin;
    string tempString;
    char* temp = new char[50];
    Operation tempOperation;
    int processLength = 0;
    
    /*=====  End of Variable Declarations  ======*/
    
    //Open the meta data file
    fin.open ( metaFile.c_str (  ) );

    //Skip the first line
    fin.ignore ( 256, '\n' );

    fin.getline ( temp, 256, ';' );
    tempString = temp;
    //Until we hit EOF
    while ( tempString != "A(end)0" )
    {
        //Get the first character for the operation type
        tempOperation.operationType = tempString.at ( 0 );


        //Find the length of the data in between the parenthesis
        processLength = tempString.find ( ')' ) - tempString.find ( '(' );

        //The process is the data starting at 2 with length processLength
        tempOperation.processType = tempString.substr ( 2, processLength - 1 );

        //Cut the string off so that only the number of cycles are left
        tempString = tempString.substr ( processLength + 2 );

        //I would use stoi
        tempOperation.cycles = atoi ( tempString.c_str (  ) );


        fin.ignore ( 1 );

        if ( fin.peek (  ) == '\n' )
        {
            fin.ignore ( 1 );
        }
        
        ready.push ( tempOperation );

        fin.getline ( temp, 256, ';' );
        tempString = temp;

    }

    //The simulation will always end with these, so we hard code them
    tempOperation.processType = "end";
    tempOperation.operationType = 'A';
    tempOperation.cycles = 0;

    ready.push ( tempOperation );

    tempOperation.processType = "end";
    tempOperation.operationType = 'S';
    tempOperation.cycles = 0;

    ready.push ( tempOperation );

    //Close the file
    fin.close (  );
}


 /** Run
*@fn void Simulator::run (  )
*@brief Runs through the ready queue
*@param N/A
*@pre The ready queue has operational data
*@post Nothing in ready is changed
*/
void Simulator::run ( queue<Operation>& ready )
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/
    
    int counter; 
    SimpleTimer timer;
    Operation currentOp;
    char* elapsed = new char[10];
    int size = ready.size (  );
    ofstream fout;
    
    /*=====  End of Variable Declaration  ======*/
    
    //Start the timer
    timer.start(  );

    //Open the log file
    if ( toFile )
    {
        fout.open ( logPath.c_str (  ) );
    }

    //Time the start of the sim
    timer.stop (  );
    timer.getElapsedTime ( elapsed );
    totalElapsed += atof ( elapsed );
    timer.start (  );

    if ( toScreen )
    {
        cout << totalElapsed << " - Simulator program starting" 
            << endl;
    }
    if ( toFile )
    {
        fout << totalElapsed << " - Simulator program starting" 
            << endl;
    }

    //Loop through the queue
    for ( counter = 0; counter < size; counter++ )
    {
        //Get the start time of the new process
        timer.stop (  );
        timer.getElapsedTime ( elapsed );
        totalElapsed += atof ( elapsed );
        timer.start (  );

        //Get the operation from the front of the queue
        currentOp = ready.front (  );

        //If the operation is a system operation
        if ( currentOp.operationType == 'S' )
        {
            if ( toScreen )
            {
                //We'll always output this
                cout << totalElapsed << " - OS: ";
            }
            if ( toFile )
            {
                //We'll always output this
                fout << totalElapsed << " - OS: ";
            }

            //If we're starting stuff
            if ( currentOp.processType == "start" )
            {
                if ( toScreen )
                {
                    //Say we're starting stuff
                    cout << "preparing process 1" << endl;
                }
                if ( toFile )
                {
                    fout << "preparing process 1" << endl;
                }
            }
            //Otherwise
            else
            {
                if ( toScreen )
                {
                    //Say we're stopping stuff
                    cout << "removing process 1" << endl;
                }
                if ( toFile )
                {
                    //Say we're stopping stuff
                    fout << "removing process 1" << endl;
                }
            }
        }

        //If its an application operation
        else if ( currentOp.operationType == 'A')
        {
            //If we're starting things
            if ( currentOp.processType == "start" )
            {
                if ( toScreen )
                {
                    //Tell the user we're starting things
                    cout << totalElapsed << " - OS: starting process 1" << endl;
                }
                if ( toFile )
                {
                    fout << totalElapsed << " - OS: starting process 1" << endl;
                }
            }
        }

        //If its an input operation
        else if ( currentOp.operationType == 'I')
        {
            //And a hard drive operation
            if ( currentOp.processType == "hard drive")
            {
                if ( toScreen )
                {
                    //Start printing
                    cout << totalElapsed << " - Process 1: start hard "
                                         << "drive input" << endl;
                }
                if ( toFile )
                {
                    //Start printing
                    fout << totalElapsed << " - Process 1: start hard "
                                         << "drive input" << endl;
                }                   

                //Thread the sleep time
                thread current( usleep,
                         ( currentOp.cycles * hardDriveTime * 1000 ) );                

                //Join the thread
                current.join (  );

                //Get the time that took
                timer.stop (  );
                timer.getElapsedTime ( elapsed );
                totalElapsed += atof ( elapsed );
                timer.start (  );
                if ( toScreen )
                {
                    //Output that time
                    cout << totalElapsed << " - Process 1: end hard drive input"
                                         << endl;
                }
                if ( toFile )
                {
                    //Output that time
                    fout << totalElapsed << " - Process 1: end hard drive input"
                                         << endl;
                }
            }
            //The only other option is keyboard input
            else
            {
                if ( toScreen )
                {
                    //Start printing
                    cout << totalElapsed << " - Process 1: start keyboard input"
                                         << endl;
                }
                if ( toFile )
                {
                    //Start printing
                    fout << totalElapsed << " - Process 1: start keyboard input"
                                         << endl;
                }

                //Thread the sleep time
                thread current( usleep,
                         ( currentOp.cycles * keyboardTime * 1000 ) );   

                //Join the thread
                current.join (  );

                //Get the time that took
                timer.stop (  );
                timer.getElapsedTime ( elapsed );
                totalElapsed += atof ( elapsed );
                timer.start (  );

                if ( toScreen )
                {
                    //Output that time
                    cout << totalElapsed << " - Process 1: end keyboard input" 
                                         << endl;
                }
                if ( toFile )
                {
                    //Output that time
                    fout << totalElapsed << " - Process 1: end keyboard input" 
                                         << endl;
                }
            }
        }
        //If it is an output operation
        else if ( currentOp.operationType == 'O' )
        {
            //And a hard drive operation
            if ( currentOp.processType == "hard drive")
            {
                if ( toScreen )
                {
                    //Start printing
                    cout << totalElapsed << " - Process 1: start hard "
                                         << "drive output" << endl;
                }
                if ( toFile )
                {
                    //Start printing
                    fout << totalElapsed << " - Process 1: start hard "
                                         << "drive output" << endl;
                }                   

                //Thread the sleep time
                thread current( usleep,
                         ( currentOp.cycles * hardDriveTime * 1000 ) );                

                //Join the thread
                current.join (  );

                //Get the time that took
                timer.stop (  );
                timer.getElapsedTime ( elapsed );
                totalElapsed += atof ( elapsed );
                timer.start (  );
                if ( toScreen )
                {
                    //Output that time
                    cout << totalElapsed<< " - Process 1: end hard drive output"
                                         << endl;
                }
                if ( toFile )
                {
                    //Output that time
                    fout << totalElapsed<< " - Process 1: end hard drive output"
                                         << endl;
                }
            }
            //And a printer operation
            else if ( currentOp.processType == "printer")
            {
                if ( toScreen )
                {
                    //Output the start time
                    cout << totalElapsed << " - Process 1: start printer output"
                                         << endl;
                }
                if ( toFile )
                {
                    //Output the start time
                    fout << totalElapsed << " - Process 1: start printer output"
                                         << endl;
                }
                

                //Thread the sleep time
                thread current( usleep,
                         ( currentOp.cycles * printerTime * 1000 ) );   

                //Wait for the thread to finish
                current.join (  );

                //Get the elapsed time
                timer.stop (  );
                timer.getElapsedTime ( elapsed );
                totalElapsed += atof ( elapsed );
                timer.start (  );

                if ( toScreen )
                {
                    //Print out the elapsed time
                    cout << totalElapsed << " - Process 1: end printer output"
                                         << endl;
                }
                if ( toFile )
                {
                    //Print out the elapsed time
                    fout << totalElapsed << " - Process 1: end printer output" 
                                         << endl;
                }
            }
            //And a monitor operation
            else
            {
                if ( toScreen )
                {
                    //Output the start time
                    cout << totalElapsed << " - Process 1: start monitor output"
                                    << endl;
                }
                if ( toFile )
                {
                    //Output the start time
                    fout << totalElapsed << " - Process 1: start monitor output"
                                         << endl;
                }
                

                //Thread the sleep time
                thread current( usleep,
                         ( currentOp.cycles * monitorTime * 1000 ) );   

                //Wait for the thread to finish
                current.join (  );

                //Get the elapsed time
                timer.stop (  );
                timer.getElapsedTime ( elapsed );
                totalElapsed += atof ( elapsed );
                timer.start (  );

                if ( toScreen )
                {
                    //Print out the elapsed time
                    cout << totalElapsed << " - Process 1: end monitor output" 
                                        << endl;
                }
                if ( toFile )
                {
                    //Print out the elapsed time
                    fout << totalElapsed << " - Process 1: end monitor output" 
                                        << endl;
                }

            }
        }
        //If it is a processing action
        else
        {
            if ( toScreen )
            {
                //Output that start time
                cout << totalElapsed << " - Process 1: start processing action"
                                     << endl;
            }
            if ( toFile )
            {
                //Output that start time
                fout << totalElapsed << " - Process 1: start processing action"
                                     << endl;
            }            
            

            //Thread the sleep time
            thread current( usleep,
                         ( currentOp.cycles * processerTime * 1000 ) );   

            //Wait for the thread to finish
            current.join (  );

            //Get the elapsed time
            timer.stop (  );
            timer.getElapsedTime ( elapsed );
            totalElapsed += atof ( elapsed );
            timer.start (  );

            if ( toScreen )
            {
                //Print the elapsed time
                cout << totalElapsed << " - Process 1: end processing action"
                     << endl;
            }
            if ( toFile )
            {
                //Print the elapsed time
                fout << totalElapsed << " - Process 1: end processing action"
                     << endl;
            }   
        }

        //Pop the first item off the queue
        ready.pop (  );
    }

    timer.stop (  );

    if ( toFile )
    {
        fout.close (  );
    }

}

