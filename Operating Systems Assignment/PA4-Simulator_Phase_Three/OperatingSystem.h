#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H
#include <algorithm>    // std::sort
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "re2-master/re2/re2.h"
#include <cstdlib>
#include "Logger.h"
#include "ProcessControlBlock.h"
#include "Structs.h"
using namespace std;

extern Logger myLog;

class OperatingSystem
{
public:
	/**
	 * Default constructor for OperatingSystem
	 * 
	 * Pre: None
	 * Post: All OperatingSystem methods are ready* to use
	 * * Assuming their pre conditions are met
	 */
	OperatingSystem( );

	/**
	 * Reads the configuration file and stores each line in a
	 * ConfigurationSettings variable.
	 * At the end of this method, the ConfigurationSettings variable is told
	 * to check if the input is valid and set its own fields. It will
	 * dynamically figure out whether to use Phase 1.0, 2.0, or 3.0 settings
	 * when storing file data. Currently, only Phase 1.0 is implemented.
	 * The end of this method also tells the Logger whether to print to file,
	 * monitor, or both, and to what file if necessary.
	 *
	 * Pre: None
	 * Post: Settings will have all lines of the configuration file
	 * stored appropriately.
	 * 
	 * @param fileName Name of the configuration file
	 */
	void readConfigurationFile( 
		char * fileName );

	/**
	 * Reads the meta data file and stores operations in a
	 * vector of processes variable. Uses Regular Expressions via Google's RE2
	 * library to find operations. Data is not validated in this method, except
	 * for the first and last lines which must be
	 * "Start Program Meta-Data Code:" and "End Program Meta-Data Code."
	 * respectfully.
	 *
	 * Pre: The configuration file must have been read and stored to tell this
	 * method what meta data file to open. Also, the Google RE2 dynamic library
	 * must be available.
	 * Post: The vector of Processes variable will be stored with each meta
	 * data file instruction.
	 */
	void readMetaDataFile( );

	/**
	* Generic "run" method
	* Will call appropriate Phase 1, 2, or 3 run methods based on
	* configuration file.
	*
	* Pre: Phase number must be known, and PCB's must be loaded and
	* ready to run
	* Post: All PCB's will be run according to the configuration file
	* settings
	*/
	void runSimulator( );

	/**
	 * FOR DEBUGGING PURPOSES ONlY
	 * This method is used to print out the configuration settings stored in
	 * the settings variable to ensure that the file was read correctly.
	 *
	 * Pre: readConfigurationSettings was called and ran successfully.
	 * Post: The settings will be output, but nothing will be changed.
	 */
	void outputSettingsToConsole( );
private:
	/**
	 * Calls the run FIFO method, as PhaseOne is essentially just a 
	 * single process FIFO system
	 *
	 * Pre: The vector of processes is full and ready to be read.
	 * Post: The operations of this meta data file will have been carried out
	 * correctly and completely.
	 */
	void runPhaseOneSimulator( );

	/**
	* If cpu scheduling = SJF:
	* - Run SJF
	* If cpu scheduling = SRTFN:
	* - Run SRTFN
	*
	* Pre: This method will only be called from runSimulator, and thus
	* the preconditions of that method will satisfy the pre conditions
	* for this one
	* Post: The appropriate cpu scheduling method will be used based on
	* configuration file, and appropriate method called for that cpu scheduling
	*/
	void runPhaseTwoSimulator( );

	void runPhaseThreeSimulator( );

	/**
	* Iterates over all PCB's linearly, and calls their "run" method
	*
	* Pre: This method will only be called from runPhaseOneSimulator or
	* runPhaseTwoSimulator, so the preconditions of those methods satisfy
	* all preconditions for this method
	* Post: All PCB's will have run all of their processes
	*/
	void runFIFO( );

	void runRR( );
	void runFIFOP( );
	void runSRTFP( );

	/**
	* Runs the sortSJF method, then runs runFIFO
	*/
	void runSJF( );

