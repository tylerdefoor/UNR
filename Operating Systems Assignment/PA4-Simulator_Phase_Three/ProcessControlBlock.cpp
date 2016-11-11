#include "ProcessControlBlock.h"

/**
 * Nonpreemptive thread just sleeps for the parameter thread's
 * processTime
 * @param currentThread CurrentThread to be waited on
 */
static void createNonPremptiveThread( 
	PcbThread currentThread )
{
	this_thread::sleep_for( chrono::milliseconds( currentThread.processTime ) );
}

/**
 * Preemptive thread waits for a given process time, then creates
 * and triggers a new interrupt with a given processNumber and
 * endLogMessage
 * 
 * @param processNumber processNumber of the process that spawned this
 *                      thread
 * @param processTime   Amount of time to wait for
 * @param endLogMessage The message that this thread will report once
 *                      the interrupt it resolved
 */
static void createPremptiveThread(
	int processNumber, int processTime, string endLogMessage )
{
	this_thread::sleep_for( chrono::milliseconds( processTime ) );
	interrupts.addNewInterruptBack( processNumber, endLogMessage );
}

ProcessControlBlock::ProcessControlBlock( )
{
	needToRecalcRT = true;
	remainingTime = 0;
	processNumber = 0;
	processCycleTime = 0;
	monitorDisplayTime = 0;
	hardDriveCycleTime = 0;
	printerCycleTime = 0;
	keyboardCycleTime = 0;
}

ProcessControlBlock::ProcessControlBlock( 
	unsigned int pProcessNumber, 
	const ConfigurationSettings & pSettings )
{
	needToRecalcRT = true;
	remainingTime = 0;
	processNumber = pProcessNumber;
	quantumCycles = pSettings.quantumCycles;
	processCycleTime = pSettings.processCycleTime;
	monitorDisplayTime = pSettings.monitorDisplayTime;
	hardDriveCycleTime = pSettings.hardDriveCycleTime;
	printerCycleTime = pSettings.printerCycleTime;
	keyboardCycleTime = pSettings.keyboardCycleTime;
}

ProcessControlBlock::ProcessControlBlock( const ProcessControlBlock & rhs )
{
	needToRecalcRT = rhs.needToRecalcRT;
	remainingTime = rhs.remainingTime;
	processNumber = rhs.processNumber;
	quantumCycles = rhs.quantumCycles;
	processCycleTime = rhs.processCycleTime;
	monitorDisplayTime = rhs.monitorDisplayTime;
	hardDriveCycleTime = rhs.hardDriveCycleTime;
	printerCycleTime = rhs.printerCycleTime;
	keyboardCycleTime = rhs.keyboardCycleTime;
	readyProcessThreads = vector<PcbThread>(rhs.readyProcessThreads);
}

ProcessControlBlock& ProcessControlBlock::operator=( const ProcessControlBlock & rhs )
{
	if(this == &rhs)
	{
		return * this;
	}
	needToRecalcRT = rhs.needToRecalcRT;
	remainingTime = rhs.remainingTime;
	processNumber = rhs.processNumber;
	quantumCycles = rhs.quantumCycles;
	processCycleTime = rhs.processCycleTime;
	monitorDisplayTime = rhs.monitorDisplayTime;
	hardDriveCycleTime = rhs.hardDriveCycleTime;
	printerCycleTime = rhs.printerCycleTime;
	keyboardCycleTime = rhs.keyboardCycleTime;
	readyProcessThreads = vector<PcbThread>(rhs.readyProcessThreads);
	return * this;
}


void ProcessControlBlock::runApplicationNonPreemptive()
{
	needToRecalcRT = true;
	// Check that there are threads to run
	if( readyProcessThreads.size( ) == 0 )
	{
		myLog.logError( "This ProcessControlBlock has no processes, but was asked to run its threads" );
	}
	// Iterate through each thread
	// Process thread, then dump from ready queue
	while( readyProcessThreads.size( ) != 0 )
	{
		PcbThread currentThread = readyProcessThreads.front( );
		// Log start process
		myLog.logProcess( currentThread.startLogMessage );
		// Sleep
		thread process( createNonPremptiveThread, currentThread );
		process.join( );
		// Log end process
		myLog.logProcess( currentThread.endLogMessage );
		// Take it out of the vector
		readyProcessThreads.erase( readyProcessThreads.begin( ) );
	}
}

