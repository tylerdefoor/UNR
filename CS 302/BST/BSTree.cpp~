#include "Tree.h"
#include <stdexcept>
#include <iostream>

using namespace std;

template <typename DataType, class KeyType>
BSTree<DataType, KeyType>::BSTreeNode::BSTreeNode ( const DataType &nodeDataItem, BSTreeNode *leftPtr, BSTreeNode *rightPtr )
{
	dataItem = nodeDataItem;
	left = leftptr;
	right = rightptr;
}

template <typename DataType, class KeyType>
BSTree<DataType, KeyType>::BSTree (  )
{
	root = NULL;
}

template <typename DataType, class KeyType>
BSTree<DataType, KeyType>::BSTree ( const BSTree<DataType, KeyType>& other )
{
	if ( other.root == NULL )
		root = NULL;
	else
	{
		copy ( root, other.root );	
	}	
}

template <typename DataType, class KeyType>
BSTree<DataType, KeyType>::copy ( BSTreeNode*& source, BSTreeNode* other )
{
	if ( other == NULL )
		source = NULL;
	else
	{
		current = new BSTreeNode ( other.dataItem, NULL, NULL );
		copy ( source->left, other->left );
		copy ( source->right, other->right );
	}
}

template <typename DataType, class KeyType>
BSTree<DataType, KeyType>& BSTree<DataType, KeyType>::operator= ( const BSTree& other )
{
	if ( this != source) 
	{
		clear ( );
		if ( other.root != NULL )
			copy ( root, other.root );
	}
	return *this;
}

template <typename DataType, class KeyType>
BSTree<DataType, KeyType>::~BSTree (  )
{
	destroy ( root );
}

template <typename DataType, class KeyType>
void BSTree<DataType, KeyType>::destroy ( BSTreeNode* source )
{
	if ( source != NULL )
	{
		destroy ( source->left );
		destroy ( source->right );
		delete source;
	}
}

template <typename DataType, class KeyType>
void BSTree<DataType, KeyType>::insert ( const DataType& newDataItem )
{
	insertHelper ( newDataItem, root );
}

template <typename DataType, class KeyType>
void BSTree<DataType, KeyType>::insertHelper ( const DataType& newData, BSTreeNode* other )
{
	//If we have reached a NULL node, we insert the data
	if ( other == NULL )
		other = new BSTreeNode ( newData, NULL, NULL );

	//If the data is greater than the node data, send it right
	if ( newData > other->dataItem )
		insertHelper ( newData, other->right );

	//Otherwise, the data is less than the node data so we send it left
	else
		insertHelper ( newData, other->left );
}
