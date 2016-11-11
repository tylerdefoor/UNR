#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "string.h"
#include "eights.h"

using namespace std;

//Method prototypes
void readIn ( Card* );
void readIn ( Player*, int );
void shuffle ( Card*, Card*, Card*, Card* );
void printShuffled ( Card* );
void printDiscard ( Card* );
void printStock ( Card* );
void printGroup ( Player*, int );
void cardCopy ( Card&, Card& );
void swap ( Card&, Card& );

int main (  )
{
	Card* unsh = new Card[52];
	Card* sh = new Card[52];
	Card* stock = new Card[51];
	Card* discard = new Card;
	readIn ( unsh );

	Player* group = new Player[8];
	readIn ( group, 8 );

	shuffle ( unsh, sh, stock, discard );
}

/**
  *Reads in the cards from a user specified file
  */
void readIn ( Card* dest )
{
	//Variables
	char* temp = new char[100];
	fstream fin;

	//Get the file name and open the file
	cout << "Enter the name of the card file" << endl;
	cin >> temp;

	fin.open ( temp );

	for ( int i = 0; i < 52; i ++ )
	{
		fin >> temp;
		(*dest).setSuit ( temp );
		fin >> temp;
		(*dest).setRank ( atoi ( temp ) );
		(*dest).setLocation ( "unshuffled" );
		dest++;
	}
	delete[] temp;
	fin.close (  );
}

/**
  *Reads in the players from a user specified file
  */
void readIn ( Player* dest, int numPlayers )
{
	//Variables
	char* temp = new char[100];
	fstream fin;

	//Get the file name and open it
	cout << "Enter the name of the player file" << endl;
	cin >> temp;
	fin.open ( temp );

	//Reads in the players
	int* iptr = NULL;
	for ( int i = 0; i < numPlayers; i++ )
	{
		//Read in the name
		fin >> temp;
		(*dest).setName ( temp );

		//Read in the ID
		iptr = new int[5];
		char* tempHome = temp;
		int* iHome = iptr; 		//Just try suing me Apple

		fin >> temp;

		for ( int j = 0; j < 5; j++ )
		{
			*iptr = (int)(*temp) - 48;
			temp++;
			iptr++;
		}
		iptr = iHome;				//Teehee
		(*dest).setID ( iptr );

		dest++;
		temp = tempHome;
	}
	fin.close (  );
	delete[] temp;

}

/**
  *Shuffles the cards
  *Takes the cards from the unshuffled array and copies them over to the shuffled array
  *Then it shuffles the cards in place
  */
void shuffle ( Card* unshuffled, Card* shuffled, Card* stock, Card* discard )
{
	//Variables
	Card* home = shuffled;
	srand ( time ( NULL ) );

	//Copy the cards from unshuffled to shuffled
	for ( int i = 0; i < 52; i++ )
	{
		cardCopy ( *unshuffled, *shuffled );
		(*shuffled).setLocation ( "shuffled" );
		unshuffled++;
		shuffled++;
	}

	shuffled = home;

	//Shuffled the cards in place
	for ( int j = 0; j < 5000; j++ )
	{
		Card* temp1;
		Card* temp2;
		int rand1 = rand (  ) % 52;
		int rand2 = rand (  ) % 52;
		for ( int k = 0; k < 52; k++ )
		{
			if ( k == rand1 )
				temp1 = shuffled;
			if ( k == rand2 )
				temp2 = shuffled;
			shuffled++;
		}
		shuffled = home;

		swap ( *temp1, *temp2 );
		temp1 = temp2 = NULL;
	}

	shuffled = home;
	for ( int l = 0; l < 51; l++ )
	{
		cardCopy ( *shuffled, *stock );
		(*stock).setLocation ( "stock" );
		cout << (*stock).getSuit (  ) << " " << (*stock).getRank (  ) << endl;
		shuffled++;
		stock++;
	}

	cardCopy ( *shuffled, *discard );
	cout << (*discard).getSuit (  ) << " " << (*discard).getRank (  ) << endl;
}


/**
  *Copies the data in source into dest
  */
void cardCopy ( Card& source, Card& dest )
{
	//Sets the rank
	dest.setRank ( source.getRank (  ) );

	//Sets the location
	dest.setLocation ( source.getLocation (  ) );

	//Sets the suit
	dest.setSuit ( source.getSuit (  ) );
}

/**
  *Swaps the values in card1 and card2
  */
void swap ( Card& card1, Card& card2 )
{
	//Variables
	Card temp;

	//Copies card2 into temp
	cardCopy ( card2, temp );

	//Copies card1 into card2
	cardCopy ( card1, card2 );

	//Copies temp into card1
	cardCopy ( temp, card1 );
}
