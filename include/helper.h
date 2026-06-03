// helper.h
#ifndef HELPER_H // check if myheader_h is not defined
#define HELPER_H


// all your header file code (declarations, etc.) goes here
void getFirstLine(char *buffer, FILE *filepntr);
void getNextLine(int *index, char *buffer, FILE *filepntr);
void skipToEndOfComment(char *buffer, int *index, FILE *filepntr);
char *nameOutputFile(char *inputname, char *new_path);
char *process(int *tab, int *index, char *buffer, char *token, char *expected_token, 
              char *token_type, FILE *filewrtr, FILE *filepntr);
void printXMLToken(int *tab, char *token, char *token_type, FILE *filewrtr);
int typeCheck(char *token, char *token_type);
void printTabs(int *tab, FILE *filewrtr);
int opCheck(char *token);
int keywordConstantCheck(char *token);
int termCheck(char *token, char *token_type);
int fileCheck(char *inputname);
void zeroBuffers(char *token, char *buffer, char *new_path);
int getCharCode(char character);

#endif // ends the #ifndef block
