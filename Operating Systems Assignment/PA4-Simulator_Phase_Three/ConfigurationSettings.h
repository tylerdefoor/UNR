#ifndef CONFIGURATION_SETTINGS_H
#define CONFIGURATION_SETTINGS_H
#include <iostream>
#include <string>
#include <cstdlib>
#include "re2-master/re2/re2.h"
#include <vector>
#include "Logger.h"
using namespace std;

extern Logger myLog;
class ConfigurationSettings
{
public:
	/**
	 * Default constructor
	 * Sets version to 1.0
	 * Defines the initial regex keys needed for version 1.0
	 *
	 * Pre: None
	 * Post: All other methods will be ready to use, given their pre conditions
	 */
	ConfigurationSettings( );

	/**
	 * Inserts the given string into our vector of strings
	 * The string is not processed now, but will be processed later
	 *
	 * Pre: None
	 * Post: Given string is inserted into vector of configuration strings
	 * 
	 * @param line Line of the config file
	 */
	void insertLineOfConfigFile( 
		string line );

	/**
	 * Iterates through each line of the config file and determines
	 * 1. If it is a valid line and 
	 * 2. What to do with the given value
	 *
	 * Currently, this method is only implemented to handle version 1.0.
	 * However, in the future it will be able to adapt to version 2.0 and 3.0
	 * if it detects that is the current version being used.
	 *
	 * Pre: Version is 1.0 and the config string vector is full/ready
	 * Post: All settings will be set correctly based on the config file
	 * text available.
	 */
	void processConfigFile( );

	/**
	 * FOR DEBUGGING PURPOSES ONlY
	 * This method is used to print out the configuration settings stored in
	 * the settings variable to ensure that the file was read correctly.
	 *
	 * Pre: processConfigFile has been called and ran successfully
	 * Post: The settings will be output, but nothing will be changed.
	 */
	void outputSettingsToConsole( );
	
	unsigned int quantumCycles;
	unsigned int processCycleTime;
	unsigned int monitorDisplayTime;
	unsigned int hardDriveCycleTime;
	unsigned int printerCycleTime;
	unsigned int keyboardCycleTime;
	string cpuScheduling;
	string filePath;
	string logType;
	string logFilePath;
	string version;
private:
	/**
	 * Sets appropriate variable based on settingLine
	 * Currently, only works for version 1.0 and will crash otherwise.
	 * Uses Google's RE2 library to evaluate the string to see if it is
	 * valid. If not, program crashes with appropriate error message.
	 * settingLine 0 and 1 are dealt with here.
	 * All others are passed to setPhaseOne method.
	 * 
	 * Pre: Version is 1.0 or 2.0
	 * Post: Calls setPhaseOne to set value appropriately
	 * 
	 * @param settingLine  Between 0 and 10 inclusively (currently)
	 * @param settingValue Must be a valid setting value
	 */
	void set( 
		unsigned int settingLine, 
		string settingValue );

	/**
	 * Set variables and settings appropriately for phase one
	 * Will use atoi to convert from string to unsigned int if necessary
	 * settingLine 2 through 9 set a variable
	 * 10 is ignored
	 * 0 and 1 are dealt with in set.
	 * All others crash with appropriate error message.
	 * 
	 * @param settingLine  Must be between 2 and 10 inclusively
	 * @param settingValue Must be an appropriate setting value
	 */
	void setPhaseOne( 
		unsigned int settingLine, 
		string settingValue );

	/**
	 * Set variables and settings appropriately for phase two
	 * Will use atoi to convert from string to unsigned int if necessary
	 * settingLine 2 through 10 set a variable
	 * 0 and 1 are dealt with in set.
	 * 11 is ignored
	 * All others crash with appropriate error message.
	 * 
	 * @param settingLine  Must be between 2 and 11 inclusively
	 * @param settingValue Must be an appropriate setting value
	 */
	void setPhaseTwo(
		unsigned int settingLine, 
		string settingValue );

	/**
	 * Set variables and settings appropriately for phase three
	 * Will use atoi to convert from string to unsigned int if necessary
	 * settingLine 2 through 11 set a variable
	 * 0 and 1 are dealt with in set.
	 * 12 is ignored
	 * All others crash with appropriate error message.
	 * 
	 * @param settingLine  Must be between 2 and 12 inclusively
	 * @param settingValue Must be an appropriate setting value
	 */
	void setPhaseThree(
		unsigned int settingLine,
		string settingVale) ;

	vector< string > regexLineKeys;
	vector< string > configFile;
};
#endif // CONFIGURATION_SETTINGS_H