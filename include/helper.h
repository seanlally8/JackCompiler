// helper.h
#ifndef HELPER_H // check if myheader_h is not defined
#define HELPER_H


// all your header file code (declarations, etc.) goes here
void getNextLine(int *index, char *buffer, FILE *filepntr);
void skipToEndOfComment(char *buffer, int *index, FILE *filepntr);
int checkIfKeywordOrSymbol(int* index, char *token, char *buffer);


#endif // ends the #ifndef block
