#ifndef LOGGER_H
#define LOGGER_H
#include "SimpleTimer.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class Logger
{
public:
	/**
	 * Set the type of output for operations and the name of the file
	 * if operations are to be output to a file
	 *
	 * Pre: None
	 * Post: Type of output and name of file will be set as necessary
	 * 
	 * @param pLogType  Must be 'Monitor', 'File', or 'Both'
	 * @param pFileName Must be a .lgf file
	 */
	void setTypeAndName( 
		string pLogType, 
		string pFileName = "" );

	/**
	 * Outputs the process. If type is monitor or both, will immediately
	 * send to cout. If file or both, will save to a vector of strings to be
	 * output at the end of the program.
	 *
	 * Output is in the form of %.6f - processDescription
	 * Where %.6f is the type since program started
	 *
	 * Pre: Type of output and file name have been set
	 * Post: Output will be distributed appropriately
	 * 
	 * @param processDescription Describes what to output.
	 */
	void logProcess( 
		string processDescription );

	/**
	 * Outputs an error to the console and immediately exits the program.
	 * Output is in the form of Error: errorMessage
	 *
	 * Pre: Something has gone wrong
	 * Post: Error message printed and program closed
	 * 
	 * @param errorMessage Describes the error that occurred.
	 */
	void logError( 
		string errorMessage );

	/**
	 * Outputs the contents of fileOutputs to a file.
	 *
	 * Pre: fileOutputs has information in it and fileName has been set
	 * Post: fileOutput will be logged to a file
	 */
	void outputLogFile( );
	SimpleTimer timer;
private:
	vector<string> fileOutputs;
	string logType;
	string fileName;
	double totalTime;
};

#endif // LOGGER_H