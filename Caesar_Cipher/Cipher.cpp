/*
/@Author Tyler DeFoor
/@Version 1.2
/@Date 3/5/2016
/@Brief A program that allows encryption via a Caesar Cipher
*/


/*================================
=            Includes            =
================================*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>

/*=====  End of Includes  ======*/

using namespace std;

/*======================================
=            Method Headers            =
======================================*/

void openfile (vector <int> &plaintext, string filename);
void encode (vector <int> plaintext, vector <char> &encrypted);
void decode (vector <int> encrypted, vector <char> &plaintext, int key);
void outfileEncrypt (vector <char> text);
void outfileDecrypt (vector <int>  encrypted);

/*=====  End of Method Headers  ======*/

 /**
*@fn int main (int argc, char* argv[])
*@brief Main program driver
*@param argc The number of arguments
*@param argv The array of characters passed into the method
*/

int main (int argc, char* argv[])
{	
    //Seed the random number generator
	srand (time(NULL));
	
    //Get input from the user
	cout << "1. Encode" << endl << "2. Decode" <<endl;
	
	int input;
	
	cin >> input;

    //Switch case for the input
	switch (input)
    {
        //If we are encoding
		case 1:
        {
            //Create vectors to hold plaintext and encrypted text
			vector <int> plaintext;
			vector <char> encrypted;

            //Open the location of the plaintext
			openfile(plaintext, "Plaintext");

            //Encode the plaintext
			encode (plaintext, encrypted);

            //Write the encrypted text to a file
			outfileEncrypt (encrypted);
			break;
		}

		//If we are decoding	
		case 2:
        {
            //Create vectors to hold the plaintext and encrypted text
			vector < vector <char> > plaintext;
			vector <int> encrypted;

            //Open the file that holds the encrpted results
			openfile(encrypted,"EncodedResults");
			outfileDecrypt (encrypted);
			break;
		}
	}
    
    //Return a success	
	return 0;
}

 /**
*@fn void openfile (vector <int> &text, string filename) 
*@brief Opens the given file 
*@param text The text in the file
*@param filename The name of the file
*@pre The file is not open
*@post The file is closed and none of the contents have changed
*/
void openfile ( vector <int> &text, string filename )
{
/*============================================
=            Variable Declaration            =
============================================*/

	ifstream fin;
	char current;

/*=====  End of Variable Declaration  ======*/

    //Open the file
    fin.open ( filename.c_str (  ) );
    
	//While we are not at the end of the file
	while (fin >> current)
    {
        //Push the characters onto the vector
		text.push_back ( current );	
    }
}

 /**
*@fn void encode (vector <int> plaintext, vector <char> &encrypted)
*@brief Encodes the plaintext using a random offset
*@param 
*@pre 
*@post 
*/
void encode (vector <int> plaintext, vector <char> &encrypted)
{
    //Get the randomly generated keys
	int key = (rand() % 93); 

    //Go through the entire plaintext
	for ( int i = 0; i < plaintext.size (  ); i++ )
    {
        //Encrypt the data with the key and push onto the enrypted vector
		encrypted.push_back ( ( plaintext [ i ] + key % 93 ) +33 );
	}
}

 /**
*@fn void decode (vector <int> encrypted, vector <char> &plaintext, int key)
*@brief Decodes the encrypted text
*@param encrypted The encrypted text
*@param plaintext The plaintext
*@param key The key to the cipher
*@pre   There is text in encrypted
*@post  Nothing in encrypted is changed. plaintext holds the plaintext
*/
void decode (vector <int> encrypted, vector <char> &plaintext, int key)
{
    //Loop through the encrypted text	
	for (int i = 0; i < encrypted.size(); i++)
    {
        //Decrypt the text and put it on plaintext
		plaintext.push_back ( ( encrypted [ i ] - key % 93 ) );
	}
}

 /**
*@fn    void outfileEncrypt (vector <char> text)
*@brief Output the encrypted text to a file
*@param text The text to output
*@pre   text has relevant data in it
*@post  Nothing in text is changed. EncodedResults holds the encoded plaintext
*/
void outfileEncrypt ( vector <char> text )
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/
    
	ofstream fout;
    
    /*=====  End of Variable Declaration  ======*/
    
    //Open the file EncodedResults
	fout.open ("EncodedResults");
	
    //Loop through the text
	for (int i = 0; i < text.size(); i++)
    {
        //Output it to the file
		fout << text[i];
	}
}

 /**
*@fn    void outfileDecrypt (vector <int> encrypted)
*@brief Output the decrypted results
*@param encrypted The encrypted text
*@pre   encrypted holds relevant information
*@post  Nothing in encrypted is changed. Decryption holds all possible texts
*/
void outfileDecrypt (vector <int> encrypted)
{
    /*============================================
    =            Variable Declaration            =
    ============================================*/
    
	ofstream fout;
    
    /*=====  End of Variable Declaration  ======*/
    
    //Open the file
	fout.open ( "Plaintexts" );
	
    //Loop through all the keys
    for ( int k = 0; k < 93; k++ )
    {
       //Vector to hold the plaintext
	   vector <char> plaintext;

       //Output the key
	   fout << "Key: " << k << endl;

       //Decode the plaintext with that key
	   decode (encrypted, plaintext, k+33);
	
       //Output the plaintext
	   for (int i = 0; i < plaintext.size(); i ++)
       {
            cout << plaintext[i];
            fout << plaintext[i];
	   }
	
	fout << endl << endl;
    }

    fout.close (  );
    
}