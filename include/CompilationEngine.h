// CompilationEngine.h 
#ifndef COMPILATIONENGINE_H // check if myheader_h is not defined
#define COMPILATIONENGINE_H


// all your header file code (declarations, etc.) goes here
void compileClass(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr, labelNode *labelList);
void compileClassVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
void compileSubroutine(int *tab, int *index, char *buffer, char *token, char *nameOfClass, 
                          FILE *filewrtr, FILE *filepntr, labelNode *labelList, tableNode *firstTable);
void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, char *nameOfClass, char *subroutineName, FILE *filewrtr,  
                           FILE *filepntr, labelNode *labelList, tableNode *firstTable);
void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr,  
                  FILE *filepntr, tableNode *firstTable);
void compileStatements(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList);
void compileStatement(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList);
void compileLet(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
void compileIf(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList);
void compileExpression(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
void compileWhile(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList);
void compileDo(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
char *compileTerm(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
void compileReturn(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);
int compileExpressionList(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable);

#endif // ends the #ifndef block
