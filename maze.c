#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "maze.h"
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Argument error");
		return 1;
	}
	else
	{
		char maze[200][200];
		// Check the validity of the maze
		FILE *p = fopen(argv[1], "r");
		if (p == NULL)
		{
			printf("File error");
			return 2;
		}
		else
		{
			if (!Check_maze_Valid(p))
			{
				printf("Invalid maze");
				return 3;
			}
			// Store data into 2-dimensional array
			Store_filedata(p, maze);
			User_Move(maze);
			fclose(p);
		}
	}
	return 0;
}
