#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
 * @param start start point to allocate
 * @param end end point to allocate
 * @return maze
 */

maze create_maze(maze maze, int height, int width, coord start, coord end)
{
	maze.height = height;
	maze.width = width;
	maze.start = start;
	maze.end = end;
	return maze;
}

/**
 * @brief help get the actual length of the string
 *
 * @param char *s
 * @return int length of string
 */ 
int length_string(char *s)
{
    int i = 0;
	while (*s != '\0')
	{
		s++;
		i++;
	}
	return i;
}

/**
 * @brief check if there exists invalid symbol
 *
 * @param char *s
 * @return int 0 on success, 1 on fail
 */ 
int Check_Symbol(char *s) {
	char arr[] = {' ', '#', 'E', 'S', '\n'};
	int i, j;
	for (i = 0; i < length_string(s); i++) {
		for (j = 0; j < (int)sizeof(arr); j++) {
			if (s[i] == arr[j]) {
				return 0;
			}
		}
	}
	return 1;
}

/**
 * @brief check if the maze is valid
 *
 * @param file pointer
 * @return int 0 on success, 1 on fail
 */
int Check_maze_Valid(FILE* filename)
{
    char buffer[1024];
    int lineSize[1024];
    int lineIndex = 0, numS = 0, numE = 0;
    while(fgets(buffer, 1024, filename)) {
    	// Check if file only contains 'S' and 'E', spaces,'#', and '\n'
        if (Check_Symbol(buffer) == 1) {
			return 1;
		} 
		char *pos = strchr(buffer, '\n');
		if (pos != NULL) {
			*pos = '\0';
		}
		lineSize[lineIndex] = length_string(buffer);
		lineIndex++;
    	char* searchS = strchr(buffer, 'S');
    	if (searchS != NULL) {
    		numS++;
		}
		char* searchE = strchr(buffer, 'E');
		if (searchE != NULL) {
			numE++;
		}
	}
	// Check if there has single point 'S' and 'E'
	if (numS != 1 || numE != 1) {
		return 1;
	}
	// Check the height of the maze
	if (lineIndex > 100 || lineIndex < 5) {
		return 1;
	}
	int i;
	for (i = 0; i < (lineIndex - 1); i++) {	
		// Check if every row has the same length, every column has the same height
		if (lineSize[i] != lineSize[i+1]) {
		    // May have a empty line at the end of the file
			if ((i+1) != (lineIndex -1)) {
		        return 1;	
			}
			else {
				if (lineSize[i+1] != 0) {
					return 1;
				}
			}
		}
	}
	// Check the width of maze
	if (lineSize[0] > 100 || lineSize[0] < 5) {
		return 1;
	} 
	return 0;
}

/**
 * @brief return the width of the mazefile
 *
 * @param file the file pointer
 * @return int valid width (5-100)
 */
int get_width(FILE *file)
{
	rewind(file);
	char buffer[1024];
	fgets(buffer, 1024, file);
	char *pos = strchr(buffer, '\n');
		if (pos != NULL) {
			*pos = '\0';
		}
	return length_string(buffer);
}

/**
 * @brief return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int valid height (5-100)
 */
int get_height(FILE *file)
{
	rewind(file);
	char buffer[1024];
	int height = 0;
	while(fgets(buffer, 1024, file)) {
		if((length_string(buffer) - 1) != 0) {
			height++;
		}
	} 
	return height;
}

/**
 * @brief return the end point
 *
 * @param file the file pointer 
 * @return coord end point
 */
coord get_end(FILE *file) {
	coord end;
	rewind(file);
	char buffer[1024];
    int lineindex = 0;
    while(fgets(buffer, 1024, file)) {
    	char *pos = strchr(buffer, '\n');
		if (pos != NULL) {
			*pos = '\0';
		}
        char *ptrE;
        ptrE = strchr(buffer, 'E');
		if (ptrE != NULL) {
			end.x = ptrE - buffer;
			end.y = lineindex;
		}
		lineindex++;
    }
	return end;
}

