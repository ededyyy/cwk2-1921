#include <String.h>
#include <stdio.h>
// Help get the actual length of the rows in the maze
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
// Check if there exists invalid symbol
bool Check_Symbol(char *s) {
	char arr[] = {' ', '#', 'E', 'S', '\n'};
	int i, j;
	for (i = 0; i < length_string(s); i++) {
		for (j = 0; j < sizeof(arr); j++) {
			if (s[i] == arr[j]) {
				return true;
			}
		}
	}
	return false;
}
// Check if the maze is valid
bool Check_maze_Valid(FILE* filename)
{
    char buffer[1024];
    int lineSize[1024];
    int lineIndex = 0, numS = 0, numE = 0;
    while(fgets(buffer, 1024, filename)) {
    	// Check if file only contains 'S' and 'E', spaces,'#', and '\n'
        if (!Check_Symbol(buffer)) {
			return false;
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
		return false;
	}
	// Check the height of the maze
	if (lineIndex > 100 || lineIndex < 5) {
		return false;
	}
	int i;
	for (i = 0; i < (lineIndex - 1); i++) {	
		// Check if every row has the same length, every column has the same height
		if (lineSize[i] != lineSize[i+1]) {
		    // May have a empty line at the end of the file
			if ((i+1) != (lineIndex -1)) {
		        return false;	
			}
			else {
				if (lineSize[i+1] != 0) {
					return false;
				}
			}
		}
	}
	// Check the width of maze
	if (lineSize[0] > 100 || lineSize[0] < 5) {
		return false;
	} 
	return true;
}
// Store the data of the file into 2-dimensional array
void Store_filedata(FILE* filename, char arry[200][200])
{
    // read the file data and store the details of maze into 2-dimensional array
    char buffer[1024];
    int lineindex = 0;
    while(fgets(buffer, 1024, filename)) {
    	char *pos = strchr(buffer, '\n');
		if (pos != NULL) {
			*pos = '\0';
		}
		int i;
		for (i=0; i < length_string(buffer); i++) {
			arry[lineindex][i] = buffer[i];
		}
		lineindex++;
    }
}
// The actual operation of maze
void User_Move(char arry[200][200])
{
    // Creat x and y dimensional variables for start, end and current location point
    // Read the command
    // Check the command (W/w or A/a or S/s or D/d or M/m)
    // If: M/m, show the map of the maze,print "MAP"
    // If: Q/q, quit the program and print"Quit successfully"
    // If: W/w or A/a or S/s, check whether player is going through the walls or off the edge or not
    // Change the current location if the movement is valid
    // If the current point is the end point, print"Success!"
}
