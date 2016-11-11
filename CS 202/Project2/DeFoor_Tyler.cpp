#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//Structs
struct Card
{
	char suit[10];
	int rank;
	char loc[11];
};

struct Player
{
	char name[15];
	int id[5];
	Card hand[5];
};

//Method prototypes
void readIn ( Card[] );
void readIn ( Player[] );
void shuffle ( Card[], Card[] );
void deal ( Card[], Player[] );
void getCards ( Card[] );
void getPlayers ( Player[] );
void strCopy ( char[], char[] );
void strCopy ( const char[], char[] );
void concat ( char[], char[], char[] );
int strLen ( char[] );
int strLen ( const char[] );
void menu (  );

int main (  )
{
	Card deck[52];
	Player group[4];
	readIn ( deck );
	readIn ( group );
	return 0;
}

void readIn ( Card dest[] )
{
	//Variables
	ifstream fin;
	char temp[100];
	int iTemp;

	cout << "Enter the name of the card file" << endl;
	cin >> temp;
	fin.open ( temp );

	for ( int i = 0; i < 52; i++ )
	{
		fin >> temp;
		strCopy ( temp, dest[i].suit );
		fin >> temp;
		dest[i].rank = atoi ( temp );
	}

	fin.close (  );
}

void readIn ( Player dest[] ) 
{
	//Variables
	ifstream fin;
	char temp[100];
	int iTemp;

	cout << "Enter the name of the player file" << endl;
	cin >> temp;
	fin.open ( temp );

	for ( int i = 0; i < 4; i++ )
	{
		fin >> temp;
		strCopy ( temp, dest[i].name );
		fin >> temp;
		for ( int j = 0; j < 5; j++ )
		{
			dest[i].id[j] = temp[j] - '0';
		}
		cout << endl;
	}
	fin.close (  );
}


/**
  *Returns the length of a C-Style String, not including the null
  */
int strLen ( char source[] )
{
	char temp;
	int length = 0;
	temp = source[0];
	while ( temp != '\0' )
	{
		length++;
		temp = source[length];
	}
	return length;
}


int strLen ( const char source[] )
{
	char temp;
	int length = 0;
	temp = source[0];
	while ( temp != '\0' )
	{
		length++;
		temp = source[length];
	}
	return length;
}


/**
  *Copies source to dest
  *Assumes that the dest array is to size
  */
void strCopy ( char source[], char dest[] )
{
	for ( int i = 0; i < strLen ( source ) + 1; i++ )
	{
		dest[i] = source[i];
	}
}


void strCopy ( const char source[], char dest[] )
{
	for ( int i = 0; i < strLen ( source ); i++ )
	{
		dest[i] = source[i];
	}
}


/**
  *Concatenates beg and end into dest
  *Assumes dest is to size
  */
void concat ( char beg[], char end[], char dest[] )
{
	int length = strLen ( beg ) + strLen ( end ) + 1;
	for ( int i = 0; i < length; i++ )
	{
		if ( i > strLen ( beg ) )
		{
			//Since we have to skip the null character in beg
			//   we have to go to one less than i in dest
			//Likewise, we have to start from the beginning of end
			//   so we go to the length of beg subtracted from i
			dest[i-1] = end[i - ( strLen ( beg ) + 1)];
		}
		else if ( i == strLen ( beg ) + 1 )
		{
			//Do nothing, this is a null character
		}
		else
		{
			//Beg[i] is copied into dest[i]
			dest[i] = beg[i];
		}
	}
}