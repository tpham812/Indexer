#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long getIndex(unsigned char *word);
void toLowerCase(char *token);
char *getStringFromFile(FILE *file);
int outputToFile(char *fileName);
int freeSortedList();
