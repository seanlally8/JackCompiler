// CompilationEngine.h 
#ifndef COMPILATIONENGINE_H // check if myheader_h is not defined
#define COMPILATIONENGINE_H

// all your header file code (declarations, etc.) goes here
void compileClass(int *index, char *buffer, char *token, 
                  FILE *filewrtr, FILE *filepntr);
void compileClassVarDec(int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr);
void compileSubroutine(int *index, char *buffer, char *token, 
                          FILE *filewrtr, FILE *filepntr);
void compileParameterList(int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr);
void compileSubroutineBody(int *index, char *buffer, char *token, 
                           FILE *filewrtr, FILE *filepntr);

#endif // ends the #ifndef block
