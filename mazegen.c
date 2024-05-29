/**
 * @file mazegen.c
 * @author Xu Mingyue
 * @brief Code for creating maze for COMP1921 Assignment 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object
 *
 * @param maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return maze
 */
maze initial_maze(maze maze, int height, int width) {
	maze.height = height;
	maze.width = width;
	maze.map = (char**)malloc(sizeof(char*)*height);
	int i, j;
	for (i = 0; i < height; i++) {
		maze.map[i] = (char*)malloc((width+1)*sizeof(char));
		for (j = 0;j < width; j++) {
			maze.map[i][j] = '#';
		}
	}
	return maze;
}
/**
 * @brief Set start point for maze
 *
 * @param maze
 * @return maze
 */
maze set_start(maze instance) {
	srand((unsigned)time(NULL));
	int i = rand() % (instance.height);
	int j = rand() % (instance.width);
	instance.start.x = j;
	instance.start.y = i;
	return instance;
}

/**
 * @brief Set end point for maze
 *
 * @param maze
 * @return maze
 */
maze set_end(maze instance) {
	srand((unsigned)time(NULL));
	do {
		int i = rand() % (instance.height);
	    int j = rand() % (instance.width);
	    instance.end.x = j;
	    instance.end.y = i;
	} while (instance.start.x == instance.end.x && instance.start.y == instance.end.y);
	return instance;
}
/**
 * @brief Generate a maze object
 *
 * @param maze to be initialised
 * @return maze
 */
maze generate_maze(maze instance) {
	instance = set_start(instance);
	instance = set_end(instance);
	return instance;
}

/**
 * @brief Print maze into file
 *
 * @param file pointer
 * @param maze object
 */
void print_maze(FILE* p, maze instance) {
	instance = generate_maze(instance);
	int i, j;
	for (i = 0; i < instance.height; i++)
    {
    	for (j = 0; j < instance.width; j++) {
    		if (i == instance.start.y && j == instance.start.x) {
    			fprintf(p, "%c", 'S');
			} else if (i == instance.end.y && j == instance.end.x) {
				fprintf(p, "%c", 'E');
			}
			else {
				fprintf(p, "%c", instance.map[i][j]);
			}
		}
		fprintf(p, "\n");
    }
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Argument error");
		return 1;
	}
	FILE *p = fopen(argv[1], "w");
	int height = atoi(argv[3]);
	int width = atoi(argv[2]);
	// Check if the arguments are invalid
	if (height < 5 || height > 100) {
		printf("invalid height");
		return 1;
	}
	if (width < 5 || width > 100) {
		printf("invalid width");
		return 1;
	}
	maze instance;
	instance = initial_maze(instance, height, width);
	instance = generate_maze(instance);
	print_maze(p, instance);
	fclose(p);
	printf("success");	
	return 0;
} 
