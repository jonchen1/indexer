//
//  tokenizer.h
//  
//
//  Created by Adam on 10/11/13.
//
//

#ifndef _tokenizer_h
#define _tokenizer_h

struct TokenizerT_ {
	char* copied_string;
	char* delimiters;
	char* current_position;
};

typedef struct TokenizerT_ TokenizerT;
TokenizerT *TKCreate(char *separators, char *ts);

void TKDestroy(TokenizerT *tk);
char *TKGetNextToken(TokenizerT *tk);

	

#endif
