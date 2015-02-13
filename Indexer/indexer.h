
#define SIZE 10000	/* constant Size of the HashMap */


typedef struct Indexer File;

/**
   Structure to hold a node for the filname the token is in and the frequency of the token	
*/
struct Indexer {

	int frequency; 		/* Frequency of token */
	char *fileName;		/* File name of the the file the token is in */
	File *nextFile;		/* Link to next file structure */
};

typedef struct Word Token;

/** 
   Structure to hold a node for the token 
*/
struct Word {

	char *word;		/* The token */
	Token *nextToken;	/* The link to the next word structure */
	Token *nextTokenSL;	/* The link to the next word structure in the sorted list */
	File *headFileList;	/* Holds the beginning of the linked list of file structure */
};

typedef struct HashMap HashMap;

/* 
  HashMap structure 
*/
struct HashMap {

	Token *token;			/* Holds the beginning node in the HashMap */
};

/* 
  Sorted List structure 
*/
typedef struct Sorted_List SortedList;

struct Sorted_List {

	Token *headList;		/* Holds the beginning node in the sorted list */
};

/**
   Declare the hasMap and sorted list as global variables
*/
extern HashMap hashMap[SIZE]; 
extern SortedList sl;

int initializeSortedList();
int initializeHashMap();
int initializeFileNode(File **file, char *fileName);
int initializeTokenNode(Token **token, char *word);
int sortFileFrequency(Token *word, File *nodeToSort);
int incrementFrequency(Token *word, char *fileName);
int findWordToIncrement(char *word, char *fileName, unsigned long index);
int addToSortedList(Token *token);
