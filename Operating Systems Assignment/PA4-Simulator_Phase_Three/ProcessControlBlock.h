#ifndef PROCESS_CONTROL_BLOCK_H
#define PROCESS_CONTROL_BLOCK_H

#include <chrono>					// milliseconds
#include <string>					// string, to_string
#include <thread>					// thread, this_thread::sleep_for
#include <vector>					// vector
#include "ConfigurationSettings.h"
#include "InterruptSystem.h"
#include "Logger.h"
#include "Structs.h"

using namespace std;

extern Logger myLog;
extern InterruptSystem interrupts;

class ProcessControlBlock
{
public:
	/**
	 * Default constructor
	 * Should only be used for temporary PCB's
	 */
	ProcessControlBlock( );

	/**
	 * Copy constructor
	 * Creates a deep copy of the rhs PCB
	 */
	ProcessControlBlock( const ProcessControlBlock & rhs );

	/**
	 * ProcessControlBlock constructor
	 * Updates this PCB's processNumber and various numberOfCycles settings
	 * - ProcessCycleTime
	 * - MonitorDisplayTime
	 * - HardDriveCycleTime
	 * - PrinterCycleTime
	 * - KeyboardCyelTime
	 *
	 * Pre: None
	 * Post: Will update all settings necessary for this object to function
	 * correctly.
	 */
	ProcessControlBlock( 
		unsigned int pProcessNumber, 
		const ConfigurationSettings &pSettings );
	/**
	 * = Operator overload
	 * Creates a deep copy of the rhs PCB
	 */
	ProcessControlBlock& operator=(const ProcessControlBlock& rhs);

	/**
	* Iterates through each process in the PCB,
	* logs the process, waits, then logs the end of the
	* process.
	*
	* Pre: Processes will have been fully loaded into the
	*      readyProcessThreads vector
	* Post: readyProcessThreads will be empty and finishedProcessThreads
	*       will be filled with all the processes from readyProcessThreads
	*/
	void runApplicationNonPreemptive();

	/**
	 * Preemptive application runs essentially the same as nonPreemptive,
	 * but doesn't wait for IO threads (detaches instead of joins), and it
	 * removes process threads from the ready queue if they report back that they
	 * are finished
	 *
	 * Pre: Processes will have been fully loaded into the
	 * 		readyPrcessThreads vector
	 * Post: readyProcessThreads will be updated appropriately based on the
	 * 		type of thread that was run and the status of that thread
	 * 	
	 * @return Returns whether or not the thread run was IO or process
	 *                 If IO, return true; if process, return false
	 */
	bool runApplicationPreemptive();

	/**
	 * Removes the first instruction from the readyQueue
	 * This will only be used for removing IO threads after they have
	 * returned
	 */
	void removeFirstInstruction();

	/**
	* Add a new instruction to this PCB
	*
	* Pre: The new instruction was initialized correctly
	* Post: This PCB will have one more instruction in it
	*
	* @param newInstruction Either a Processor, Input, or Output action
	* 			with a number of cycles and a specific operation
	*/
	void addInstructionNonPreemptive(Process newInstruction);

	/**
	 * Adds a new instruction to this PCB with preemptive related information
	 * (Type of thread, blocked message)
	 *
	 * Pre: The new instruction parameter was initialized correctly
	* Post: This PCB will have one more instruction in it
	*
	* @param newInstruction Either a Processor, Input, or Output action
	* 			with a number of cycles and a specific operation
	*/
	void addInstructionPreemptive(Process newInstruction);

	/**
	* Calculates the remaining time of the active processes in this PCB
	* If the remaining time has already been calculated and no new process
	* has been run / added, will use the last calculated time instead of
	* recalculating
	*
	* @return 0 if there are no more processes, or >0 dependent on the
	*  number of instructions and their run times
	*/
	unsigned int getRemainingTime();
	
	unsigned int processNumber;
private:

	/**
	 * Preemptive process will run for the min of:
	 * - The number of quantums specified in ConfigurationSettings
	 * - The number of cycles it has left
	 * - The number of cycles before an interrupt occurs
	 * It will then report back whether or not it is finished
	 * (ie, no more cycles)
	 * 
	 * @param  currentProcess A copy of the current process
	 *                        that is to be run. Its
	 *                        number of cycles and total
	 *                        time left wil be updated
	 * @return                True if number of cycles left
	 *                        is 0. False otherwise.
	 */
	bool runProcessPreemptive( PcbThread &currentProcess );

	/**
	 * Initialize a new process
	 * Sets sleep time to processCycleTime * numberOfCycles
	 *
	 * Pre: processCycleTime has been set appropriately
	 * Post: Program will initialize a new thread, that will be
	 * completely ready to run and report on its activities
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newProcessThreadNonPreemptive( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Initialize a new input thread
	 * Sets sleep time to 
	 *  hardDriveCycleTime * numberOfCycles
	 *  keyboardCycleTime * numberOfCycles
	 * based on the given operation
	 *
	 * Pre: hardDriveCycleTime and keyboardCycleTime
	 * have been set appropriately
	 * Post: Program will initialize a new thread, that will be
	 * completely ready to run and report on its activities
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newInputThreadNonPreemptive( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Initialize a new output thread
	 * Sets sleep time to 
	 *  hardDriveCycleTime * numberOfCycles
	 *  monitorDisplayTime * numberOfCycles
	 *  printerCycleTime * numberOfCycles
	 * based on the given operation
	 *
	 * Pre: hardDriveCycleTime, monitorDisplayTime, printerCycleTime
	 * have been set appropriately
	 * Post: Program will initialize a new thread, that will be
	 * completely ready to run and report on its activities
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newOutputThreadNonPreemptive( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Initialize a new process
	 * Sets sleep time to processCycleTime * numberOfCycles
	 *
	 * Pre: processCycleTime has been set appropriately
	 * Post: Program will initialize a new thread, that will be
	 * completely ready to run and report on its activities
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newProcessThreadPreemptive( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Initialize a new input thread
	 * Sets sleep time to 
	 *  hardDriveCycleTime * numberOfCycles
	 *  keyboardCycleTime * numberOfCycles
	 * based on the given operation
	 *
	 * Pre: hardDriveCycleTime and keyboardCycleTime
	 * have been set appropriately
	 * Post: Program will initialize a new thread, that will be
	 * completely ready to run and report on its activities
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newInputThreadPreemptive( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Initialize a new output thread
	 * Sets sleep time to 
	 *  hardDriveCycleTime * numberOfCycles
	 *  monitorDisplayTime * numberOfCycles
	 *  printerCycleTime * numberOfCycles
	 * based on the given operation
	 *
	 * Pre: hardDriveCycleTime, monitorDisplayTime, printerCycleTime
	 * have been set appropriately
	 * Post: Program will initialize a new thread, that will be
	 * completely ready to run and report on its activities
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newOutputThreadPreemptive( 
		string operation, 
		unsigned int numberOfCycles );

	
	bool needToRecalcRT; // Used to determine whether the remaining time needs to be recalculated
	unsigned int remainingTime;
	unsigned int quantumCycles;
	unsigned int processCycleTime;
	unsigned int monitorDisplayTime;
	unsigned int hardDriveCycleTime;
	unsigned int printerCycleTime;
	unsigned int keyboardCycleTime;
	vector<PcbThread> readyProcessThreads;
};
#endif // PROCESS_CONTROL_BLOCK_H