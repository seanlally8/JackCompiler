// CompilationEngine.h 
#ifndef COMPILATIONENGINE_H // check if myheader_h is not defined
#define COMPILATIONENGINE_H

// all your header file code (declarations, etc.) goes here
void compileClass(int *tab, int *index, char *buffer, char *token, 
                  FILE *filewrtr, FILE *filepntr);
void compileClassVarDec(int *tab, int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr);
void compileSubroutine(int *tab, int *index, char *buffer, char *token, 
                          FILE *filewrtr, FILE *filepntr);
void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr);
void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, 
                           FILE *filewrtr, FILE *filepntr);
void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, 
              FILE *filpntr);
void compileStatements(int *tab, int *index, char *buffer, char *token, 
                       FILE *filewrtr, FILE *filepntr);
void compileStatement(int *tab, int *index, char *buffer, char *token, 
                      FILE *filewrtr, FILE *filepntr);
void compileLet(int *tab, int *index, char *buffer, char *token, 
                FILE *filewrtr, FILE *filepntr);
void compileIf(int *tab, int *index, char *buffer, 
               char *token, FILE *filewrtr, FILE *filepntr);
void compileExpression(int *tab, int *index, char *buffer, 
                       char *token, FILE *filewrtr, FILE *filepntr);
void compileWhile(int *tab, int *index, char *buffer, char *token, 
                  FILE *filewrtr, FILE *filepntr);
void compileDo(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr);
void compileTerm(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr);
void compileReturn(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr);
#endif // ends the #ifndef block
