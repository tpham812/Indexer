#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Tokenizer Structure */
struct TokenizerT_ {
	char* copied_string;		
	char* current_position;
};

typedef struct TokenizerT_ TokenizerT;

/*
* Description: creates a new tokenizer struct from the token stream and delimiters
* Parameters:  token stream
* Modifies: nothing
* Returns: a pointer to a tokenizer struct on success, a null pointer on failure
* 
*/
TokenizerT *TKCreate(char *ts) {
	
	if(ts == NULL){
		return NULL;
	}
	
	TokenizerT* tokenizer = (TokenizerT*)malloc(sizeof(TokenizerT));
	
	if(tokenizer == NULL){
		return NULL;
	}
	
	tokenizer->copied_string = (char*)malloc(strlen(ts) + 1);
	strcpy(tokenizer -> copied_string, ts);
	tokenizer->copied_string[strlen(ts)] = '\0';
	tokenizer->current_position = tokenizer->copied_string;
	
	return tokenizer;
}

/*
 * Description: determines if a particular character is alpha numberic or not
 * Parameters: character to be compared
 * Modifies: Nothing
 * Returns: 1 if character is a delimiter, 0 if it is not
 */
char is_delimiter(char character) {
	
	if(isalnum(character) == 0) 
		return 1;
	
	return 0;
}

/*
 * Description: returns the next token from the token stream specified within the tokenizer
 * Parameters: tokenizer from which to extract token
 * Modifies: tokenizer->current_position: identifies starting point of next token; creates a new string with 
 * Returns: token extracted as a char* on success, null on failure/end of string;
 */
char *TKGetNextToken(TokenizerT *tk) {
	
	char* token = NULL;
	char* token_start = NULL;

	while(tk->current_position - tk->copied_string < strlen(tk->copied_string)) {
		if(!is_delimiter(*tk->current_position)) {
		
			token_start = tk->current_position;
			break;
		}
		tk->current_position++;
	}
	
	if(token_start == NULL) {
		return NULL;
	}
	
	while(tk->current_position - tk->copied_string < strlen(tk->copied_string)) {
		if(is_delimiter(*tk->current_position)) {
			break;
		}
		tk->current_position++;
	}	

	token = (char*)malloc(sizeof(char) * (tk->current_position - token_start + 1));
	strncpy(token, token_start, tk->current_position - token_start);
	token[(tk->current_position - token_start)] = '\0';
	return token;
}

/*
 * Description: destroys tokenizer struct and deallocates all memory
 * Parameters: tokenizer to be destroyed
 * Modifies: tokenizer struct: deallocates memory
 * Returns: nothing 
 */
void TKDestroy(TokenizerT *tk) {	
	 
	free(tk->copied_string);
	free(tk);
	return;
}
