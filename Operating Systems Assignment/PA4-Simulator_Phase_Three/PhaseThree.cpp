//////////////////////////////////////////////////////////////////////
/* 
 FileName: PhaseThree.cpp

 Description: 
 Simulates a multi-program Operating System. It will accept the
 meta-data for one or more programs with potentially unlimited number of
 meta-data operations, run each program, and end the simulation. Each unique
 I/O operation must be conducted with its own unique thread.
 Additional Operations for this Phase:
 - CPU Scheduling now utilizes preemptive schedules:
 - Round Robin(RR): Order is determined by the next process available (no sorting/selection)
 - FIFO-P: The order of the next process is determined by the order in which the processes came into the system
 - SRTF-P: The order of the next process is determined by which process has the shortest remaining time left

 */
// Version/Revision Information ///////////////////////////////////
/* 
 1.0 ( 11/19/2015 ) - Tim Kwist
 Alas we have solved it! We forgot about some basic rules about
 copy constructors and = operator overload. Coincidentally, we
 wouldn't have run into this error if we hadn't tried to "improve"
 the = operator overload from last time
 FIFO-P currently works (needs to be tested on a single core machine
 because it runs in less than half the expected time on my machine -
 I assume its because the threads are actually taking advantage of
 my AMD.
 RR currently works as well
 SRTFP now works as well
 0.7 ( 11/18/2015 ) - Tim Kwist
 A segfault error continues to elude us. If it cannot be solved,
 all hope is lost...
 Finished initial design and implementation of interrupt system
 ProcessControlBlock should need no further additions, interrupt
 system and operating system may need additions
 0.6 ( 11/17/2015 ) - Tim Kwist
 Built building blocks of interrupt system
 Partially integrated interrupt system into process control block
 0.5 ( 11/16/2015 ) - Tim Kwist
 Copied Phase Two code to new folder
 */
 // Program Description/Support /////////////////////////////////////
/*
This program relies on building Google's re2 library to be able to utilize
their dynamic library file. The make file is set up to automatically do 
this.
*/
/* 

 */
 // Precompiler Directives //////////////////////////////////////////
//
 #ifndef SIMO3_CPP
 #define SIM03_CPP
//
// Header Files ///////////////////////////////////////////////////
//
 #include <iostream>
 #include "ConfigurationSettings.h"
 #include "Logger.h"
 #include "ProcessControlBlock.h"
 #include "OperatingSystem.h"
 #include "InterruptSystem.h"

 using namespace std;

// Global Constant Definitions ////////////////////////////////////
//
// Defined my Logger as global because it is used in 3 different
// classes and I didn't want to have to redundantly pass it around,
// store it in a class that didn't need it, or have every class
// create a version of its methods.
Logger myLog;
// Defined interrupts as global because it interacts with
// OperatingSystem, ProcessControlBlock, and stand alone threads
// spawned by the ProcessControlBlock in version 3.0, and I 
// didn't want to have to redundantly pass it around or develop 
// a messaging system
// To an extent, it "belongs" to the Operating System, but the
// frequent use with the threads makes it very tricky to easily
// pass messages between the Operating System, ProcessControlBlock,
// and the threads themselves.
InterruptSystem interrupts;
//

//
// Class Definitions //////////////////////////////////////////////
//
///// NONE
//

//
// Free Function Prototypes ///////////////////////////////////////
//
///// NONE
//

// Main Function Implementation ///////////////////////////////////
//
int main( int argc, char * argv[ ] ) 
{
    OperatingSystem os;
    myLog.timer.start( );
    if( argc != 2 )
    {
        cout << 
        "Incorrect number of command line parameters. Only need name of file" 
        << endl;
        exit( 0 );
    }
    os.readConfigurationFile( argv[1] );
    os.readMetaDataFile( );
    //os.outputSettingsToConsole( );
    myLog.logProcess( "Simulator program starting" );
    os.runSimulator( );
    myLog.timer.stop( );
    myLog.outputLogFile( );

    return 0;
}

//
// Free Function Implementation ///////////////////////////////////
//
///// NONE
//



//
// Class/Data Structure Member Implementation //////////////////////
//
///// NONE
//

//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // SIM01_CPP
//