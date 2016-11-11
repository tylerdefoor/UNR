#include "listA.h"
#include <iostream>

using namespace std;

/**
  *Default constructor for List
  */
List::List ( int newSize )
{
	size = newSize;
	actual = cursor = -1;
	data = new char[size];
}

/**
  *Copy constructor for class List
  */
List::List ( const List& source )
{
	size = source.size;
	actual = source.actual;
	cursor = source.cursor;
	data = new char[size];
	for ( int i = 0; i < source.actual; i++ )
	{
		data[i] = source.data[i];
	}
}

/**
  *Destructor for class List
  */
List::~List (  )
{
	size = actual = cursor = -1;
	delete[] data;
}

/**
  *Sets the cursor to the beginning of the list
  *Returns false if the list is empty
  */
bool List::gotoBeginning (  )
{
	if ( empty (  ) )
		return false;
	else
	{
		cursor = 0;
		return true;
	}
}

/**
  *Moves the cursor to the last value in the list
  *Cannot be done if the list is empty
  */
bool List::gotoEnd (  )
{
	if ( empty (  ) )
		return false;
	else if ( cursor == actual - 1 )
		return true;
	else
	{
		cursor = actual - 1;
		return true;
	}
}

/**
  *Moves the cursor to the next value in the list
  *Cannot be done if the list is empty
  *Cannot be done if the cursor is at the last item in the list
  */
bool List::gotoNext (  )
{
	if ( empty (  ) || cursor == actual - 1 )
		return false;
	else
	{
		cursor++;
		return true;
	}
}

/**
  *Moves the cursor to the previous value in the list
  *Cannot be done if the list is empty
  *Cannot be done if the cursor is at 0
  */
bool List::gotoPrior (  )
{
	if ( empty (  ) || cursor == 0 )
		return false;
	else
	{
		cursor--;
		return true;
	}
}

/**
  *Inserts source after the cursor
  *Cannot be done if the list is full
  */
bool List::insertAfter ( char source )
{
	if ( empty (  ) )
	{
		cursor = 0;
		actual = 1;
		data[cursor] = source;
		return true;
	}
	else if ( full (  ) )
		return false;
	else
	{
		if ( cursor = actual - 1 )
		{
			cursor++;
			data[cursor] = source;
			actual++;
			return true;
		}
		else
		{
			int temp = cursor + 1;
			gotoEnd (  );
			while ( cursor != temp )
			{
				char cTemp = data[cursor];
				data[cursor] = data[cursor - 1];
				cursor--;
				data[cursor] = temp;
			}
			data[cursor] = source;
			actual++;
			return true;
		}
	}
}

/**
  *Inserts source before the cursor
  *Cannot be done if the list is full
  */
bool List::insertBefore ( char source )
{
	if ( empty (  ) )
	{
		cursor = 0;
		actual = 1;
		data[cursor] = source;
		return true;
	}
	else if ( full (  ) )
		return false;
	else
	{
		int temp;
		if ( cursor == 0 )
			temp = cursor;
		else
			temp = cursor - 1;
		gotoEnd (  );
		while ( cursor != temp )
		{
			data[cursor + 1] = data[cursor];
			cursor--;
		}
		data[cursor + 1] = data[cursor];
		data[cursor] = source;
		actual++;
		return true;
	}
}

/**
  *Removes the data at the cursor from the list
  *Cannot be done if the list is empty
  */
bool List::remove ( char& dest )
{
	if ( empty (  ) )
	{
		return false;
	}
	else
	{
		int temp = cursor;
		dest = data[cursor];
		if ( cursor == actual - 1 )
		{
			actual--;
			cursor = 0;
		}
		else
		{
			while ( cursor != actual - 1 )
			{
				data[cursor] = data[cursor + 1];
				cursor++;
			}
		}
		return true;
	}
}

/**
  *Replaces the element with the current cursor with source
  *Cannot be done if the list is empty
  */
bool List::replace ( char source )
{
	if ( empty (  ) )
		return false;
	else
	{
		data[cursor] = source;
		return true;
	}
}

/**
  *Returns the element at the cursor
  *Cannot be done if the list is empty
  */
bool List::getCursor ( char& dest ) const
{
	if ( empty (  ) )
		return false;
	else
	{
		dest = data[cursor];
		return true;
	}
}


























