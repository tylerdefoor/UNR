#ifndef INTERRUPT_SYSTEM_H
#define INTERRUPT_SYSTEM_H

#include <deque>				// deque
#include <string>				// string
#include "Structs.h"
#include "Logger.h"

using namespace std;

extern Logger myLog;
class InterruptSystem
{
public:
	/**
	 * Adds a new interrupt to the front of the queue, giving it immediate
	 * priority for once interrupts are resolved
	 * 
	 * @param processNumber The number of the process that this interrupt
	 *                      came from
	 * @param endLogMessage The end log message that this interrupt should
	 *                      report once resolved
	 */
	void addNewInterruptFront( int processNumber, string endLogMessage );

	/**
	 * Adds a new interrupt to the back of the queue
	 * 
	 * @param processNumber The number of the process that this interrupt
	 *                      came from
	 * @param endLogMessage The end log message that this interrupt should
	 *                      report once resolved
	 */
	void addNewInterruptBack( int processNumber, string endLogMessage );

	/**
	 * Resolves the current interrupt, which involves taking it out of the
	 * queue, reporting its end message, then returning the process number
	 * it came from
	 *
	 * @pre This will only be called if there is an interrupt in the queue
	 * @post One less interrupt will be in the queue
	 * 
	 * @return the process number that the most recent interrupt came from
	 */
	int resolveInterrupt( );

	int numberOfInterrupts; // Keep track of our interrupts so that a size method does
							// not need to be called
private:
	deque< Interrupt > interruptQueue;
};

#endif // INTERRUPT_SYSTEM_H