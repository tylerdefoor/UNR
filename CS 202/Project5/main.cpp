#include "string.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main (  )
{
	char* c1 = NULL;
	copy ( "hello", c1 );
	cout << &c1 << endl;
	cout << c1 << endl;

	return 0;
}
