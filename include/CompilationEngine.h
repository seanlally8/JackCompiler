// CompilationEngine.h 
#ifndef COMPILATIONENGINE_H // check if myheader_h is not defined
#define COMPILATIONENGINE_H


// all your header file code (declarations, etc.) goes here
void compileClass(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr, labelNode *labelList);
void compileClassVarDec(int *tab, int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileSubroutine(int *tab, int *index, char *buffer, char *token, char *nameOfClass, 
                          FILE *filewrtr, FILE *filepntr, labelNode *labelList);
void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, char *nameOfClass, char *subroutineName, FILE *filewrtr,  
                           FILE *filepntr, node **hashTable, labelNode *labelList);
void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr,  
                  FILE *filepntr, node **hashTable);
void compileStatements(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable, labelNode *labelList);
void compileStatement(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable, labelNode *labelList);
void compileLet(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileIf(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable, labelNode *labelList);
void compileExpression(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileWhile(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable, labelNode *labelList);
void compileDo(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable);
char *compileTerm(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable);
void compileReturn(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable);
int compileExpressionList(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, node **hashTable);

#endif // ends the #ifndef block