bool ProcessControlBlock::runApplicationPreemptive()
{
	needToRecalcRT = true;
	bool currentProcessIsIO = false;
	// Check that there are threads to run
	if( readyProcessThreads.size( ) == 0 )
	{
		myLog.logError( "This ProcessControlBlock has no processes, but was asked to run its threads" );
	}
	PcbThread & currentThread = readyProcessThreads.front( );
	// Log start process
	myLog.logProcess( currentThread.startLogMessage );
	if( currentThread.interruptType == InterruptType::IO )
	{
		thread process( createPremptiveThread, int(processNumber), int(currentThread.processTime), string(currentThread.endLogMessage));
		process.detach();
		currentProcessIsIO = true;
		interrupts.addNewInterruptFront( processNumber, currentThread.blockLogMessage );
	}
	else if( currentThread.interruptType == InterruptType::QUANTUM )
	{
		bool processIsFinished = runProcessPreemptive( currentThread );
		if( processIsFinished == true )
		{
			interrupts.addNewInterruptFront( processNumber, currentThread.endLogMessage );
			readyProcessThreads.erase( readyProcessThreads.begin( ) );
		}
	}

	return currentProcessIsIO;
}

void ProcessControlBlock::removeFirstInstruction()
{
	readyProcessThreads.erase( readyProcessThreads.begin( ) );
}

bool ProcessControlBlock::runProcessPreemptive( PcbThread &currentProcess )
{
	bool interruptTriggered = ( interrupts.numberOfInterrupts != 0 );
	unsigned int cyclesRun = 0;
	while( ( currentProcess.numCyclesRemaining != 0 ) && ( !interruptTriggered ) )
	{
		if( interrupts.numberOfInterrupts == 0 )
		{
			// Either run for total of quantums or number of cycles left, whichever lower
			int cyclesToRun = min( quantumCycles, currentProcess.numCyclesRemaining );
			if( cyclesRun < cyclesToRun )
			{
				currentProcess.numCyclesRemaining--;
				cyclesRun++;
				//usleep( currentProcess.timePerCycle * 1000 );
				this_thread::sleep_for( chrono::milliseconds( currentProcess.timePerCycle ) );
			}
			else
			{
				interrupts.addNewInterruptFront( processNumber, currentProcess.blockLogMessage );
				// Program specifications expect a different message for processes that have already been
				// run at least once
				// Process #: processing action - continue
				currentProcess.startLogMessage =
					string( "Process " ) + 
					to_string( processNumber ) + 
					": processing action - continue"; 
				interruptTriggered = true;
			}
		}
		else
		{
			interruptTriggered = true;
		}
	}
	currentProcess.processTime = currentProcess.numCyclesRemaining * currentProcess.timePerCycle;
	return ( currentProcess.numCyclesRemaining == 0 );
}

unsigned int ProcessControlBlock::getRemainingTime( )
{
	// If we don't need to recalculate, save time by returning the
	// last calculated remainingTime
	if( needToRecalcRT == false )
	{
		return remainingTime;
	}

	if(readyProcessThreads.size() == 0)
	{
		remainingTime = 0;
		needToRecalcRT = false;
		return remainingTime;
	}

	// Otherwise, reset the remaining time and iterate through each
	// process and add the process time to our remaining time
	// Set the need to recalculate flag to false
	needToRecalcRT = false;
	remainingTime = 0;
	vector<PcbThread>::iterator it;
	for( it = readyProcessThreads.begin( );
		it != readyProcessThreads.end( );
		it++ )
	{
		remainingTime += it->processTime;
	}

	return remainingTime;
}

void ProcessControlBlock::addInstructionNonPreemptive( Process newInstruction )
{
	// Set recalc flag to true since we now have a new instruction
	// Then outsource the work to the appropriate thread creation
	// method
	needToRecalcRT = true;
	switch( newInstruction.component )
	{
		case 'P':
		{
			newProcessThreadNonPreemptive( newInstruction.operation, newInstruction.numberOfCycles );
			break;
		}
		case 'I':
		{
			newInputThreadNonPreemptive( newInstruction.operation, newInstruction.numberOfCycles );
			break;
		}
		case 'O':
		{
			newOutputThreadNonPreemptive( newInstruction.operation, newInstruction.numberOfCycles );
			break;
		}
		default:
		{
			myLog.logError( "Unknown component: " + newInstruction.component );
		}
	}
}

void ProcessControlBlock::addInstructionPreemptive( Process newInstruction )
{
	// Set recalc flag to true since we now have a new instruction
	// Then outsource the work to the appropriate thread creation
	// method
	needToRecalcRT = true;
	switch( newInstruction.component )
	{
		case 'P':
		{
			newProcessThreadPreemptive( newInstruction.operation, newInstruction.numberOfCycles );
			break;
		}
		case 'I':
		{
			newInputThreadPreemptive( newInstruction.operation, newInstruction.numberOfCycles );
			break;
		}
		case 'O':
		{
			newOutputThreadPreemptive( newInstruction.operation, newInstruction.numberOfCycles );
			break;
		}
		default:
		{
			myLog.logError( "Unknown component: " + newInstruction.component );
		}
	}
}

