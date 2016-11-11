/** @file WeightedGraph.cpp
 * @brief Lab 12 - Weighted Graph
 * An implementation of a Weighted Graph
 * @author Tyler DeFoor
 * @date 11/20/2014
 * @version 1.0
 */
 
 #include <stdexcept>
 #include <iostream>
 #include <climits>
 #include <string>
 #include "WeightedGraph.h"
 
 using namespace std;
 
 /**Constructor
 *@fn WeightedGraph::WeightedGraph ( int maxNumber )
 *@brief Default constructor for class WeightedGraph
 *@param maxNumber The maximum number of vertices
 *@return N/A
 *@pre N/A
 *@post A WeightedGraph is constructed
 */
WeightedGraph::WeightedGraph ( int maxNumber )
{
	maxSize = maxNumber;
	size = 0;
	vertexList = new Vertex[maxNumber];
	adjMatrix = new int[maxNumber * maxNumber];
	pathMatrix = new int[maxNumber * maxNumber];
}

/**Copy Constructor
 *@fn WeightedGraph::WeightedGraph ( const WeightedGraph& other )
 *@brief Copy constructor for class WeightedGraph
 *@param other The WeightedGraph to be copied
 *@return N/A
 *@pre other is instantiated
 *@post The current WeightedGraph is instantiated as an exact copy of other
 */
WeightedGraph::WeightedGraph ( const WeightedGraph& other )
{
	*this = other;
}

/**Overloaded assignment operator
 *@fn WeightedGraph& WeightedGraph::operator= ( const WeightedGraph& other )
 *@brief Overloaded assignment operator for class WeightedGraph
 *@param other The WeightedGraph to be copied from
 *@return WeightedGraph&
 *@pre Both this and other are instantiated
 *@post Both this and other are equal
 */
WeightedGraph& WeightedGraph::operator= ( const WeightedGraph& other )
{
	if ( this != &other )
	{
		if ( !isEmpty (  ) )
		{
			delete[] vertexList;
			delete[] adjMatrix;
			delete[] pathMatrix;
		}
		
		maxSize = other.maxSize;
		size = other.size;
		vertexList = new Vertex[maxSize];
		adjMatrix = new int[maxSize * maxSize];
		pathMatrix = new int[maxSize * maxSize];
		
		//Loop through the list
		for ( int i = 0; i < maxSize; i++ )
		{
			//Set the vertex matrix equal to each other
			vertexList[i] = other.vertexList[i];
			
			//Loop through the columns
			for ( int j = 0; j < maxSize; j++ )
			{
				//The current position is given by ( i * maxSize ) + j
				int position = ( i * maxSize ) + j;
				
				//Set the path and adjacency matrices equal to eachother
				adjMatrix[position] = other.adjMatrix[position];
				pathMatrix[position] = other.pathMatrix[position];
			}
		}
	}
	
	return *this;
}
 
 /**Destructor
 *@fn WeightedGraph::~WeightedGraph (  )
 *@brief Destructor for class WeightedGraph
 *@param N/A
 *@return N/A
 *@pre The current WeightedGraph is instantiated
 *@post There is no significant data in the WeightedGraph
 */
WeightedGraph::~WeightedGraph (  )
{
	size = 0;
	delete[] pathMatrix;
	delete[] adjMatrix;
	delete[] vertexList;
}

 /**InsertVertex
 *@fn void WeightedGraph::insertVertex ( const Vertex& newVertex ) throw ( logic_error )
 *@brief Inserts newVertex into the graph
 *@param newVertex The vertex to be inserted
 *@return void
 *@pre The current WeightedGraph is instantiated
 *@throw logic_error
 *@post newVertex is inserted into the WeightedGraph
 */
