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
#include "Structs.h"
#include <unistd.h>
#include <thread>
#include <chrono>


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

    //Get the scheduling type
    fin.ignore( 256, ':' );
    fin >> quanta;

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
    int counter = 0;
    int processLength = 0;
    int idCounter = 0;
    Operation test;
    int queueSize = 1;
    
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

    start.resize ( 1000 );
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
            else if ( tempString == "A(end)0" )
            {
                queueSize++;
                //Push the PCB onto the new queue
                start [ tempPCB.id ] = tempPCB;

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

            //Set the process time
            tempOperation.processTime = 0;

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

            //If it is a hard drive process
            if ( tempOperation.processType == "hard drive" )
            {
                //Set the process time to a hard drive time
                tempOperation.processTime = hardDriveTime;
            }

            //If it is a monitor process
            else if ( tempOperation.processType == "monitor" )
            {
                //Set the process time to a monitor time
                tempOperation.processTime = monitorTime;
            }

            //If it is a processor action
            else if ( tempOperation.processType == "process" )
            {
                //Set the process time to a processer time
                //  Say that 5 times fast
                tempOperation.processTime = processerTime;
            }

            //If it is a keyboard process
            else if ( tempOperation.processType == "keyboard" )
            {
                //Set the process time to a keyboard time
                tempOperation.processTime = keyboardTime;
            }

            //Push the operation onto the PCB queue
            tempPCB.operations.push_back ( tempOperation );
        }
    }
    //Close the file
    fin.close (  );

    start.resize ( queueSize );
}

 /** Run
*@fn void Simulator::run (  )
*@brief Runs the program 
*@param N/A
*@pre Vector start is initialized and sorted to ready
*@post Nothing in start is changed
*/
void Simulator::runFIFOP (  )
{
    /*=============================================
    =            Variable Declarations            =
    =============================================*/
    
    SimpleTimer timer;
    Operation currentOp;
    char* elapsed = new char[10];
    int size = ready.size (  );
    ofstream fout;
    int current = 0;
    Interrupt newInterrupt;
    int timeForInterrupt = 0;
       
    /*=====  End of Variable Declarations  ======*/

    //Start the timer
    timer.start(  );

    //Open the log file
    if ( toFile )
    {
        fout.open ( logPath.c_str (  ) );
    }


    //While there are still unfinished PCBs
    while ( ! allDone (  ) )
    {
        //Get the start time of the new process
        timer.stop (  );
        timer.getElapsedTime ( elapsed );
        totalElapsed += atof ( elapsed );
        timer.start (  );

        resolveInterrupts ( elapsed );

        //Get the PCB at ready[current]
        current = 1;

        //While we still have operations to... Operate?
        if ( ! ( ready[current].operations.empty (  ) ) && 
            ! ( ready[current].isBlocked ) )
        {
            currentOp = ready[current].operations.front (  );

            cout << currentOp.operationType << " " << currentOp.processType << " " << currentOp.cycles << endl;
            //If the current operation isn't I/O
            if ( currentOp.operationType != 'I' && 
                 currentOp.operationType != 'O' )
            {
                //If it is a system operation
                if ( currentOp.operationType == 'S' )
                {
                    //If we are starting things
                    if ( currentOp.processType == "start" )
                    {
                        //If we are printing to the screen
                        if ( toScreen )
                        {
                            cout << elapsed << " - Simulator program starting" 
                            << endl;
                        }
                        //If we are printing to a file
                        if ( toFile )
                        {
                            fout << elapsed << " - Simulator program starting" 
                            << endl;   
                        }
                    }
                    //Delete the current operation
                    ready[current].operations.erase ( 
                        ready[current].operations.begin (  ) );
                }
                //If it is a processor operation
                else if ( currentOp.operationType == 'P' )
                {
                    if ( currentOp.cycles <= quanta )
                    {
                        //If we print to the screen
                        if ( toScreen )
                        {
                            cout << elapsed << " - Process " << ready[current].id
                                << ": start processing action" << endl;
                        }
                        //If we print to a file
                        if ( toFile )
                        {
                            fout << elapsed << " - Process " << ready[current].id
                                << ": start processing action" << endl;
                        }

                        //Sleep for the required time
                        this_thread::sleep_for ( 
                            chrono::milliseconds ( currentOp.processTime 
                                                    * currentOp.cycles ) );

                        //Get the time
                        timer.stop (  );
                        timer.getElapsedTime ( elapsed );
                        totalElapsed += atof ( elapsed );
                        timer.start (  );

                        //If we print to the screen
                        if ( toScreen )
                        {
                            cout << elapsed << " - Process " << ready[current].id
                                << ": end processing action" << endl;
                        }
                        //If we print to a file
                        if ( toFile )
                        {
                            fout << elapsed << " - Process " << ready[current].id
                                << ": end processing action" << endl;
                        }


                        //Delete the current operation
                        ready[current].operations.erase ( 
                            ready[current].operations.begin (  ) );
                    }
                    else
                    {
                        //If we print to the screen
                        if ( toScreen )
                        {
                            cout << elapsed << " - Process " << ready[current].id
                                << ": start processing action" << endl;
                        }
                        //If we print to a file
                        if ( toFile )
                        {
                            fout << elapsed << " - Process " << ready[current].id
                                << ": start processing action" << endl;
                        }

                        //Sleep for the required time
                        this_thread::sleep_for ( 
                            chrono::milliseconds ( currentOp.processTime 
                                                    * quanta ) );

                        //Subtract the quanta from the cycles
                        ready[current].operations.front (  ).cycles -= quanta;

                        timer.stop (  );
                        timer.getElapsedTime ( elapsed );
                        totalElapsed += atof ( elapsed );
                        timer.start (  );

                        //If we print to the screen
                        if ( toScreen )
                        {
                            cout << elapsed << " - Interrupt: Process " 
                            << ready[current].id << " time out" << endl;
                        }
                        //If we print to a file
                        if ( toFile )
                        {
                            fout << elapsed << " - Interrupt: Process " 
                            << ready[current].id << " time out" << endl;
                        }
                    }
                }
                //If it is an application start or end
                else
                {
                    //Pop it off because it is no longer relevant
                    ready[current].operations.erase ( ready[current].operations.begin (  ) );
                    //If its the end of the process
                    {
                        //The PCB is done
                        ready[current].isFinished == true;
                    }
                }

            }
            //If the current operation is I/O
            else
            {
                //Print out that we started a process thingy
                cout << elapsed << " - Process " << ready[current].id << " " 
                    << currentOp.processType << " ";

                //If it is an input operation
                if ( currentOp.processType == "I" )
                {
                    cout << "input start" << endl;
                }

                //If it is an output operation
                else
                {
                    cout << "output start" << endl;
                }

                //Set the needed interrupt info
                newInterrupt.processID = ready[current].id;
                newInterrupt.operationType = currentOp.operationType;
                newInterrupt.processType = currentOp.processType;
                timeForInterrupt = currentOp.cycles * currentOp.processTime;

                //Create the thread
                thread process ( &Simulator::runInterrupt, this, 
                    newInterrupt, timeForInterrupt );

                //Detach the thread
                process.detach (  );

                //Set the PCB to blocked
                ready[current].isBlocked = true;
                ready[current].operations.erase ( ready[current].operations.begin (  ) );

            }

        }

        //If all the processes are blocked
        else if ( allBlocked (  ) )
        {
            //Let people know the processor is idle (lazy bum)
            cout << elapsed << " - Processor idle" << endl;

            //Wait for an interrupt
            while ( interrupts.empty (  ) )
            {
                //Twiddle your thumbs
            }
        }
        else 
        {
            current++;
        }
                        
    }

    //Stop the timer
    timer.stop (  );

    //If we output to the screen
    if ( toScreen )
    {
        //Output the end of the simulation
        cout << elapsed << " - Simulator program ending" 
            << endl;
    }
    //If we output to file
    if ( toFile )
    {
        //Putput the end of the simulation
        fout << elapsed << " - Simulator program ending" 
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


 /**runInterrupt
*@fn void runInterrupt ( Interrupt current, int totalTime )
*@brief Runs I/O and puts it in the interrupt queue when it is finished
*@param current The current interrupt
*@param time Time in milliseconds to sleep for
*@pre The interrupt is fully instantiated
*@post Nothing in current is changed
*/
void Simulator::runInterrupt ( Interrupt current, int time )
{
    //Sleep for how long the interrupt lasts
    this_thread::sleep_for ( chrono::milliseconds ( time ) );

    //Push the interrupt onto the interrupt vector
    interrupts.push_back ( current );
}

 /**
*@fn void Simulator::resolveInterrupts ( char* currentTime )
*@brief Resolves all interrupts
*@param currentTime The time the simulator has been running
*@pre N/A
*@post All interrupts are resolved and interrupts vector is cleared
*/
void Simulator::resolveInterrupts ( char* currentTime )
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/

    Interrupt tempInterrupt;
    
    /*=====  End of Variable Declaration  ======*/

    //Loop through the interrupts
    while ( ! interrupts.empty (  ) )
    {
        cout << "Interrupt resolved" << endl;
        //Get the first interrupt
        tempInterrupt = interrupts.front (  );

        //Erase the first interrupt
        interrupts.erase ( interrupts.begin (  ) );

        //Output that an interrupt interrupted something (how rude!)
        cout << currentTime << " - Interrupt: Process " 
        << tempInterrupt.processID << " " << tempInterrupt.processType << " ";

        //If it was an input operation
        if ( tempInterrupt.operationType == 'I' )
        {
            cout << "input completed" << endl;
        }

        //If it was an output operation
        else
        {
            cout << "output completed" << endl;
        }

        //Unblock the process
        ready[tempInterrupt.processID].isBlocked = false;
    }
}


/**allDone
*@fn bool Simulator::allDone (  )
*@brief Checks to see if all PCBs are finished
*@pre N/A
*@post Nothing in ready is changed
*/
bool Simulator::allDone (  )
{
    /*=============================================
    =            Variable Declarations            =
    =============================================*/
    
    bool allDone = true;                                //If we are done
    int counter = 0;

    /*=====  End of Variable Declarations  ======*/
    
    //Loop through the ready queue
    for ( counter = 0; counter < ready.size (  ); counter++ )
    {
        //If a PCB isn't finished
        if ( ! ( ready[counter].isFinished ) )
        {
            //We aren't done!
            allDone = false;
        }
    }

    //Return if we are done
    return allDone;
}

/**allBlocked
*@fn bool Simulator::allBlocked (  )
*@brief Checks to see if all PCBs are blocked
*@pre N/A
*@post Nothing in ready is changed
*/
bool Simulator::allBlocked (  )
{
    /*=============================================
    =            Variable Declarations            =
    =============================================*/
    
    bool allBlocked = true;                         //If we are blocked
    int counter = 0;                                //Generic counter

    /*=====  End of Variable Declarations  ======*/
    
    //Loop through the ready queue
    for ( counter = 0; counter < ready.size (  ); counter++ )
    {
        //If a PCB isn't blocked
        if ( ! ( ready[counter].isBlocked ) )
        {
            //We aren't all blocked
            allBlocked = false;
        }
    }

    //Return if we are all blocked
    return allBlocked;
}