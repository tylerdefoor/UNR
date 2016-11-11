#include <stdexcept>
#include <iostream>
#include "QueueLinked.h"

using namespace std;


template <typename DataType>
QueueLinked<DataType>::QueueLinked ( int maxNumber )
{
	front = back = NULL;
}

template <typename DataType>
QueueLinked<DataType>::QueueLinked ( const QueueLinked& other )
{
	if ( other.isEmpty (  ) )
		front = back = NULL;
	else
	{
		front = new QueueNode ( other.front->nodeData, NULL );
		QueueNode* temp = front;
		QueueNode* oTemp = other.front;
		while ( oTemp->next != NULL )
		{
			oTemp = oTemp->next;
			temp->next = new QueueNode ( oTemp->nodeData, NULL );
			temp = temp->next;
		}
		back = temp;
	}
}

template <typename DataType>
QueueLinked<DataType>& QueueLinked<DataType>::operator= ( const QueueLinked& other )
{
	if ( this != &other )
	{
		clear (  );
		front = new QueueNode ( other.front->nodeData, NULL );
		QueueNode* temp = front;
		QueueNode* oTemp = other.front;
		while ( oTemp->next != NULL )
		{
			oTemp = oTemp->next;
			temp->next = new QueueNode ( oTemp->nodeData, NULL );
			temp = temp->next;
		}
		back = temp;
	}
	return this;
}

template <typename DataType>
QueueLinked<DataType>::~QueueLinked (  ) 
{
	clear (  );
}

template <typename DataType>
void QueueLinked<DataType>::enqueue ( const DataType& newDataItem ) throw (logic_error)
{
	if ( isFull (  ) )
	{
		throw logic_error ( "enqueue() while queue full" );
	}
	else
	{
		if ( isEmpty (  ) )
		{
			front = back = new QueueNode ( newDataItem, NULL );
		}
		else
		{
			back->next = new QueueNode ( newDataItem, NULL );
			back = back->next;
		}
	}
}

template <typename DataType>
DataType QueueLinked<DataType>::dequeue (  ) throw ( logic_error )
{
	if ( isEmpty (  ) )
		throw logic_error ( "dequeue() while queue empty" );
	else
	{
		DataType dTemp = front->dataItem;
		if ( front == back )
		{
			delete front;
			front = back = NULL;
		}
		else
		{
			QueueNode* temp = front;
			front = front->next;
			delete front;
		}
		return dTemp;
	}
}

template <typename DataType>
void QueueLinked<DataType>::clear (  )
{
	if ( !isEmpty (  ) )
	{
		QueueNode* temp = front->next;
		delete front;
		front = temp;
		while ( front != NULL )
		{
			temp = temp->next;
			delete front;
			front = temp;
		}
		front = back = NULL;
	}
}

template <typename DataType>
bool QueueLinked<DataType>::isEmpty (  ) const
{
	return ( front == NULL );
}

template <typename DataType>
bool QueueLinked<DataType>::isFull (  ) const
{
	return ( 3 == 4 );
}

template <typename DataType>
void QueueLinked<DataType>::putFront ( const DataType& newItem ) throw ( logic_error )
{
	if ( isFull (  ) )
		throw logic_error ( "putFront() while queue full" );
	else
	{
		if ( isEmpty (  ) )
			front = back = new QueueNode ( newItem, NULL );
		else
		{
			QueueNode* temp = front;
			front = new QueueNode ( newItem, temp );
		}
	}
}


template <typename DataType>
DataType QueueLinked<DataType>::getRear (  ) throw ( logic_error )
{
	if ( isEmpty (  ) )
		throw logic_error ( "getRear() while queue empty" );
	else
	{
		DataType dTemp = back->dataItem;
		if ( back != front )
		{
			QueueNode* temp = front;
			while ( temp->next != back )
				temp = temp->next;

			delete back;
			back = temp;
		}
		else
		{
			delete back;
			back = front = NULL;
		}
		return dTemp;
	}
}

template <typename DataType>
int QueueLinked<DataType>::getLength (  ) const
{
	if ( isEmpty (  ) )
		return 0;
	else
	{
		int length = 0;
		QueueNode* temp = front;
		while ( temp != NULL )
		{
			length++;
			temp = temp->next;
		}
		return length;
	}
}

template <typename DataType>
void QueueLinked<DataType>::showStructure () const

// Linked list implementation. Outputs the elements in a queue. If
// the queue is empty, outputs "Empty queue". This operation is
// intended for testing and debugging purposes only.

{
    // Iterates through the queue
	QueueNode* p;

    if ( isEmpty() )
	cout << "Empty queue" << endl;
    else
    {
	cout << "Front\t";
	for ( p = front ; p != NULL ; p = p->next )
	{
	    if( p == front )
	    {
		cout << '[' << p->dataItem << "] ";
	    }
	    else
	    {
		cout << p->dataItem << " ";
	    }
	}
	cout << "\trear" << endl;
    }
}

template <typename DataType>
QueueLinked<DataType>::QueueNode::QueueNode ( const DataType& nodeData, QueueNode* nextPtr )
{
	dataItem = nodeData;
	next = nextPtr;
}