void WeightedGraph::insertVertex ( const Vertex& newVertex ) throw ( logic_error )
{
	//Loop through the list
	for ( int i = 0; i < size; i++ )
	{
		//If the vertex is already in the list
		if ( vertexList[i].getLabel (  ) = newVertex.getLabel (  ) )
		{
			for ( int j = 0; j < size; j++ )
				setEdge ( i, j, INFINITE_EDGE_WT );
			
			vertexList[i] = newVertex;
			return;
		}
	}
	
	if ( isFull (  ) )
		throw logic_error ( "Graph is full" );
		
	vertexList[size] = newVertex;
	size++;
	for ( int i = 0; i < size; i++ )
		setEdge ( size, i, INFINITE_EDGE_WT );
}

 /**InsertEdge
 *@fn void WeightedGraph::insertEdge ( const string& v1, const string& v2, int wt ) throw ( logic_error )
 *@brief Inserts an edge between two vertices
 *@param v1 The first vertex
 *@param v2 The second vertex
 *@param wt The weight of the edge
 *@return void
 *@pre WeightedGraph is instantiated
 *@post An edge is inserted between v1 and v2 with weight wt
 *@throw logic_error
 */
void WeightedGraph::insertEdge ( const string& v1, const string& v2, int wt ) throw ( logic_error )
{
	int v1Index = -1;
	int v2Index = -1;
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		//If we found the first vertex
		if ( vertexList[i].getLabel (  ) == v1 )
			//Store the location
			v1Index = i;
		
		//If we found the second vertex
		if ( vertexList[i].getLabel (  ) == v2 )
			//Store the location
			v2Index = i;
	}
	if ( v1Index == -1 || v2Index == -1 )
		throw logic_error ( "Vertex not found " );
		
	printf ( "Size = %i , idx_v1 = %i , idx_v2 = %i /n" , size, v1Index, v2Index );
	setEdge ( v1Index, v2Index, wt );
}

 /**RetrieveVertex
 *@fn bool WeightedGraph::retrieveVertex ( const string& v, Vertex& vData ) const
 *@brief Checks to see whether or not the vertex is in the WeightedGraph
 *@param v The string to search for
 *@param vData The data from the returned vertex
 *@return bool
 *@pre The current WeightedGraph is instantiated
 *@post The WeightedGraph is unchanged
 */
bool WeightedGraph::retrieveVertex ( const string& v, Vertex& vData ) const
{
	//Loop through the list
	for ( int i = 0; i < size; i++ )
	{
		//If we found the string
		if ( vertexList[i].getLabel (  ) == v )
		{
			//Put the data in vData
			vData = vertexList[i];
			
			//Return true
			return true;
		}
	}
	
	//If we get here, we didn't find it. So we return false
	return false;
}

 /**GetEdgeWeight
 *@fn bool WeightedGraph::getEdgeWeight ( const string& v1, const string& v2, int& wt ) const throw ( logic_error )
 *@brief Gets the weight of the edge between v1 and v2
 *@param v1 The first vertex
 *@param v2 The second vertex
 *@param wt The returned weight of the vertex
 *@return bool
 *@pre The current WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 *@throw logic_error
 */
bool WeightedGraph::getEdgeWeight ( const string& v1, const string& v2, int& wt ) const throw ( logic_error )
{
	int v1Index = -1;
	int v2Index = -1;
	
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		//If we find the first vertex
		if ( vertexList[i].getLabel (  ) == v1 )
			//Save the index
			v1Index = i;
			
		//If we find the second vertex
		if ( vertexList[i].getLabel (  ) == v2 )
			//Save the index
			v2Index = i;
	}
	
	//If we did not find the indices
	if ( v1Index == -1 || v2Index == -1 )
		//Throw an error
		throw logic_error ( "Not found" );
		
	else
	{
		//The position of the edge in the adjacency matrix is 
		//	( v1Index * maxSize ) + v2Index because math
		int position = ( v1Index * maxSize ) + v2Index;
		
		//If the edge is not of infinite weight
		if ( adjMatrix[position] != INFINITE_EDGE_WT )
		{
			//Save the weight and return true
			wt = adjMatrix[position];
			return true;
		}
		else
			//Otherwise return false
			return false;
	}
}
 
 /**RemoveVertex
 *@fn void WeightedGraph::removeVertex ( const string& v ) throw ( logic_error )
 *@brief Removes the vertex with label v1
 *@param v The label of the vertex to be removed
 *@return void
 *@pre A WeightedGraph is instantiated
 *@post The vertex with label v is removed
 *@throw logic_error
 */
