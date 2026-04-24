// helper.h
#ifndef HELPER_H // check if myheader_h is not defined
#define HELPER_H // if not defined, define it now

// all your header file code (declarations, etc.) goes here
int checkToken(int *index, char *buffer);
int skipToEnd(char *buffer, FILE *filepntr, int index);
int getNextLine(char *buffer, FILE *filepntr);
FILE *openFileToRead(char *filename, char *buffer);

#endif // ends the #ifndef block
