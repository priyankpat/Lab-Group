#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define INPUT_PATH "/home/sonuthemaster/workspace/Lab3/src/inputs/puzzle.txt"

/*
 * structure for passing data to threads
 */
typedef struct
{
	int row;
	int column;
	int (* puzzle)[9];
} PARAMETERS;

void readInputFile(int puzzle[][9]);
void *checkRow(void *arg);

/**
 * Main constructor
 * @param  argc Number of arguments
 * @param  argv List of arguments
 * @return      0
 */
int main (int argc, char **argv)
{
	// Initialize 9x9 array grid
	int puzzle[9][9];

	// Read the input data
	readInputFile(puzzle);

	// Allocate PARAMETERS structure
	PARAMETERS *param = (PARAMETERS *) malloc(sizeof(PARAMETERS));
	param->row = 0;
	param->column = 0;
	param->puzzle = puzzle;

	pthread_t prow;
	pthread_t pcol;

	void *valid_rows;

	pthread_create(&prow, NULL, checkRow, (void *) param);

	pthread_join(prow, &valid_rows);

	if((int) valid_rows == 1)
		printf("All Rows Valid!");
	else
		printf("Not Solved");

	// Return 0
	return 0;
}

/**
 * Read data from file
 * @param  void none
 * @return      none
 */
void readInputFile(int puzzle[][9])
{
	// Initialize FILE
	FILE *fp;

	// Open the input file
	fp = fopen(INPUT_PATH, "r");

	// Check if the file is found
	if(fp == NULL)
	{
		// Throw error if the file was not found
		perror("Error while reading the file");
		exit(EXIT_FAILURE);
	}

	// Nested loop to loop through the 2D puzzle array
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			// Scan each int and add it to puzzle 2D using puzzle[row][column]
			fscanf(fp, "%1d", &puzzle[i][j]);
		}
	}

	// Close the file after reading
	fclose(fp);
}

/**
 * Check to check that each row contains the digits 1 through 9
 * @param  void arg
 * @return      1
 */
void *checkRow(void *arg)
{
	// Cast the argument back to structure
	PARAMETERS *data = (PARAMETERS *) arg;

	// Get the starting position of row
	int startRow = data->row;
	// Get the starting position of column
	int startCol = data->column;

	// Nested for loop to check reach row
	for(int i = startRow; i < 9; ++i)
	{
		// Initialize array size of 10 with 0 as default
		int row[10] = {0};
		fprintf(stderr, "\n");

		for(int j = startCol; j < 9; ++j)
		{
			// Get the value of row and column position on every loop
			int val = data->puzzle[i][j];

			fprintf(stderr, "val: %d", val);

			// check if the value is not 0
			if(row[val] != 0)
				return (void *) 0;
			else
				row[val] = 1;
		}
	}

	return (void *) 1;
}
 