	/**
	* Uses same algorithm as FIFO, except that, instead of incrementing
	* the current index, we search for the index of the shortest
	* remaining time PCB after each iteration
	*
	* Pre: This method will only be called from runPhaseTwoSimulator
	* Post: All PCB's will be iterated through, in order of least to greatest
	* remaining time
	*/
	void runSRTFN( );

	/**
	* Finds the shortest remaining time PCB by looping through PCB's
	* and (essentially) performing a min(currentShortest, currentPCB)
	*
	* Pre: This method will only be called from runSRTFN when there are still
	* active/unrun PCB's
	* Post: No PCB's will be changed in this method, except for potentially 
	* their remaining time variable
	*
	* @return index of the shortest remaining time PCB
	*/
	unsigned int findSRTFN( );

	/**
	 * Looks through Operating System instructions and checks that each
	 * start has a matching end
	 * Then goes through each PCB and runs initial calculateRemainingTime
	 *
	 * Pre: Expects that readyQueue and operatingSystemInstructions have
	 * been initialized by appropriate methods
	 * Post: Guaranteed that each OpSys instruction has a start/end (thus
	 * won't have to check this as we go along); each PCB will have their
	 * initial remaining time calculated (that way the first "search for
	 * next process" doesn't seem to take much longer than the rest)
	 */
	void prepareProcesses( );

	bool resolveInterrupts( );

	/**
	 * Looks at the current operation being performed and passes it to the
	 * correct evaluation method. If the component is not recognized, the
	 * program crashes. Accepted components: S, A, P, I, O
	 *
	 * Pre: Simulator must be running, or the operation being passed in
	 * must start the simulator
	 * Post: The correct evaluation method will correctly and completely
	 * handle this operation.
	 * 
	 * @param component  S, A, P, I, or O. Tells whether this operation is
	 * handled by the Operating System (S and A) or the process control block
	 * (P, I, O).
	 * @param operation  Describes the exact operation that this component
	 * is trying to achieve. Is not evaluated here, but is passed on.
	 * @param numberOfCycles The number of cycles that this operation will run for.
	 * Not evaluated in this method, but is passed on.
	 */
	void processOperation( 
		char component, 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Specifically evaluates the operating system operations.
	 * Ways this will succeed:
	 * - operation = start, numberOfCycles = 0, simulatorRunning = false
	 * - operation = end, numberOfCycles = 0, simulatorRunning = true
	 * All other possibilities cause the program to crash with an
	 * appropriate error message.
	 *
	 * Pre: None
	 * Post: The simulatorRunning bool will be set to either true or false
	 * depending on whether operation is start or end, respectively.
	 *  
	 * @param operation Must be 'start' or 'end'
	 * @param numberOfCycles Must be 0
	 */
	void evalOperatingSystem( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Specifically evaluates the application operations.
	 * Ways this will succeed:
	 * - operation = start, numberOfCycles = 0
	 * - operation = end, numberOfCycles = 0
	 * All other possibilities cause the program to crash with an 
	 * appropriate error message.
	 *
	 * Pre: None
	 * Post: A new application will be created or destroyed in the 
	 * vector of ProcessControlBlock variable.
	 * @param operation Must be 'start' or 'end'
	 * @param numberOfCycles Must be 0
	 */
	void evalApplication( 
		string operation, 
		unsigned int numberOfCycles );
	
	bool simulatorRunning; // True if yes, false if no
	bool applicationStarted; // True if yes, false if no
	unsigned int totalNumberOfProcesses; // Number of unique PCB's from this OS
	unsigned int indexOfCurProcess; // Current PCB from this OS
	unsigned int numberOfProcesses; // Number of current PCB's from this OS
	vector< Process > operatingSystemInstructions; // Instructions from meta-data
	vector< ProcessControlBlock > readyProcesses; // PCB's ready to use
	unordered_map< int, ProcessControlBlock > blockedProcesses; // PCB's that are blocked due to IO
	ConfigurationSettings settings; // Instructions from config file
	// Future: CPU Scheduling Type
	// Future: Quantum time
};
#endif // OPERATING_SYSTEM_H