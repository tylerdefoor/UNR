#include "Logger.h"

void Logger::setTypeAndName( string pLogType, string pFileName )
{
	logType = pLogType;
	fileName = pFileName;
}

void Logger::logProcess( string processDescription )
{
	if( logType.empty( ) || fileName.empty( ) )
	{
		logError( "Log Type or Log File Name have not been defined" );
	}

	char timeInText[15];
	timer.stop( );
	timer.getElapsedTime( timeInText );
	timer.start( );
	totalTime += atof( timeInText );
	char buffer[100];
	sprintf( buffer, "%.6f - %s\n", totalTime, processDescription.c_str( ) );
	string outputString( buffer );

	if( logType.compare( "File" ) == 0 )
	{
		fileOutputs.push_back( outputString );
	}
	else if( logType.compare( "Monitor" ) == 0 )
	{
		cout << outputString;
	}
	else if( logType.compare( "Both" ) == 0 )
	{
		fileOutputs.push_back( outputString );
		cout << outputString;
	}

}

void Logger::logError( string errorMessage )
{
	cout << ( "Error: " + errorMessage ) << endl;
	exit( 0 );
}

void Logger::outputLogFile( )
{
	ofstream logFile;
	logFile.open( fileName.c_str( ) );
	if( logFile.is_open( ) == false )
	{
		string errorMessage = "Could not open file" + fileName;
		logError( errorMessage );
	}

	for( vector<string>::iterator vecIterator = fileOutputs.begin( );
		vecIterator != fileOutputs.end( );
		vecIterator++ )
	{
		logFile << * vecIterator;
	}
	
	logFile.close( );
}