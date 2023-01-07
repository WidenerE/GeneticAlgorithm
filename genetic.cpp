#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

//Struct used to define each simulated organism
struct cell
{
	bool alive = false;

	//Number of neighbors
	int neighbors = 0;
	//How many generations the cell has survived
	int age = 0;
};

//Prototypes
void printBoard(cell board[20][20]);
void simulateGeneration(cell (&board)[20][20]);
void prepGeneration(cell (&board)[20][20]);
int countNeighbors(cell board[20][20], int row, int column);

int main(int argc, char* argv[])
{
	//Array containing all the cells
	cell board[20][20];
	//Number of generations to be simulated
	int genCount;
	//Used to read files containing boards of cells
	char temp;
	//Tracks the current row and column when reading in boards
	int row, column = 0;
	//Name of the external file
	string fileName = "";
	//Random number between 1 and 10 that determines if a cell will be placed in a given array space
	int chance;

	//Rules
	cout << "Artificial Life Simulation" << endl;
	cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
	cout << endl;

	cout << "Each cell with one or no neighbors dies, as if by loneliness." << endl;
	cout << "Each cell with four or more neighbors dies, as if by overpopulation." << endl;
	cout << "Each cell with two or three neighbors survives." << endl;
	cout << "Each cell with three neighbors becomes populated." << endl;
	cout << "Each cell that ages over 10 generations dies." << endl << endl;

	//Prompts user for total generations to simulate
	cout << "How many generations would you like to simulate? ";
	cin >> genCount;
	cin.clear();
	cin.ignore();
	

	//If a file is passed on the command line, load the data in the file
	if(argc > 2)
	{
		//Read in board from file
		try
		{
			fileName = argv[2];
			fstream inStream(fileName, ios::in);
		

			//Read char by char
			while(inStream.get(temp))
			{
				if(temp == 'X')
				{
					board[row][column].alive = true;
					column++;
				}
				//Reset counters at the end of every line
				else if(temp == '\n')
				{
					row++;
					column = 0;
				}
				else if(temp == '0')
					column++;
			}

			cout << endl << endl << endl;
		        cout << "Initial File Colony" << endl;

		}
		catch(std::ifstream::failure e)
               	{
                       	cout << "Failed To Open File!" << endl;
                }
		

	}
	//Otherwise, generate a random board
	else
	{
		//Random seed
		srand(time(NULL));

		//Randomly place cells in each array space
		for(int i = 0; i < 20; i++)
		{
			for(int j = 0; j < 20; j++)
			{
				
				chance = rand() % 10 + 1;

				if(chance > 7)
				{
					board[i][j].alive = true;
				}
			}
		}

		cout << endl << endl << endl;
	        cout << "Initial Random Colony" << endl;

	}

	//Display the initial board
	printBoard(board);
	
	//Loop through generation count and simulate and display each generation
	for(int i = 0; i < genCount; i++)
	{
		cout << endl << "Generation ";
		cout << i + 1;
		cout << endl << endl;
		
		simulateGeneration(board);

		printBoard(board);
	}
	
}

//Pre-Condition: An array of cells has been loaded from a file or randomly generated and passed in
//Post-Condition: The array is printed to the screen where an X represents a living cell and a 0 is an empty space
void printBoard(cell board[20][20])
{
	cout << "*****************************************************" << endl;

	//Loop through 2D array, check each space for a living cell, and print either an X or a 0
	for(int i = 0; i < 20; i++)
        {
                for(int j = 0; j < 20; j++)
                {
                        if(board[i][j].alive == true)
				cout << "X ";
			else
				cout << "0 ";
                }

		cout << endl;
        }


	cout << "*****************************************************" << endl;

}

//Pre-Condition: A cell board has been constructed
//Post-Condition: One full generation will have been simulated, with births and deaths occuring based on the given set of rules
void simulateGeneration(cell (&board)[20][20])
{
	//This method must occur before births and deaths can be calculated.
	//Counts each array index's living neighbors
	prepGeneration(board);

	//Row loop
	for(int i = 0; i < 20; i++)
        {
		//Column loop
                for(int j = 0; j < 20; j++)
                {
			//First, check the cell's age. If it is 10 or older, it dies.
                        if(board[i][j].age >= 10)
			{
				board[i][j].alive = false;
				board[i][j].age = 0;
			}
			//Otherwise, check the rest of the rules that depend upon the number of neighbors of the cell
			else
			{
				switch(board[i][j].neighbors)
				{
				case 0:
				case 1:
				//Cell dies with 1 or fewer neighbors
					if(board[i][j].alive == true)
					{
						board[i][j].alive = false;
						board[i][j].age = 0;
					}
					break;
				case 2:
				//2 Neighbors preserves the cell
					if(board[i][j].alive == true)
						board[i][j].age = board[i][j].age + 1;
					break;
				case 3:
				//3 allow a new cell to be born if the index is empty
                       		        if(board[i][j].alive == true)
                       	        	        board[i][j].age = board[i][j].age + 1;
					else
					{
						board[i][j].alive = true;
						board[i][j].age = 0;
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				//4 or more causes overcrowding and the cell dies
					if(board[i][j].alive == true)
					{
						board[i][j].alive = false;
						board[i][j].age = 0;
					}
					break;
				default:
					break;

				}
               		}
        	}


	}
}

//Pre-Condition: A generation is being simulated and needs to be prepped
//Post-Condition: Neighbors of all cells will be counted and saved for future reference
void prepGeneration(cell (&board)[20][20])
{
	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			board[i][j].neighbors = countNeighbors(board, i, j);
		}
	}

}

//Pre-condition: prepGeneration has been called
//Post-Condition: Checks to see if adjacent spaces have a living cell and returns the total
int countNeighbors(cell board[20][20], int row, int column)
{
	//Counts number of neighbors
	int count = 0;

	//X00
	//0I0
	//000
	if(row - 1 >= 0 && column - 1 >= 0 && board[row - 1][column - 1].alive == true)
		count++;

	//0X0
	//0I0
	//000
	if(row - 1 >= 0 && board[row - 1][column].alive == true)
                count++;

        //00X
        //0I0
        //000
        if(row - 1 >= 0 && column + 1 < 20 && board[row - 1][column + 1].alive == true)
                count++;

	//000
	//XI0
	//000
	if(column - 1 >= 0 && board[row][column - 1].alive == true)
                count++;

	//000
	//0IX
	//000
	if(column + 1 < 20 && board[row][column + 1].alive == true)
                count++;

	//000
	//0I0
	//X00
	if(row + 1 < 20 && column - 1 >= 0 && board[row + 1][column - 1].alive == true)
                count++;

	//000
	//0I0
	//0X0
	if(row + 1 < 20 && board[row + 1][column].alive == true)
                count++;

	//000
	//0I0
	//00X
	if(row + 1 < 20 && column + 1 < 20 && board[row + 1][column + 1].alive == true)
                count++;

	return count;
}
