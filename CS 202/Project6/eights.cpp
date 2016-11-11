#include "eights.h"
#include "string.h"
#include <iostream>

using namespace std;

/**
  *Default constructor for class Card
  */
Card::Card ( int newRank, char* newSuit, char* newLoc )
{
	//Copy the rank
	rank = newRank;

	//Copy the suit
	//If the suit is NULL, there is no reason to copy the suit
	if ( newSuit == NULL )
	{
		suit = new char[9];
		copy ( "suit", suit );
	}
	else
	{
		suit = new char[9];
		copy ( newSuit, suit );
	}

	//Copy the location
	//If the location is NULL, there is no reason to copy the location
	if ( newLoc == NULL )
	{
		location = new char[30];
		copy ( "location", location );
	}
	else
	{
		location = new char[30];
		copy ( newLoc, location );
	}
}

/**
  *Destructor for class Card
  */
Card::~Card (  )
{
	//Reset the rank
	rank = 0;

	//Delete the suit if the suit is not NULL 
	if ( suit != NULL )
		delete[] suit;

	//Delete the location if the location is not NULL
	if ( location != NULL )
		delete[] location;

	suit = location = NULL;
}

/**
  *Sets the rank to the parameter newRank
  *newRank has to be a value greater than 0 and less than 14
  */
void Card::setRank ( int newRank )
{
	if ( newRank > 0 && newRank < 14 )
		rank = newRank;
}

/**
  *Returns the rank
  */
int Card::getRank (  )
{
	return rank;
}

/**
  *Sets the suit to source
  */
void Card::setSuit ( char* source )
{
	if ( source == NULL )
		suit = NULL;
	else
	{
		if ( suit == NULL )
			suit = new char[9];

		copy ( source, suit );
	}
}

/**
  *Sets the suit to source
  */
void Card::setSuit ( const char* source )
{
	if ( source == NULL )
		suit = NULL;
	else
	{
		if ( suit == NULL )
			suit = new char[9];

		copy ( source, suit );
	}
}

/**
  *Returns the suit
  */
char* Card::getSuit (  )
{
	return suit;
}

/**
  *Sets the location to source
  */
void Card::setLocation ( char* source )
{
	if ( source == NULL )
		location = NULL;
	else
	{
		if ( location == NULL )
			location = new char[30];

		copy ( source, location );
	}
}

/**
  *Sets the location to source
  */

void Card::setLocation ( const char* source )
{
	if ( source == NULL )
		location = NULL;
	else
	{
		if ( location == NULL )
			location = new char[30];

		copy ( source, location );
	}
}

/**
  *Returns the location
  */
char* Card::getLocation (  )
{
	return location;
}

/**
  *Default constructor for class Player
  */
Player::Player (  )
{
	name = new char[30];
	copy ( "name", name );
	id = new int[5];
}

/**
  *Destructor for class Player
  */
Player::~Player (  )
{
	if ( name != NULL )
		delete[] name;
	name = NULL;

	if ( id != NULL )
		delete[] id;
	id = NULL;
}

/**
  *Sets name to source
  */
void Player::setName ( char* source )
{
	if ( source == NULL )
		name = NULL;
	else
	{
		if ( name == NULL )
			name = new char[30];

		copy ( source, name );
	}
}

/**
  *Sets name to source
  */
void Player::setName ( const char* source )
{
	if ( source == NULL )
		name = NULL;
	else
	{
		if ( name == NULL )
			name = new char[30];

		copy ( source, name );
	}
}

/**
  *Returns name
  */
char* Player::getName (  )
{
	return name;
}

/**
  *Sets the ID to source
  */
void Player::setID ( int* source )
{
	if ( source == NULL )
		id = NULL;
	else
	{
		if ( id == NULL )
			id = new int[5];
		for ( int i = 0; i < 5; i++ )
		{
			*id = *source;
			id++;
			source++;
		}
	}

}

/**
  *Returns the ID
  */
int* Player::getID (  )
{
	return id;
}
