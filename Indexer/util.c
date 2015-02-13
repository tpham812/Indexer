#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

/**
   Function to convert a string to an integer value to map to hash map. Returns a long int.
   Note: this is a djb2 algorithm. Credit goes to Dan Bernstein for making this hash function.  
*/
unsigned long getIndex(unsigned char *word) {

	unsigned long hashValue = 5381;
	int index;

	for(index = 0; index < strlen(word); index ++) 
		hashValue = ((hashValue << 5) + hashValue) + word[index]; 
	
	return hashValue;
}

void toLowerCase(char *token) {

	int size = strlen(token);
	int count = 0;
	while(count < size) {
		token[count] = (char)tolower(token[count]);
		count++; 
	}
}

/**
This function reads in a line from a file of an unkown string size. The string to return will reallocate if space runs out and there are  more characters to read in. Finally reallocate size to the actual length of the string line. Returns a line store in a string
*/
char *getStringFromFile(FILE *file) {

	int size = 150;
	int length = 0;
	int ch;

	char *stringLine = (char *)malloc(sizeof(char) * size);

	while(EOF != (ch = fgetc(file)) && ch != '\n') {
		stringLine[length++] = ch;
		/* Reallocate size if length if the maximum size of string */
		if(length == size) {
			stringLine = realloc(stringLine, sizeof(char) * (size += 50));
			if(stringLine == NULL) return stringLine;
		}
	}

	stringLine[length++] = '\0';
	stringLine = realloc(stringLine, sizeof(char) * length); /* Final reallocation */

	return stringLine;	/* Return string */
}

/**
Out put to a given fileName. First make sure if the exists or not. If it does exist, check with user to see if he/she wants to 	  	overwrite the file. If user says no, return 0. If user says yes, then write output to file. Returns 1 to indicated successfuly write to file. Returns -1 if the list is empty signaling there are no tokens to output.
*/
int outputToFile(char *fileName) {

	FILE *outFile;
	int answer;
	outFile = fopen(fileName, "r");
	/* Prompt user to see if he/she wants to overwrite the file */
	if(outFile != NULL) {
		printf("\nFile to write output to already exists. Do you want to write over this file?\n(Yes: 1/ No: 0)  ");
		scanf("%d", &answer);
		fclose(outFile);
		if(answer == 0) return 0; 

	}
	outFile = fopen(fileName, "w");	
	Token *token = sl.headList;
	/* Writing to file */
	while(token != NULL) {
		fprintf(outFile, "<list> %s\n", token -> word);
		File *file = token -> headFileList;
		while(file != NULL) {
			fprintf(outFile, "%s %d ", file -> fileName, file -> frequency);
			file = file -> nextFile;
		}
		fprintf(outFile, "\n</list>\n");
		token = token -> nextTokenSL;
	}
	fclose(outFile);
	return 1;
}

/** 
Free the sorted list (freeing the Token node and File node that were allocated in memory). Returns 1 to indicate successful freeing.
*/
int freeSortedList() {

	Token *token, *temp; 
	token = sl.headList;
	while(token != NULL) {
		File *file, *temp2; 
		file = token ->  headFileList;
		while(file != NULL) {
			temp2 = file -> nextFile;
			free(file -> fileName);
			free(file);
			file = temp2;
		}
		temp = token -> nextTokenSL;
		free(token -> word);
		free(token);
		token = temp;
	}
	sl.headList = NULL;
	return 1;
}