/**
 * @brief return the start point
 *
 * @param file the file pointer 
 * @return coord start point
 */
coord get_start(FILE *file) {
	coord start;
	rewind(file);
	char buffer[1024];
    int lineindex = 0;
    while(fgets(buffer, 1024, file)) {
    	char *pos = strchr(buffer, '\n');
		if (pos != NULL) {
			*pos = '\0';
		}
        char *ptrS;
        ptrS = strchr(buffer, 'S');
		if (ptrS != NULL) {
			start.x = ptrS - buffer;
			start.y = lineindex;
		}
		lineindex++;
    }
	return start;
}

/**
 * @brief read the map into struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return maze
 */
maze read_map(maze maze, FILE *file)
{
	rewind(file);
	char buffer[1024];
    int lineindex = 0;
    while(fgets(buffer, 1024, file)) {
    	if((length_string(buffer) - 1) != 0) {
		char *pos = strchr(buffer, '\n');
		if (pos != NULL) {
			*pos = '\0';
		}
		maze.map[lineindex] = malloc(strlen(buffer) + 1);
		strcpy(maze.map[lineindex], buffer);
		lineindex++;
		}
    }
    return maze;
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    int i, j;
    for (i = 0; i < this->height; i++)
    {
        for (j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze maze, coord player)
{
	if ((player.x == maze.end.x) && (player.y == maze.end.y)) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * @brief the actual operation of maze
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void User_Move(maze maze, coord player) {
	while(1) {
    	char ch = getchar();
		if (ch == 'W' || ch == 'w') {
    		if(maze.map[player.y - 1][player.x] != '#' && player.y != 0 ){
    			player.y --;
    			if (has_won(maze,player) == 1) {
			        printf("Success!");
			        break;
		        }
			}
			else {
				printf("invalid step\n");
			}
		}
		if (ch == 'S' || ch == 's') {
    		if(maze.map[player.y + 1][player.x] == ' ' || maze.map[player.y + 1][player.x] == 'S' || maze.map[player.y + 1][player.x] == 'E' ){
    			player.y ++;
    			if (has_won(maze,player) == 1) {
			        printf("Success!");
			        break;
		        }
			}
			else {
				printf("invalid step\n");
			}
		}
		if (ch == 'A' || ch == 'a') {
    		if(maze.map[(player.y)][player.x - 1] != '#' && player.x != 0 ){
    			player.x --;
    			if (has_won(maze,player) == 1) {
			        printf("Success!");
			        break;
		        }
			}
			else {
				printf("invalid step\n");
			}
		}
		if (ch == 'D' || ch == 'd') {
    		if(maze.map[player.y][player.x + 1] != '#' && player.x != (maze.width - 1) ){
    			player.x ++;
    			if (has_won(maze,player) == 1) {
			        printf("Success!");
			        break;
		        }
			}
			else {
				printf("invalid step\n");
			}
		}
		if (ch == 'M' || ch == 'm') {
			print_maze(&maze, &player);
		}
		if (ch == 'Q' || ch == 'q') {
			printf("quit successfully");
			break;
		}
    }
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Argument error");
		return 1;
	}
	else
	{
		// Check the validity of the maze
		FILE *p = fopen(argv[1], "r");
		if (p == NULL)
		{
			printf("File error");
			return 2;
		}
		else
		{
			if (Check_maze_Valid(p) != 0)
			{
				printf("Invalid maze");
				return 3;
			}
			// Store data into 2-dimensional array
			maze instance;
			instance = create_maze(instance, get_height(p), get_width(p), get_start(p), get_end(p));
            instance = read_map(instance, p);
			coord player;
			player.x = get_start(p).x;
			player.y = get_start(p).y;
			User_Move(instance, player);
			fclose(p);
		}
	}
	return 0;
}
