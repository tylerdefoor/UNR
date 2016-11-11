
bool forward ( int );
bool backward ( int  );
bool solve ( char[][6], int );
bool solved ( char[][6] );







int main (  )
{
	//Variables
	int scenario = 0;
	int max = 10;
	int lowest = 10;
	int current = 0;
	int numCars = -1;
	int grid[6][6];

	while ( true )
	{
		scenario++;
		cin >> numCars;
		if ( numCars == 0 )
			return 0;
		
		//Clear the grid and set all the spaces to empty
		for ( int i = 0; i < 6; i++ )
		{
			for ( int j = 0; j < 6; j++ )
				grid[i][j] = -1;
		}

		for ( int i = 0; i < numCars; i++ )
		{
			int length;
			int row;
			int col;
			char temp;
			cin >> length >> temp >> row >> col;
			if ( temp == 'H' )
			{
				for ( int j = 0; j < length; j++ )
					grid[row][col + j] = i;
			}
			else
			{
				for ( int k = 0; k < length; k++ )
					grid[row + k][col] = i;
			}
		}

	}
}