void WeightedGraph::removeVertex ( const string& v ) throw ( logic_error )
{
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		//If we find the vertex
		if ( vertexList[i].getLabel (  ) == v )
		{
			//Loop through the remaining columns
			for ( int j = i; j < size - 1; j++ )
			{
				for (  int k = 0; k < size; k++ )
				{
					//Swap the current column with the next column
					int col = j + k * maxSize;
					int swap = j + 1 + k * maxSize;
					adjMatrix[col] = adjMatrix[swap];
				}
			}
			//Loop through the remaining rows
			for ( int j = 0; j < size - 1; j++ )
			{
				for ( int k = i; k <  size; k++ )
				{
					//Swap the current row with the next row
					int row = j + k * maxSize;
					int row = j + ( k + 1 ) * maxSize;
					adjMatrix[row] = adjMatrix[swap];
				}
			}
			
			for ( int j = i; j < size; j++ )
				vertexList[j] = vertexList[j+1];
				
			size--;
			return;
		}
	}
}
 
 /**RemoveEdge
 *@fn void WeightedGraph::removeEdge ( const string& v1, const string& v2 ) throw ( logic_error )
 *@brief Removes the edge between v1 and v2
 *@param v1 The first vertex
 *@param v2 The second vertex
 *@return void
 *@pre The current WeightedGraph is instantiated
 *@post The edge between v1 and v2 is removed
 *@throw logic_error
 */
void WeightedGraph::removeEdge ( const string& v1, const string& v2 ) throw ( logic_error )
{
	int v1Index = -1;
	int v2Index = -1;
	
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		//If we find v1
		if ( vertexList[i].getLabel (  ) == v1 )
			//Save the index
			v1Index = i;
			
		//If we find the second vertex
		if ( vertexList[i].getLabel (  ) == v2 )
			//Save the index
			v2Index = i;
	}
	
	//If we didn't find the vertices
	if ( v1Index == -1 || v2Index == -1 )
		//Throw a logic_error
		throw logic_error ( "Not Found" );
		
	else
	{
		//The two positions in the list are given by
		//	( row * maxNumber ) + column
		int position1 = ( v1Index * maxSize ) + v2Index;
		int position2 = ( v2Index * maxSize ) + v1Index;
		
		//Set the edges to infinte
		adjMatrix[position1] = INFINITE_EDGE_WT;
		adjMatrix[position2] = INFINITE_EDGE_WT;
	}
}
 
 /**Clear
 *@fn void WeightedGraph::clear (  )
 *@brief Clears the WeightedGraph of all significant data
 *@param N/A
 *@return void
 *@pre The current WeightedGraph is instantiated
 *@post The WeightedGraph is cleared of all significant data
 */
void WeightedGraph::clear (  )
{
	size = 0;
}
 
 /**IsEmpty
 *@fn bool WeightedGraph::isEmpty (  ) const
 *@brief Checks to see whether or not the WeightedGraph has any significant data
 *@param N/A
 *@return bool
 *@pre The WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */
bool WeightedGraph::isEmpty (  ) const
{
	return ( size == 0 );
}
 
 /**IsFull
 *@fn bool WeightedGraph::isFull (  ) const
 *@brief Checks to see whether or not the WeightedGraph is full
 *@param N/A
 *@return bool
 *@pre The WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */
bool WeightedGraph::isFull (  ) const
{
	return ( size == maxSize );
}
 
 /**ShowShortestPaths
 *@fn void WeightedGraph::showShortestPaths (  ) const
 *@brief Loops through all of the vertices and checks to see where the shortest paths are
 *@param N/A
 *@return void
 *@pre The WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */
void WeightedGraph::showShortestPaths (  ) const
{
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		//Loop through the rest of the vertices
		for ( int j = 0; j < size; j++ )
			//Set the path to the edge weight
			setPath ( i, j, getEdge ( i, j ) );
		
		//Set the path to itself as 0
		setPath ( i, i, 0 );
	}
	
	//Loop through the path array three times
	for ( int k = 0; k < size; k++ )
	{
		for ( int l = 0; l < size; l++ )
		{
			for ( int m = 0; m < size; m++ )
			{
				//If the path from l to k plus the path from k to m is less than 
				//	the path from l to m
				if ( getPath ( l, k ) + getPath ( k, m ) < getPath ( l, m ) || 
						getPath ( l, m )  )
				{
					//Change the path
					setPath ( j, k, ( getPath ( l, k ) + getPath ( k, m ) ) );
				}
			}
		}
	}
	
	//Pretty stuff to output the matrix
	cout << endl << "Path matrix : " << endl << '\t';
    for ( int col = 0 ; col < size ; col++ )
        cout << col << '\t';
    cout << endl;
    for ( int row = 0 ; row < size ; row++ )
    {
        cout << row << '\t';
        for ( int col = 0 ; col < size ; col++ )
        {
            int wt = getPath(row,col);
            if ( wt == INFINITE_EDGE_WT )
               cout << "- \t";
            else
               cout << wt << '\t';
        }
        cout << endl;
    }
}
 
 /**HasProperColoring
 *@fn bool WeightedGraph::hasProperColoring (  ) const
 *@brief Checks to see if the graph has two colors next to each other
 *@param N/A
 *@return bool
 *@pre The WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */
bool WeightedGraph::hasProperColoring (  ) const
{
	//Loop through the list with two iterators
	for ( int i = 0; i < size; i++ )
	{
		for ( int j = 0; j < size; j++ )
		{
			//If there is an edge, it is the same color and it is not the same vertex
			if ( getEdge ( i, j ) != INFINITE_EDGE_WT &&
				 vertexList[i].getColor (  ) == vertexList[j].getColor (  ) &&
				 i != j )
			{
				//Return false
				return false
			}
		}
	}
	
	//If we get here, return true
	return true;
}
 
 /**AreAllEven
 *@fn bool WeightedGraph::areAllEven (  ) const
 *@brief Checks to see if all of the edges are event_callback
 *@param N/A
 *@return bool
 *@pre A WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */
bool WeightedGraph::areAllEven (  ) const
{
	int count = 0;
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		for ( int j = 0; j < size; j++ )
		{
			//If the vertex has an edge with another vertex
			if ( adjMatrix [ ( i * maxSize ) + j ] != INFINITE_EDGE_WT )
				//Increment count
				count++;
		}
		
		//If the count is not even
		if ( count % 2 != 0 )
			//Return false
			return false;
			
		//Reset count
		count = 0;
	}
	//If we reach here, it is all even
	return true;
}
 
 /**GetIndex
 *@fn int WeightedGraph::getIndex ( const string& v ) const
 *@brief Returns the index of the vertex with label v
 *@param v The label of the vertex
 *@return int
 *@pre A WeightedGraph is constructed
 *@post Nothing in the WeightedGraph is changed
 */
int WeightedGraph::getIndex ( const string& v ) const
{
	//Loop through the vertexList
	for ( int i = 0; i < size; i++ )
	{
		//If we found the vertex
		if ( vertexList[i].getLabel (  ) == v )
			//Return the current index
			return i;
	}
	
	//If we didn't find it, return -1
	return -1;
}
 
 /**GetEdge
 *@fn int WeightedGraph::getEdge ( int row, int col ) const
 *@brief Returns the edge at row, col
 *@param row The row of the edge
 *@param col The column of the edge
 *@return int
 *@pre The WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */ 
int WeightedGraph::getEdge ( int row, int col ) const
{
	return ( adjMatrix[ ( row * maxSize ) + col ] );
} 
 
 /**SetEdge
 *@fn void WeightedGraph::setEdge ( int row, int col, int wt )
 *@brief Sets the edge at row, col
 *@param row The row of the new edge
 *@param col The column of the new edge
 *@param wt The weight of the new edge
 *@return void
 *@pre The WeightedGraph is instantiated
 *@post Nothing in the WeightedGraph is changed
 */
void WeightedGraph::setEdge ( int row, int col, int wt )
{
	adjMatrix [ ( row * maxSize ) + col ] = wt;
	adjMatrix [ ( col * maxSize ) + row ] = wt;
}





 
 
 
 
 