void ProcessControlBlock::newProcessThreadNonPreemptive( 
	string operation, 
	unsigned int numberOfCycles )
{
	PcbThread currentThread;
	// Check for correct operation type
	if( operation.compare( "run" ) != 0 )
	{
		myLog.logError( "Unknown operation for process: " + operation );
	}
	// Process #: start processing action
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start processing action";
	// Calculate the time thread will need to be processed
	currentThread.processTime = processCycleTime * numberOfCycles;
	// Process #: end processing action
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end processing action";
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newInputThreadNonPreemptive( 
	string operation, 
	unsigned int numberOfCycles )
{
	PcbThread currentThread;
	// Process #: start <operation> input
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " input";
	// Calculate the time thread will need to be processed
	if( operation.compare( "hard drive" ) == 0 )
	{
		currentThread.processTime = hardDriveCycleTime * numberOfCycles;
	}
	else if( operation.compare( "keyboard" ) == 0 )
	{
		currentThread.processTime = keyboardCycleTime * numberOfCycles;
	}
	else
	{
		myLog.logError( "Unknown operation for input: " + operation );
	}
	// Process #: end <operation> input
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + operation + " input";
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newOutputThreadNonPreemptive( 
	string operation, 
	unsigned int numberOfCycles )
{
	PcbThread currentThread;
	// Process #: start <operation> output
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " output";
	// Calculate the time thread will need to be processed
	if( operation.compare( "hard drive" ) == 0 )
	{
		currentThread.processTime = hardDriveCycleTime * numberOfCycles;
	}
	else if( operation.compare( "monitor" ) == 0 )
	{
		currentThread.processTime = monitorDisplayTime * numberOfCycles;
	}
	else if( operation.compare( "printer" ) == 0 )
	{
		currentThread.processTime = printerCycleTime * numberOfCycles;
	}
	else
	{
		myLog.logError( "Unknown operation for output: " + operation );
	}
	// Process #: end <operation> output
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + 
		operation + 
		" output";
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newProcessThreadPreemptive( 
	string operation, 
	unsigned int numberOfCycles )
{
	PcbThread currentThread;
	currentThread.interruptType = InterruptType::QUANTUM;
	// Check for correct operation type
	if( operation.compare( "run" ) != 0 )
	{
		myLog.logError( "Unknown operation for process: " + operation );
	}
	// Process #: processing action - start
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": processing action - start";
	// Process # - quantum time out
	currentThread.blockLogMessage = 
		string( "Process " ) +
		to_string( processNumber ) +
		" - quantum time out";
	// Process #: processing action - completed
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": processing action - completed";
	// Give the thread the means to calculate its wait times
	currentThread.timePerCycle = processCycleTime;
	currentThread.numCyclesRemaining = numberOfCycles;
	currentThread.processTime = processCycleTime * numberOfCycles;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newInputThreadPreemptive( 
	string operation, 
	unsigned int numberOfCycles )
{
	PcbThread currentThread;
	currentThread.interruptType = InterruptType::IO;
	// Process #: <operation> input - start
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": " + operation + " input - start";
	// Process #: block for <operation> input
	currentThread.blockLogMessage =
		string( "Process " ) +
		to_string( processNumber ) +
		": block for " + operation + " input";
	// Process # - <operation> input completed
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": " + operation + " input - completed";
	// Give the thread the means to calculate its wait times
	if( operation.compare( "hard drive" ) == 0 )
	{
		currentThread.timePerCycle = hardDriveCycleTime;
		currentThread.processTime = hardDriveCycleTime * numberOfCycles;
	}
	else if( operation.compare( "keyboard" ) == 0 )
	{
		currentThread.timePerCycle = keyboardCycleTime;
		currentThread.processTime = keyboardCycleTime * numberOfCycles;
	}
	else
	{
		myLog.logError( "Unknown operation for input: " + operation );
	}
	currentThread.numCyclesRemaining = numberOfCycles;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newOutputThreadPreemptive( 
	string operation, 
	unsigned int numberOfCycles )
{
	PcbThread currentThread;
	currentThread.interruptType = InterruptType::IO;
	// Process #: <operation> output - start
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": " + operation + " output - start";
	// Process #: block for <operation> output
	currentThread.blockLogMessage =
		string( "Process " ) +
		to_string( processNumber ) +
		": block for " + operation + " output";
	// Process # - <operation> output completed
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": " + operation + " output - completed";
	// Give the thread the means to calculate its wait times
	if( operation.compare( "hard drive" ) == 0 )
	{
		currentThread.timePerCycle = hardDriveCycleTime;
		currentThread.processTime = hardDriveCycleTime * numberOfCycles;
	}
	else if( operation.compare( "monitor" ) == 0 )
	{
		currentThread.timePerCycle = monitorDisplayTime;
		currentThread.processTime = monitorDisplayTime * numberOfCycles;
	}
	else if( operation.compare( "printer" ) == 0 )
	{
		currentThread.timePerCycle = printerCycleTime;
		currentThread.processTime = printerCycleTime * numberOfCycles;
	}
	else
	{
		myLog.logError( "Unknown operation for output: " + operation );
	}
	currentThread.numCyclesRemaining = numberOfCycles;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}