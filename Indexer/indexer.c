#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "indexer.h"
#include "compare.c"

#define SIZE 10000	/* Final size of hash map */


HashMap hashMap[SIZE]; 	/* Hash Map */
SortedList sl;		/* Sorted List */

/** 
 Initialize sorted list. Returns 1 to indicate success
*/
int initializeSortedList() {

	sl.headList = NULL;
	return 1;
}

/**
 Initialize hash map. Returns 1 to indicate success 
*/
int initializeHashMap() {

	int count = 0;
	
	while(count < SIZE) {
		hashMap[count].token = NULL;
		count++;
	}
	return 1;
}

/**
 Allocate memory for file node. Returns 1 for success, 0 if no success 
*/
int initializeFileNode(File **file, char *fileName) {

	*file = (File *)malloc(sizeof(File));
	if(file == NULL) return 0;
	(*file) -> frequency = 1;
	(*file) -> fileName = (char *)malloc(strlen(fileName) + 1);
	strcpy((*file) -> fileName, fileName);
	(*file) -> fileName[strlen(fileName)] = '\0';
	(*file) -> nextFile = NULL;
	return 1;
}

/**
 Allocate memory for token Node. Returns 1 for success, 0 if no success
*/
int initializeTokenNode(Token **token, char *word) {

	*token = (Token *)malloc(sizeof(Token));
	if(token == NULL) return 0;
	(*token) -> word = (char *)malloc(sizeof(word) + 1);
	strcpy((*token) -> word, word);
	(*token) -> word[strlen(word)] = '\0';
	(*token) -> nextToken = NULL;
	(*token) -> nextTokenSL = NULL;
	(*token) -> headFileList = NULL;
	return 1;
}

/**
 Sorts the file node according to frequency. If the node to be sorted has a higher or equal freqnecy than the current node, then insert the node before the current node. Returns 1 to indicate success, 0 if no success
*/
int sortFileFrequency(Token *word, File *nodeToSort) {

	File *curr, *prev;
	curr = word -> headFileList;
	prev = NULL;
	/* Check to see if node's frequency is higher than or equals to the curren't nodes frequency */
	if(compareInts(nodeToSort -> frequency, curr -> frequency) >= 1 || compareInts(nodeToSort -> frequency, curr -> frequency) == 0 	) {
		nodeToSort -> nextFile = curr;
		word -> headFileList = nodeToSort;
		return 1;
	}
	prev = curr;
	curr = curr -> nextFile;
	/* Traverse though the whole list until a lower frequency is found. Then insert. */
	while(curr != NULL) {
		if(compareInts(nodeToSort -> frequency, curr -> frequency) >= 1 || compareInts(nodeToSort -> frequency, curr -> frequency) 			== 0) {
			prev -> nextFile = nodeToSort;
			nodeToSort -> nextFile = curr;
			return 1;	/* Return 1 to indicate success */	
		}
		else {
			prev = curr;
			curr = curr -> nextFile;
		}
	}
	prev -> nextFile = nodeToSort;
	return 1; 	/* Return 1 to indicate success */
}

/**
 Find the specific node, according to the filename to increment frequency by 1. If no node exists then create a node and set the frequency to 1. Returns 1 if sucessful, 0 if not.
*/
int incrementFrequency(Token *word, char *fileName) {
	
	File *curr, *prev, *newFile;
	curr = word -> headFileList;
	/* Create a new node if list is empty */
	if(curr == NULL) {
		if(initializeFileNode(&newFile, fileName) == 0) return 0;
		word -> headFileList = newFile;
		return 1;
	}
	/* If head of list is the designated node, then inrement frequency */
	if(strcmp(fileName, curr -> fileName) == 0) {
		curr -> frequency++;
		return 1;
	}
	prev = curr;
	curr = curr -> nextFile;
	/* Traverse though the whole list to find the target node with the given file name and increment. Then sort the node again if 		frequency is out of order */
	while(curr != NULL) {
		if(strcmp(fileName, curr -> fileName) == 0) {
			curr -> frequency++;
			/* If previous node frequency is less than the current one, remove the current one and call sortFileFrequency to 				reinsert the node in proper position */
			if(prev -> frequency < curr -> frequency) {
				prev -> nextFile = curr -> nextFile;
				sortFileFrequency(word, curr);
			}
			return 1;
		}
		else {
			prev = curr;
			curr = curr -> nextFile;
		}
	}
	/* If target file node cannot be found,  then create a new file node */
	if(initializeFileNode(&newFile, fileName) == 0) return 0;
	sortFileFrequency(word, newFile);
	return 1; 	/* Return 1 to indicate success */
}

/**
 Find the word node to increment the frequency of a given file. If the node does not exist, then increment the frequency. Returns 1 if successful, 0 if not.
*/
int findWordToIncrement(char *word, char *fileName, unsigned long index) {

	Token *curr, *newToken;	
	curr = hashMap[index].token;
	/* If list at the given index of hash map is empty, then create a node for the token and increment the frequency of the file */
	if(curr == NULL) {
		if(initializeTokenNode(&newToken, word) == 0) return 0;
		incrementFrequency(newToken, fileName);
		hashMap[index].token = newToken;
		addToSortedList(newToken);
		return 1;	
	}
	/* Traverse through to fine the word node to increment frequency */
	while(curr != NULL) {
		if(strcmp(word, curr -> word) == 0)  {
			incrementFrequency(curr, fileName);		
			return 1;
		}
		else
			curr = curr -> nextToken;
	}
	/* If word node could not be found, then create a node and increment frequency */
	if(initializeTokenNode(&newToken, word) == 0) return 0;
	incrementFrequency(newToken, fileName);
	newToken -> nextToken = hashMap[index].token;
	hashMap[index].token = newToken;
	addToSortedList(newToken);
	return 1;		/* Return 1 to indicate success */
}
/**
Add to the sorted list in descending order according to the frequencies. Return 1 to indicate success.  
*/
int addToSortedList(Token *token) {

	Token *curr, *prev;
	curr = sl.headList;
	prev = NULL;
	/* If list is empty then the node will become the head */
	if(curr == NULL) {sl.headList = token; return 1;}
	if(strcmp(token -> word, curr -> word) < 0) {
		token -> nextTokenSL = curr;
		sl.headList = token;
		return 1;
	}
	prev = curr;
	curr = curr -> nextTokenSL;
	/* Traverse through until a targeted node is lexigraphically smaller than the current one. Then place it to the front */ 
	while(curr != NULL) {
	if(strcmp(token -> word, curr -> word) < 0) {
			prev -> nextTokenSL = token;
			token -> nextTokenSL = curr;
			return 1;
		}
		else {
			prev = curr;
			curr = curr -> nextTokenSL;
		}
	}
	prev -> nextTokenSL = token;
	return 1; 	/* Return 1 to indicate success */
}
