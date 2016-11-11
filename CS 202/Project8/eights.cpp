#include "eights.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/**
  *Default constructor for class Card
  */
Card::Card ( int newRank, string newSuit, string newLocation )
{
	rank = newRank;
	suit = newSuit;
	location = newLocation;
}

/**
  *Copy constructor for class Card
  */
Card::Card ( const Card& source )
{
	rank = source.rank;
	suit = source.suit;
	location = source.location;
}

/**
  *Destructor for class Card
  */
Card::~Card (  )
{
	rank = 0;
	location = suit = "";
}

/**
  *Sets the rank
  */
void Card::setRank ( int source ) 
{
	if ( source > 0 && source < 14 )
	{
		rank = source;
	}
}

/**
  *Returns the rank
  */
int Card::getRank (  ) const
{
	return rank;
}

/**
  *Sets the suit
  */
void Card::setSuit ( string source )
{
	suit = source;
}

/**
  *Retuns the suit
  */
string Card::getSuit (  ) const
{
	return suit;
}

/**
  *Sets the card's location
  */
void Card::setLocation ( string source )
{
	location = source;
}

/**
  *Returns the card's location
  */
string Card::getLocation (  ) const
{
	return location;
}

/**
  *Overloaded greater than or equal to operator for class card
  *Returns true if the calling card's rank is greater than the parameter's rank
  */
bool Card::operator>= ( const Card& source )
{
	return ( rank == source.rank || rank > source.rank );
}

/**
  *Overloaded assignment operator for class Card
  */
Card& Card::operator= ( const Card& source )
{
	rank = source.rank;
	location = source.location;
	suit = source.suit;
	return *this;
}

/**
  *Overloaded less than or equal to operator for class card
  *Returns true if c1's rank is less than or equal to c2's rank
  */
bool operator<= ( const Card& c1, const Card& c2 )
{
	return ( c1.rank == c2.rank || c1.rank < c2.rank );
}

/**
  *Overloaded insertion operator for class Card
  */
ostream& operator<< ( ostream& cout, const Card& source )
{
	cout << endl << source.suit << " " << source.rank << " " << source.location << endl;
	return cout;
}

/**
  *Overloaded extraction operator for class Card
  */
ifstream& operator>> ( ifstream& fin, const Card& dest )
{
	char* temp = new char[30];
	fin >> temp;
	dest.suit = temp;
	fin >> dest.rank;
	return fin;
}

/**
  *Default constructor for class Player
  */
Player::Player (  )
{
	name = "name";
	id = new int[5];
	hand = new Card[5];
}

/**
  *Copy constructor for class Player
  */
Player::Player ( const Player& source )
{
	name = source.name;
	id = new int[5];
	hand = new Card[5];
	for ( int i = 0; i < 5; i++ )
	{
		id[i] = source.id[i];
		hand[i].suit = source.hand[i].suit;
		hand[i].rank = source.hand[i].rank;
		hand[i].location = source.hand[i].location;
	}
}

/**
  *Destructor for class Player
  */
Player::~Player (  )
{
	name = "";
	if ( id != NULL )
		delete[] id;
	if ( hand != NULL )
		delete[]hand;
	hand = NULL;
	id = NULL;
}

/**
  *Sets the name
  */
void Player::setName ( string source )
{
	name = source;
}

/**
  *Returns the name
  */
string Player::getName (  ) const
{
	return name;
}

/**
  *Sets the hand
  */
void Player::setHand ( Card* source )
{
	for ( int i = 0; i < 5; i++ )
	{
		hand[i] = source.hand[i];
	}
}

/**
  *Sets the ID
  */
void Player::setID ( int* source )
{
	for ( int  i = 0; i < 5; i++ )
	{
		id[i] = source.id[i];
	}
}

/**
  *Returns the ID
  */
int* Player::getID (  ) const
{
	return id;
}

/**
  *Overloaded insertion operator for class Player
  */
ostream& operator<< ( ostream& cout, const Player& source )
{
	cout << endl << source.name << " ";
	for ( int  i = 0; i < 5; i++ )
	{
		cout << source.id[i];
	}
	cout << endl;
	for ( int j = 0; j < 5; j++ )
	{
		cout << source.hand[j] << endl;
	}
}

/**
  *Overloaded extraction operator for class Player
  */
fstream& operator>> ( fstream& fin, const Player& dest )
{
	fin >> dest.name;
	fin.get (  );
	for ( int i = 0; i < 5; i++ )
	{
		char temp = fin.get (  );
		int itemp = temp - '0';
		dest.id[i] = itemp;
	}
}



































