#ifndef EIGHTS_H
#define EIGHTS_H
#include <stdlib.h>

class Card
{
	public:
		Card ( int = 0, char* = NULL, char* = NULL );
		~Card (  );
		void setRank ( int );
		int getRank (  );
		void setSuit ( char* );
		void setSuit ( const char* );
		char* getSuit (  );
		void setLocation ( char* );
		void setLocation ( const char* );
		char* getLocation (  );

	private:
		int rank;
		char* location;
		char* suit;
};

class Player
{
	public:
		Player (  );
		~Player (  );
		void setName ( char* );
		void setName ( const char* );
		char* getName (  );
		void setID ( int* );
		int* getID (  );

	private:
		char* name;
		int* id;
};





#endif
