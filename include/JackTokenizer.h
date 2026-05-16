// JackTokenizer.h
#ifndef JACKTOKENIZER_H // check if myheader_h is not defined
#define JACKTOKENIZER_H

// all your header file code (declarations, etc.) goes here
int hasMoreTokens(int *index, FILE *filepntr, char *buffer);
char *advance(int *index, FILE* filepntr, char *buffer, char *token);
int extractKeyword(int* index, char *token, char *buffer);
int extractSymbol(int* index, char *token, char *buffer);
void extractStringConstant(int *index, char *buffer, char *token);
void extractIdentifier(int *index, char *buffer, char *token);
void extractIntegerConstant(int *index, char *buffer, char *token);
char *tokenLookAhead(int *index, FILE* filepntr, char *buffer);

#endif // ends the #ifndef block
