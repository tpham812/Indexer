#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "indexer.c"
#include "tokenizer.c"
#include "util.c"

/**
Function to read in a file. Then get the tokens and store it in the hashmap and index it. Returns 0 if file cannot be found, else returns 1 if function sucessfully reads in the file's conetents.
*/
int readFromFile(char *path) {
	
	FILE *file;
	file = fopen(path, "r");
	if(file == NULL) return 0;
	char *fileName = path;
	while(!feof(file)) {
		/* To get the string from the first line in the file */
		char *stringLine = getStringFromFile(file);
		/* Create the tokenizer */
		TokenizerT *tk = TKCreate(stringLine);
		char *token;
		if(strcmp(stringLine, "") != 0){
			while((token = TKGetNextToken(tk)) != NULL) {
			/* Index the token in the hashamp */
				toLowerCase(token);
				findWordToIncrement(token, fileName, getIndex(token) % SIZE);	
			}
		}
		/* Free the stringLine and the tokenizer */
		free(stringLine);
		TKDestroy(tk);
	}
	fclose(file);
	return 1;
}

/**
 Function to recursively open directories and open the files to read in a string line and store the tokens in the hash map. 
*/
void recurseThroughDirectory(char *path, DIR *directory) {

	DIR *subDirectory;
	FILE *file;
	struct dirent *files;	
	
	if(directory == NULL) return;
	/* Read in all files and/or subdirectories from directory */
	while((files = readdir(directory)) != NULL) {
		if(strcasecmp(files -> d_name, ".") == 0 || strcasecmp(files -> d_name, "..") == 0)
			continue;
		/* Concatenate the path and fileName/subdirectory to get full path name */ 
		char fileName[strlen(path) + strlen(files -> d_name) + 2];
		strcpy(fileName, path);
		strcat(fileName, "/");
		strcat(fileName, files -> d_name);
		/* Recursively call recruseThroughDirectory if a subdirectory is found */
		if((subDirectory = opendir(fileName)) != NULL) {
			recurseThroughDirectory(fileName, subDirectory);
			continue;
		}
		else {
			file = fopen(fileName, "r");
			if(file != NULL) {
				while(!feof(file)) {
					/* To get the string from the first line in the file */
					char *stringLine = getStringFromFile(file);
					/* Create the tokenizer */
					TokenizerT *tk = TKCreate(stringLine);
					char *token;
					if(strcmp(stringLine, "") != 0){
						while((token = TKGetNextToken(tk)) != NULL) {
							toLowerCase(token);
							/* Index the token in the hashamp */
							findWordToIncrement(token, fileName, getIndex(token) % SIZE);	
						}
					}
					/* Free the stringLine and the tokenizer */
					free(stringLine);
					TKDestroy(tk);
				}
				fclose(file); /* Close file object */
			}
		}
	}
	closedir(directory); /* Close directory object */

}

/**
 Function to read in files from a given directory and subdirectory. Then get the tokens and store it in the hashmap and index it. Returns 0 if directory is invalid, returns -1 if directory is empty, else returns 1 if sucessful.
*/
int readFromDirectory(char *path) {
	
	struct dirent *files;
	FILE *file;
	DIR *subDirectory;	
	DIR *directory = opendir(path);
	
	if(directory == NULL) return readFromFile(path);
	rewinddir(directory);
	/* Reading in files and or subdirectories from the inputted directory */
	while((files = readdir(directory)) != NULL) {
		if(strcasecmp(files -> d_name, ".") == 0 || strcasecmp(files -> d_name, "..") == 0)
			continue;
		/* Concatenate the path and the file name/subdirectory to get the full path name */
		char fileName[strlen(path) + strlen(files -> d_name) + 2];
		strcpy(fileName, path);
		strcat(fileName, "/");
		strcat(fileName, files -> d_name);
		/* Call recruseThroughDirectory if a subdirectory exists */
		if((subDirectory = opendir(fileName)) != NULL) {
			recurseThroughDirectory(fileName, subDirectory);
			continue;
		}
		else {
			file = fopen(fileName, "r");
			if(file != NULL) {
				while(!feof(file)) {
					/* To get the string from the first line in the file */
					char *stringLine = getStringFromFile(file);
					/* Create the tokenizer */
					TokenizerT *tk = TKCreate(stringLine);
					char *token;
					if(strcmp(stringLine, "") != 0){
						while((token = TKGetNextToken(tk)) != NULL) {
							/* Index the token in the hashamp */
							toLowerCase(token);
							findWordToIncrement(token, fileName, getIndex(token) % SIZE);	
						}
					}
					/* Free the stringLine and the tokenizer */
					free(stringLine);
					TKDestroy(tk);
				}
				fclose(file); /* Close file object */			}
		}
	}
	closedir(directory); /* Close directory object */
	return 1;
}

/**
 Main function. Returns 0 if any error occurs and immediately terminates program. Returns 1 when program runs succesfully. 
*/
int main(int argc, char **argv) {

	int checkToReadFiles;

	/* Bunch of error cases */
	if(argc < 3) {printf("\nNot enough arguments were inputted. Program will now exit.\n"); return 0;}
	else if( argc > 3) {printf("\nToo many arguments were inputted. Program will now exit.\n"); return 0;}

	if(initializeSortedList() == 0) {printf("\nSorted List failed to initialize. Program will now exit.\n"); return 0;}
	if(initializeHashMap() == 0) {printf("\nHash Map failed to initialize. Program will now exit.\n"); return 0;}
	checkToReadFiles = readFromDirectory(argv[2]);
	if(checkToReadFiles == 0) {printf("\nDirectory/File was not found or is empty. Program will now exit.\n"); return 0;}	
	if(outputToFile(argv[1])== 0) {printf("\nDid not write to file. Program will now exit.\n"); return 0;}
	printf("\nProgram successfully outputted the results to the file: \"%s\"\n", argv[1]);
	freeSortedList(); /* Free nodes allocated for the indexer */
	initializeHashMap(); /* Set hashMap back to null since nothing is stored in it anymore */
	return 1;
}
