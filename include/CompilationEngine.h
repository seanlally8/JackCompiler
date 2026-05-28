// CompilationEngine.h 
#ifndef COMPILATIONENGINE_H // check if myheader_h is not defined
#define COMPILATIONENGINE_H

#include "SymbolTable.h"

// all your header file code (declarations, etc.) goes here
void compileClass(int *tab, int *index, char *buffer, char *token, 
                  FILE *filewrtr, FILE *filepntr);
void compileClassVarDec(int *tab, int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileSubroutine(int *tab, int *index, char *buffer, char *token, char *nameOfClass, 
                          FILE *filewrtr, FILE *filepntr);
void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, 
                           FILE *filepntr, node **hashTable);
void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr,  
                  FILE *filepntr, node **hashTable);
void compileStatements(int *tab, int *index, char *buffer, char *token, 
                       FILE *filewrtr, FILE *filepntr);
void compileStatement(int *tab, int *index, char *buffer, char *token, 
                      FILE *filewrtr, FILE *filepntr);
void compileLet(int *tab, int *index, char *buffer, char *token, 
                FILE *filewrtr, FILE *filepntr);
void compileIf(int *tab, int *index, char *buffer, 
               char *token, FILE *filewrtr, FILE *filepntr);
void compileExpression(int *tab, int *index, char *buffer, char *token_type,
                       char *token, FILE *filewrtr, FILE *filepntr);
void compileWhile(int *tab, int *index, char *buffer, char *token, 
                  FILE *filewrtr, FILE *filepntr);
void compileDo(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr);
char *compileTerm(int *tab, int *index, char *buffer, char *token, char *token_type, FILE *filewrtr, FILE *filepntr);
void compileReturn(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr);
void compileExpressionList(int *tab, int *index, char *buffer, char *token, char *token_type, FILE *filewrtr, FILE *filepntr);

#endif // ends the #ifndef block
