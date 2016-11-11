#include <iostream>
#include <cstdio>
#include "ExpressionTree.h"
#include <vector>

using namespace std;

ExprTreeNode::ExprTreeNode ( char elem, ExprTreeNode* leftPtr, ExprTreeNode* rightPtr )
{
	dataItem = elem;
	left = leftPtr;
	right = rightPtr;
}

ExprTree::ExprTree (  )
{
	root = NULL;
}

ExprTree::ExprTree ( const ExprTree& source )
{
	if ( source.root == NULL )
		root = NULL;
	else
	{
		copy ( root, source.root );			
	}
}

ExprTree& ExprTree::operator= ( const ExprTree& source )
{
	if ( this != &source )
	{
		clear (  );
		if ( source.root != NULL )
		{
			copy ( root, source.root );
		}
	}
	return *this;
}

void ExprTree::copy ( ExprTreeNode* currentNode, ExprTreeNode* otherNode )
{
	if ( otherNode == NULL )
		currentNode = NULL;
	else
	{
		currentNode = new Node ( otherNode->dataItem, NULL, NULL );
		copy ( currentNode->left, currentNode->left );
		copy ( currentNode->right, currentNode->right );
	}
}

ExprTree::~ExprTree (  )
{
	destroy ( root );
}

void ExprTree::destroy ( ExprTreeNode* current )
{
	if ( current != NULL )
	{
		destroy ( current->left );
		destroy ( current->right );
		delete current;
	}
}

void ExprTree::build (  )
{
	char temp;
	while ( true )
	{
		cin >> temp;
	}
}
