// Program Information ////////////////////////////////////////////////////////
/** @file Simulation.cpp
 * @brief Simulator File
 * Operates the simulation
 * @author Tyler DeFoor
 * @date 10/29/2015
 * @version 1.0
 */

// Header files ///////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Simulation.h"
#include "SimpleTimer.h"
#include "PCB.h"
#include <unistd.h>
#include <thread>


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

    //Get the scheduling type
    fin.ignore( 256, ':' );
    fin >> scheduleType;

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

    //If we log to both the screen and a file
    if ( logType == "Log to Both")
    {
        toFile = true;
        toScreen = true;
    }
    //If we only log to the monitor
    else if ( logType == "Log to Monitor")
    {
        toFile = false;
        toScreen = true;
    }
    //If we only log to the file
    else
    {
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

 /** Destructor
*@fn Simulator::~Simulator (  )
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
    PCB tempPCB;
    int counter;
    int processLength = 0;
    int idCounter = 0;
    Operation test;
    
    /*=====  End of Variable Declarations  ======*/
    
    //Open the meta data file
    fin.open ( metaFile.c_str (  ) );

    //Skip the first line
    fin.ignore ( 256, '\n' );

    //Loop past the Start of the simulation
    fin.getline ( temp, 256, ';' );

    //Get the first relevent line
    fin.getline ( temp, 256, ';' );

    //Put it in the checker string
    tempString = temp;

    //Loop until the end of the relevent meta data file
    while ( fin.good (  ) )
    {
        
        //If there are spaces at the beginning, clear them
        if ( tempString.front (  ) == ' ' )
        {
            tempString = tempString.substr ( 1 );
        }

        //If it is the start or the end of an application
        if ( tempString == "A(start)0" || tempString == "A(end)0" )
        {
            if ( tempString == "A(start)0")
            {
                //Increment the ID
                idCounter++;

                //Assign the PCB it's new number
                tempPCB.id = idCounter;
            }
            if ( tempString == "A(end)0" )
            {
                //Push the PCB onto the new queue
                start.push_back ( tempPCB );

                //Clear the operations vector
                tempPCB.operations.clear (  );
            }

            //Get the first character for the operation type
            tempOperation.operationType = tempString.at ( 0 );

            //Find the length of the data in between the parenthesis
            processLength = tempString.find ( ')' ) - tempString.find ( '(' );

            //The process is the data starting at 2 with length processLength
            tempOperation.processType = tempString.substr ( 2, processLength - 1 );

            //Cut the string off so that only the number of cycles are left
            tempString = tempString.substr ( processLength + 2 );

            //Convert the cycle number to an int
            tempOperation.cycles = atoi ( tempString.c_str (  ) );

            //Ignore the semicolon
            fin.ignore ( 1 );

            //Ignore the newline if it is there
            while ( fin.peek (  ) == ' ')
            {
                fin.ignore ( 1 );
            }
            if ( fin.peek (  ) == '\n' )
            {
                fin.ignore ( 1 );
            }

            //Get the next line up to the semicolon
            fin.getline ( temp, 256, ';' );

            //Put that in tempSTring
            tempString = temp;

            //Push the operation onto the PCB queue
            tempPCB.operations.push_back ( tempOperation );  
        }
        else
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

            //Ignore the newline if it is there
            while ( fin.peek (  ) == ' ' )
            {
                fin.ignore ( 1 );
            }
            if ( fin.peek (  ) == '\n' )
            {
                fin.ignore ( 1 );
            }

            //Get the next line up to the semicolon
            fin.getline ( temp, 256, ';' );

            //Put that in tempSTring
            tempString = temp;

            //Push the operation onto the PCB queue
            tempPCB.operations.push_back ( tempOperation );
        }
    }
    //Close the file
    fin.close (  );
}

 /** Run
*@fn void Simulator::run (  )
*@brief Runs the program 
*@param N/A
*@pre Vector start is initialized and sorted to ready
*@post Nothing in start is changed
*/
void Simulator::run (  )
{
    /*=============================================
    =            Variable Declarations            =
    =============================================*/
    
    int counter; 
    SimpleTimer timer;
    Operation currentOp;
    PCB currentPCB;
    char* elapsed = new char[10];
    int size = ready.size (  );
    ofstream fout;
       
    /*=====  End of Variable Declarations  ======*/

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

    //If we output to the screen
    if ( toScreen )
    {
        //Output the start of the sim
        cout << totalElapsed << " - Simulator program starting" 
            << endl;
    }
    //If we output to the file
    if ( toFile )
    {
        //Output the start of the sim
        fout << totalElapsed << " - Simulator program starting" 
            << endl;
    }

    for ( counter = 0; counter < size; counter++ )
    {
        //Get the start time of the new process
        timer.stop (  );
        timer.getElapsedTime ( elapsed );
        totalElapsed += atof ( elapsed );
        timer.start (  );

        //Get the PCB at ready[counter]
        currentPCB = ready[counter];

        //While we still have operations to... Operate?
        while ( ! ( currentPCB.operations.empty (  ) ) )
        {
            //Pop the first operation off of the operations vector
            currentOp = currentPCB.operations.front (  );
            currentPCB.operations.erase ( currentPCB.operations.begin (  ) );
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
                        cout << "preparing process " << currentPCB.id << endl;
                    }
                    if ( toFile )
                    {
                        fout << "preparing process " << currentPCB.id << endl;
                    }
                }
                //Otherwise
                else
                {
                    if ( toScreen )
                    {
                        //Say we're stopping stuff
                        cout << "removing process " << currentPCB.id << endl;
                    }
                    if ( toFile )
                    {
                        //Say we're stopping stuff
                        fout << "removing process " << currentPCB.id << endl;
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
                        cout << totalElapsed << " - OS: starting process "
                        << currentPCB.id << endl;
                    }
                    if ( toFile )
                    {
                        fout << totalElapsed << " - OS: starting process "
                        << currentPCB.id << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": start hard drive input" << endl;
                    }
                    if ( toFile )
                    {
                        //Start printing
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": start hard drive input" << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": end hard drive input" << endl;
                    }
                    if ( toFile )
                    {
                        //Output that time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": end hard drive input" << endl;
                    }
                }
                //The only other option is keyboard input
                else
                {
                    if ( toScreen )
                    {
                        //Start printing
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": start keyboard input" << endl;
                    }
                    if ( toFile )
                    {
                        //Start printing
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": start keyboard input" << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": end keyboard input" << endl;
                    }
                    if ( toFile )
                    {
                        //Output that time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": end keyboard input" << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": start hard drive output" << endl;
                    }
                    if ( toFile )
                    {
                        //Start printing
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": start hard drive output" << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": end hard drive output" << endl;
                    }
                    if ( toFile )
                    {
                        //Output that time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": end hard drive output" << endl;
                    }
                }
                //And a printer operation
                else if ( currentOp.processType == "printer")
                {
                    if ( toScreen )
                    {
                        //Output the start time
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": start printer output" << endl;
                    }
                    if ( toFile )
                    {
                        //Output the start time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": start printer output" << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": end printer output" << endl;
                    }
                    if ( toFile )
                    {
                        //Print out the elapsed time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": end printer output" << endl;
                    }
                }
                //And a monitor operation
                else
                {
                    if ( toScreen )
                    {
                        //Output the start time
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": start monitor output" << endl;
                    }
                    if ( toFile )
                    {
                        //Output the start time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": start monitor output" << endl;
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
                        cout << totalElapsed << " - Process " << currentPCB.id
                        << ": end monitor output" << endl;
                    }
                    if ( toFile )
                    {
                        //Print out the elapsed time
                        fout << totalElapsed << " - Process " << currentPCB.id
                        << ": end monitor output" << endl;
                    }
                }
            }
            //If it is a processing action
            else
            {
                if ( toScreen )
                {
                    //Output that start time
                    cout << totalElapsed << " - Process " << currentPCB.id
                        << ": start processing action" << endl;
                }
                if ( toFile )
                {
                    //Output that start time
                    fout << totalElapsed << " - Process " << currentPCB.id
                        << ": start processing action" << endl;
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
                    cout << totalElapsed << " - Process " << currentPCB.id
                        << ": end processing action" << endl;
                }
                if ( toFile )
                {
                    //Print the elapsed time
                    fout << totalElapsed << " - Process " << currentPCB.id
                        << ": end processing action" << endl;
                }
            }   
        }
        //Remove the process
        //The number 100 is arbitrary, I am really only testing to make sure
        //   that the PCB id isn't something it shouldn't be. The first time
        //   this can be printed it will always print out garbage
        if ( toScreen && currentPCB.id < 100 && currentPCB.id > 0 )
        {
            cout << totalElapsed << " - OS: removing process " 
            << currentPCB.id << endl;
        }
        if ( toFile && currentPCB.id < 100 && currentPCB.id > 0 )
        {
            fout << totalElapsed << " - OS: removing process " 
            << currentPCB.id << endl;
        }
    }

    //Stop the timer
    timer.stop (  );

    //If we output to the screen
    if ( toScreen )
    {
        //Output the end of the simulation
        cout << totalElapsed << " - Simulator program ending" 
            << endl;
    }
    //If we output to file
    if ( toFile )
    {
        //Putput the end of the simulation
        fout << totalElapsed << " - Simulator program ending" 
            << endl;

        //Close the file
        fout.close (  );
    }
    
}

 /**CalcTime
*@fn void Simulator::calcTime ( PCB source )
*@brief Calculates the time needed for a PCB
*@param source The PCB whose time is to be calculated
*@pre The PCB has relevent data
*@post source.totalTime holds the total time the PCB will take
*/
void Simulator::calcTime ( PCB& source )
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/
    
    int counter;                    //Generic counter
    Operation tempOp;               //Temporary operation holder
    
    
    /*=====  End of Variable Declaration  ======*/
    



    //Loop through the operations in the PCB
    for ( counter = 0; counter < source.operations.size (  ); counter++ )
    {
        //Get the current operation
        tempOp = source.operations[counter];

        //If the operation is a process
        if ( tempOp.operationType == 'P' )
        {
            //Add the time it will take to total time
            source.totalTime += ( tempOp.cycles * processerTime );
        }
        //If it is an output
        else if ( tempOp.operationType == 'O' )
        {
            //And a printer
            if ( tempOp.processType == "printer" )
            {
                //Add the time it will take to total time
                source.totalTime += ( tempOp.cycles * printerTime );
            }
            //And a monitor
            else if ( tempOp.processType == "monitor" )
            {
                //Add the time it will take to total time
                source.totalTime += ( tempOp.cycles * monitorTime );   
            }
            //And a hard drive
            else
            {
                //Add the time it will take to total time
                source.totalTime += ( tempOp.cycles * hardDriveTime );
            }
        //If it is an input
        }
        else if ( tempOp.operationType == 'I' )
        {
            //And a keyboard
            if ( tempOp.processType == "keyboard" )
            {
                //Add the time it will take to total time
                source.totalTime += ( tempOp.cycles * keyboardTime );
            }
            //And a hard drive
            else
            {
                //Add the time it will take to total time
                source.totalTime += ( tempOp.cycles * hardDriveTime );
            }
        }
        else
        {
            //Do Nothing because System and Applications take no time
        }
    }
}