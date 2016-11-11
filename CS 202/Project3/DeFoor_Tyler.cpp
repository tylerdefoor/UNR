#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

/**
  *Card Struct
  *Contains: A suit C-Style String
  *          A rank int
  *	     A location C-Style String
  */
struct Card 
{
	char suit[8];
	int rank;
	char location[15];
};

/**
  *Player Struct
  *Contains: A name C-Style STring
  *	     An ID: int array
  */
struct Player
{
	char name[30];
	int id[5];
};

//Method Prototypes
void readIn ( Player* );
void readIn ( Card* );
void shuffle ( Card*, Card* );
void print ( Card* );
void print ( Player* );
void menu (  );
void copy ( Card&, Card& );
void strCopy ( char*, char* );
void strCopy ( const char*, char* );
int strLen ( char* );
int strLen ( const char* );

/**
  *The main method
  */
int main (  )
{
	//Variables
	Player group[4];
	Card unsh[52];
	Card sh[52];
	char response = '\0';

	//Reads in the group and the cards from file
	readIn ( group );
	readIn ( unsh );

	//Menu system
	while ( response != 'q' )
	{
		cout << "1. Display unshuffled" << endl;
		cout << "2. Display players" << endl;
		cout << "3. Shuffle Deck" << endl;
		cout << "4. Display shuffled " << endl;
		cout << "q. Quit" << endl;
		
		cin >> response;

		if ( response == '1' )
		{
			print ( unsh );
		}
		else if ( response == '2' )
		{
			print ( group );
		}
		else if ( response == '3' )
		{
			shuffle ( unsh, sh );
		}
		else if ( response == '4' )
		{
			print ( sh );
		}
		else if ( response != 'q' )
		{
			cout << "I didn't understand that." << endl;
		}
	}
	return 0;
}

/**
  *Reads in Players from a user specified file
  */
void readIn ( Player* dest )
{
	//Variables
	fstream fin;
	char temp[100];
	char* tempP = temp;
	int* iptr = NULL;
	char* tempHome = NULL;

	//Gets the file name and opens the file
	cout << "Enter the name of the Player file" << endl;
	cin >> temp;
	fin.open ( temp );

	//Reads in the Players
	for ( int i = 0; i < 4; i++ )
	{
		//Read in the name
		fin >> (*dest).name;
		iptr = (*dest).id;

		//Read in the ID
		int* home = iptr;
		fin >> temp;
		tempHome = tempP;
		for ( int j = 0; j < 5; j++ )
		{
			char temp = *tempP;
			*iptr = temp - '0';
			tempP++;
			iptr++;
		}
		tempP = tempHome;
		iptr = home;
		dest++;
	}
	fin.close (  );
}

/**
  *Reads in the Cards from a user specified file
  */
void readIn ( Card* dest )
{
	//Variables
	fstream fin;
	char temp[100];

	//Gets the file name and opens the file
	cout << "Enter the name of the Card file" << endl;
	cin >> temp;
	fin.open ( temp );
	
	//Read in the cards
	for ( int i = 0; i < 52; i++ )
	{
		fin >> temp;
		strCopy ( temp, (*dest).suit );
		fin >> temp;
		(*dest).rank = atoi ( temp );
		dest++;
	}
}

/**
  *Copies the unshuffled deck to the shuffled deck, then randomly
  *   swaps random cards in the shuffled array to shuffle the cards
  */
void shuffle ( Card* unshuffled, Card* shuffled )
{
	//Variables
	Card* unshuffledHome = unshuffled;
	Card* shuffledHome = shuffled;
	Card* card1 = NULL;
	Card* card2 = NULL;

	srand ( time(NULL ) );

	for ( int i = 0; i < 52; i++ )
	{
		strCopy ( (*unshuffled).suit, (*shuffled).suit );
		(*shuffled).rank = (*unshuffled).rank;
		if ( i != 51 )
		{
			shuffled++;
			unshuffled++;
		}
	}
	shuffled = shuffledHome;
	unshuffled = unshuffledHome;

	for ( int j = 0; j < 5000; j++ )
	{
		int k = 0;
		Card temp1, temp2;
		int rand1 = rand (  ) % 52;
		int rand2 = rand (  ) % 52;

		//Goes to the first shuffled card
		for ( k = 0; k < rand1; k++ )
		{
			shuffled++;
		}
		card1 = shuffled;

		shuffled = shuffledHome;

		//Goes to the second shuffled card
		for ( k = 0; k < rand2; k++ )
		{
			shuffled++;
		}
		
		card2 = shuffled;
		
		copy ( *card1, *card2 );

		shuffled = shuffledHome;
	}
}

/**
  *Prints out the contents of the source array
  */
void print ( Card* source )
{
	for ( int i = 0; i < 52; i++ )
	{
		cout << (*source).suit << " " << (*source).rank << endl;
		source++;
	}
}

/**
  *Prints out the contents of the source array
  */
void print ( Player* source )
{
	for ( int i = 0; i < 4; i++ )
	{
		cout << (*source).name << " ";
		int* iptr = (*source).id;
		for ( int j = 0; j < 5; j++ )
		{
			cout << *iptr;
			iptr++;
		}
		cout << endl;
		source++;
	}
}

/**
  *Copies the values of the source Card into the dest Card
  *Assumes the source Card has significant values
  */
void copy ( Card& source, Card& dest )
{
	//Variables
	char cTemp[15];
	int iTemp;

	//Copy the dest values into the temp variables
	strCopy ( dest.suit, cTemp );
	iTemp = dest.rank;

	//Copy the source values into dest
	strCopy ( source.suit, dest.suit );
	dest.rank = source.rank;

	//Copy the temp values into source
	strCopy ( cTemp, source.suit );
	source.rank = iTemp;
}

/**
  *Copies the source C-Style String to the dest C-Style String
  *Assumes the arrays are already to size
  */
void strCopy ( char* source, char* dest )
{
	while ( *source != '\0' )
	{
		*dest = *source;
		dest++;
		source++;
	}
	*dest = '\0';
}

/**
  *Copies the source C-Style String to the dest C-Style String
  *Assumes the arrays are already to size
  */
void strCopy ( const char* source, char* dest )
{
	while ( *source != '\0' )
	{
		*dest = *source;
		dest++;
		source++;
	}
	*dest = '\0';
}
