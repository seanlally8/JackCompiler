
// JackTonizer.h
#ifndef JACKTOKENIZER_H // check if myheader_h is not defined
#define JACKTOKENIZER_H

#include <stdio.h>

// all your header file code (declarations, etc.) goes here
void advance(int *index, FILE* filepntr, char *buffer, char *token);
char *tokenType(char *token);
char *keyword(char *token);
char symbol(char *token);
char *identifier(char *token);
int intVal(char *token);
char *stringVal(char *token);


#endif // ends the #ifndef block
