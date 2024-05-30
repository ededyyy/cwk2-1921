/**
 * @file mazegen.c
 * @author Xu Mingyue
 * @brief Code for creating maze for COMP1921 Assignment 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define PATH 0
#define WALL 1
#define MAX_DEPTH 50 
typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    int **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

int rank = 0;

/**
 * @brief Check if the str is integer
 *
 * @param str pointer
 * @return 1 fail, 0 success
 */
int is_int(char *str) {
	if (str == NULL) return 1;
	while (*str != '\0') {
		if(isdigit(*str) == 0) {
			return 1;
		}
		str++;
	}
	return 0;
}

/**
 * @brief Set start point for maze
 *
 * @param maze
 * @return maze
 */
maze set_start(maze instance) {
	int i = rand() % (instance.height - 2) + 1;
	int j = rand() % (instance.width - 2) + 1;
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
	do {
		int i = rand() % (instance.height - 2) + 1;
	    int j = rand() % (instance.width - 2 ) + 1;
	    instance.end.x = j;
	    instance.end.y = i;
	} while (instance.start.x == instance.end.x && instance.start.y == instance.end.y);
	return instance;
}

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
	maze.map = (int**)malloc(sizeof(int*)*height);
	int i, j;
	for (i = 0; i < height; i++) {
		maze.map[i] = (int*)calloc(height, sizeof(int));
	}
	for (i = 1; i < (height - 1); i++) {
		for (j = 1;j < (width - 1); j++) {
			maze.map[i][j] = WALL;
		}
	}
	for (j = 0; j < width; j++) {
		maze.map[0][j] = PATH;
		maze.map[height - 1][j] = PATH;
	}
	for (i = 0; i < height; i++) {
		maze.map[i][0] = PATH;
		maze.map[i][width - 1] = PATH;
	}
	maze = set_start(maze);
	maze = set_end(maze);
	maze.map[maze.end.x][maze.end.y] = PATH;
	return maze;
}

/**
 * @brief Generate a maze object
 *
 * @param maze to be initialised
 * @param int x
 * @param int y
 * @return maze
 */
maze generate_maze(maze instance, int x, int y, int depth) {
	// Select directions randomly
	instance.map[x][y] = PATH;
	if (x == instance.end.x && y == instance.end.y) {
		return instance;
	}
	if (depth == 0) {
		return instance;
	}
	int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
	int i;
	for (i = 0; i < 4; i++) {
		int r = rand() % 4;
		int temp = direction[0][0];
		direction[0][0] = direction[r][0];
		direction[r][0] = temp;
		temp = direction[0][1];
		direction[0][1] = direction[r][1];
		direction[r][1] = temp;
    }
    for (i = 0; i < 4; i++) {
    	int dx = x;
    	int dy = y;
    	int range;
    	if (rank == 0) range = 1;
    	else range = rand() % rank + 1;
    	while (range > 0) {
    		dx += direction[i][0];
    		dy += direction[i][1];
    		// if there exists path already exists, break
			if (instance.map[dx][dy] == PATH) {
    			break;
			}
			// if paths meet, break
			int count = 0;
			int j, k;
			for (j = dx - 1; j < dx + 2; j++) {
				for (k = dy - 1; k < dy + 2; k++) {
					if (abs(j - dx) + abs(k - dy) == 1 && instance.map[j][k] == PATH) {
						count++;
					}
				}
			}
			if (count > 1) {
				break;
			}
			// if path out of edge, break
			if (dx < 1 || dx > (instance.height - 2) || dy < 1 || dy > (instance.width - 2)) {
				break;
			}
			--range;
			instance.map[dx][dy] = PATH;	
		}
		// recursive
		if (range <= 0) {
			generate_maze(instance, dx, dy, depth - 1);
		}
	}
	return instance;
}

/**
 * @brief Print maze into file
 *
 * @param file pointer
 * @param maze object
 */
void print_maze(FILE* p, maze instance) {
	int i, j;
	for (i = 0; i < instance.height; i++)
    {
    	for (j = 0; j < instance.width; j++) {
    		if (i == instance.start.y && j == instance.start.x) {
    			fprintf(p, "%c", 'S');
			} else if (i == instance.end.y && j == instance.end.x) {
				fprintf(p, "%c", 'E');
			}
			else if (instance.map[i][j] == PATH) {
				fprintf(p, "%c", ' ');
			}
			else {
				fprintf(p, "%c", '#');
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
	if (is_int(argv[2]) == 1 || is_int(argv[3]) == 1) {
		printf("Argument error");
		return 1;
	}
	FILE *p = fopen(argv[1], "w");
	srand((unsigned)time(NULL));
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
	instance = generate_maze(instance, instance.start.x, instance.start.y, MAX_DEPTH);
	print_maze(p, instance);
	fclose(p);
	printf("success");
	return 0;
} 
