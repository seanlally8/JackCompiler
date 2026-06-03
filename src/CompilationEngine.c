#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"
#include "CompilationEngine.h"
#include "helper.h"

void compileClass(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr, labelNode *labelList) {

  // I've opted to include a local variable 
  // (token_type) for each compiler function to reduce 
  // the bloated parameters list
  char *token_type = NULL;
  tableNode *firstTable = NULL;

  firstTable = initTable(firstTable); 

  firstTable->tableType = calloc(6, sizeof(char));
  strcpy(firstTable->tableType, "class");

  // 'class'  
  token_type = process(tab, index, buffer, token, "class", "keyword", filewrtr, filepntr);
  
  char *nameOfClass = calloc(1, strlen(token) + 1);

  // className 
  if (strcmp(token_type, "identifier") == 0) {
    strcpy(nameOfClass, token);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // '{'
  token_type = process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // classVarDec*
  while (strcmp(token, "static") == 0 || strcmp(token, "field") == 0) {
    compileClassVarDec(tab, index, buffer, token, filewrtr, filepntr, firstTable);
  }

  printf("CLASS TABLE\n");
  printTable(firstTable);
  printf("----------------\n");

  // subroutineDec*
  while (strcmp(token, "constructor") == 0 || strcmp(token, "function") == 0
          || strcmp(token, "method") == 0) {
    compileSubroutine(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, labelList, firstTable);
  }

  // '}'
  token_type = process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  reset(firstTable);
  free(firstTable);
  free(nameOfClass);
} 

void compileClassVarDec(int *tab, int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {
  char *token_type = NULL;
  char *tableItems[3] = {0};

  // ('static'|'field') 
  tableItems[0] = calloc(1, (strlen(token) + 1));
  strcpy(tableItems[0], token);
  token_type = process(tab, index, buffer, token, token, "keyword", filewrtr, filepntr);
  
  // ('int' | 'boolean' | 'char' | className)
  if (strcmp(token, "int") == 0 || strcmp(token, "boolean") == 0 
      || strcmp(token, "char") == 0 || strcmp(token_type, "identifier") == 0) {
    tableItems[1] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[1], token);
  } 
  else {
    printf("Syntax Error in compileClassVarDec\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Third terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    tableItems[2] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[2], token);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  define(tableItems[2], tableItems[1], tableItems[0], firstTable);
  free(tableItems[2]);

  // Get next token
  advance(index, filepntr, buffer, token);

  // Implements the optional variable list ((',' varName)*)
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (strcmp(token_type, "identifier") == 0) {
      tableItems[2] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[2], token);
      define(tableItems[2], tableItems[1], tableItems[0], firstTable);
      free(tableItems[2]);
    }
    else {
      printf("Syntax Error in compileClassVarDec: No varName");
    }
    advance(index, filepntr, buffer, token);
  }

  // Final terminal (";")
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  for (int i = 0; i < 2; i++) {
    free(tableItems[i]);
  }
}

void compileSubroutine(int *tab, int *index, char *buffer, char *token, char *nameOfClass, 
                          FILE *filewrtr, FILE *filepntr, labelNode *labelList, tableNode *firstTable) {
  char *token_type = NULL;

  firstTable = initTable(firstTable);

  // ('method' |'function' | 'constructor')
  firstTable->nextTable->tableType = calloc(1, strlen(token) + 1);
  strcpy(firstTable->nextTable->tableType, token);
  if (strcmp(token, "method") == 0) {
    define("this", nameOfClass, "argument", firstTable);
  }

  token_type = process(tab, index, buffer, token, token, "keyword", filewrtr, filepntr);

  // ('void' | type)
  if (strcmp(token, "void") == 0 || strcmp(token, "int") == 0 
      || strcmp(token, "boolean") == 0 || strcmp(token, "char") == 0
      || strcmp(token_type, "identifier") == 0) {
  }
  else {
    printf("Syntax Erroer in compileSubroutine: no type given\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // subroutineName
  char *subroutineName = calloc(1, strlen(token) + 1);
  if (strcmp(token_type, "identifier") == 0) { 
    strcpy(subroutineName, token);
  }
  else {
    printf("Syntax Error in compileSubroutine: no subroutine name given");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // '('
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // parameterList
  compileParameterList(tab, index, buffer, token, token_type, filewrtr, filepntr, firstTable);

  // ')'
  token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // subroutineBody
  compileSubroutineBody(tab, index, buffer, token, nameOfClass, subroutineName, filewrtr, filepntr, labelList, firstTable);

  // Print subroutine symbol table to stdout
  printf("SUBROUTINE TABLE, %s\n", subroutineName);
  printTable(firstTable);
  printf("-------------\n");

  // free nodes of hashtable with reset then free remaining heap memory
  reset(firstTable);
  free(firstTable->nextTable);
  firstTable->nextTable = NULL;
  free(subroutineName);
}

void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {

  char *tableItems[2] = {0};

  // type 
  if (typeCheck(token, token_type)) {
    tableItems[0] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[0], token);
    token_type = advance(index, filepntr, buffer, token);
  }
  // if not a type, assume empty parameter list
  else {
    return;
  }

  // Second terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    tableItems[1] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[1], token);
    token_type = advance(index, filepntr, buffer, token);
  }
  else {
    printf("Syntax Error in compileParameterList: no varName\n");
  }

  // Add first parameter to table
  define(tableItems[1], tableItems[0], "argument", firstTable);

  // Free the tableItems array so it can be resized for any following variables
  for (int i = 0; i < 2; i++) {
    free(tableItems[i]);
  }


  // Beginning of (',' type varName)*
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (typeCheck(token, token_type)) {
      tableItems[0] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[0], token);
      token_type = advance(index, filepntr, buffer, token);
    }
    else {
      printf("Syntax Error in compileParameterList: No type in one of the parameter\n");
    }
    if (strcmp(token_type, "identifier") == 0) {
      tableItems[1] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[1], token);
      token_type = advance(index, filepntr, buffer, token);
    }
    else {
      printf("Syntax error in compileParamterList: no varName in one of the parameters\n");
    }
    define(tableItems[1], tableItems[0], "argument", firstTable);
    for (int i = 0; i < 2; i++) {
      free(tableItems[i]);
    }
  }
}

void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, char *nameOfClass, char *subroutineName, FILE *filewrtr,  
                           FILE *filepntr, labelNode *labelList, tableNode *firstTable) {

  int varcounter = 0;

  // '{'
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // varDec*
  while (strcmp(token, "var") == 0) {
    compileVarDec(tab, index, buffer, token, filewrtr, filepntr, firstTable);
  }

  // Code generator
  varcounter = varCount("local", firstTable);
  int zero = 0;
  writeFunction(filewrtr, nameOfClass, subroutineName, &varcounter);
  if (strcmp(firstTable->nextTable->tableType, "constructor") == 0) {
    varcounter = varCount("field", firstTable);
    writePush(filewrtr, "constant", &varcounter);
    int nArg = 1;
    writeCall(filewrtr, "Memory", "alloc", &nArg);
    writePop(filewrtr, "pointer", &zero);
  }
  else if (strcmp(firstTable->nextTable->tableType, "method") == 0) {
    writePush(filewrtr, "argument", &zero);
    writePop(filewrtr, "pointer", &zero);
  }

  // Implementation of statements rule
  compileStatements(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);

  // Second terminal ('}')
  process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

}

void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {
  char *token_type = NULL;
  char *tableItems[2] = {0};

  // var
  token_type = process(tab, index, buffer, token, "var", "keyword", filewrtr, filepntr);

  // type
  if (typeCheck(token, token_type)) {
    tableItems[0] = calloc(1, strlen(token) + 1);
    strcpy(tableItems[0], token);
  }
  else {
    printf("Syntax error in compileVarDec: no data type\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // varName
  if (strcmp(token_type, "identifier") == 0) {
    tableItems[1] = calloc(1, strlen(token) + 1);
    strcpy(tableItems[1], token);
  }
  else {
    printf("Syntax error in compileVarDec: no varName given\n");
  }

  define(tableItems[1], tableItems[0], "local", firstTable);
  free(tableItems[1]);

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // (',' varName)*
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);

    if (strcmp(token_type, "identifier") == 0) {
      tableItems[1] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[1], token);
      define(tableItems[1], tableItems[0], "local", firstTable);
      free(tableItems[1]);
    }
    else {
      printf("Syntax error in compileVarDec: missing varName following ','\n");
    }

    token_type = advance(index, filepntr, buffer, token);
  }

  free(tableItems[0]);

  // ';'
  token_type = process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);
}

void compileStatements(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList) {

  // statement*
  while (strcmp(token, "let") == 0 || strcmp(token, "while") == 0 || strcmp(token, "if") == 0 
      || strcmp(token, "do") == 0 || strcmp(token, "return") == 0) {
    compileStatement(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);
  }
}

void compileStatement(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList) {

  if (strcmp(token, "let") == 0) {
    compileLet(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable);
  }
  else if (strcmp(token, "if") == 0) {
    compileIf(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);
  }
  else if (strcmp(token, "while") == 0) {
    compileWhile(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);
  }
  else if (strcmp(token, "do") == 0) {
    compileDo(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable);
  }
  else if (strcmp(token, "return") == 0) {
    compileReturn(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable);
  }
  
}

void compileLet(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {
  char *token_type = NULL;

  // 'let'
  token_type = process(tab, index, buffer, token, "let", "keyword", filewrtr, filepntr);

  // varName
  char *vmSegment = kindOf(token, firstTable);
  int vmIndex;
  if (strcmp(token_type, "identifier") == 0) {
    vmIndex = indexOf(token, firstTable);
  }
  else {
    printf("Syntax error in compileVarDec: missing varName following ','\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // ('[' expression ']')?
  if (strcmp(token, "[") == 0) {
    token_type = process(tab, index, buffer, token, "[", "symbol", filewrtr, filepntr);
    writePush(filewrtr, vmSegment, &vmIndex);
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    writeArithmetic(filewrtr, "+ ");
    int one = 1;
    writePop(filewrtr, "temp", &one);

    token_type = process(tab, index, buffer, token, "]", "symbol", filewrtr, filepntr);

    // '='
    token_type = process(tab, index, buffer, token, "=", "symbol", filewrtr, filepntr);

    // expression
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    writePush(filewrtr, "temp",  &one);
    writePop(filewrtr, "pointer", &one);
    int zero = 0;
    writePop(filewrtr, "that", &zero);
  }
  else {
    // '='
    token_type = process(tab, index, buffer, token, "=", "symbol", filewrtr, filepntr);

    // expression
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    writePop(filewrtr, vmSegment, &vmIndex);
  }

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

}

void compileIf(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList) {
  char *token_type = NULL;

  // prepend new incremented node to keep track of current label number for duration of program
  labelNode *n = calloc(1, sizeof(labelNode));
  n->label1 = labelList->label1;
  n->label2 = labelList->label2;
  n->next = labelList->next;
  labelList->next = n;
  labelList->label1 += 2;
  labelList->label2 += 2;

  // 'if'
  process(tab, index, buffer, token, "if", "keyword", filewrtr, filepntr);

  // '('
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // expression
  compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
  writeArithmetic(filewrtr, "~");
  writeIf(filewrtr, labelList->next->label1);

  // ')'
  process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // '{'
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // statements
  compileStatements(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);
  writeGoTo(filewrtr, labelList->next->label2);
  writeLabel(filewrtr, labelList->next->label1);

  // '}'
  process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  // ('else' '{' statements '}')?
  if (strcmp(token, "else") == 0) {
    process(tab, index, buffer, token, "else", "keyword", filewrtr, filepntr);
    process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);
    compileStatements(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);
    process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);
  }
  writeLabel(filewrtr, labelList->next->label2);
  // delete the 2nd node in list -- we maintain the first to keep track of current label numbers
  if (labelList->next != NULL) {
    labelNode *ptr = labelList->next;
    labelList->next = ptr->next;
    free(ptr);
  }
}

void compileWhile(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable, labelNode *labelList) {
  char *token_type = NULL;

  // prepend new incremented node to keep track of current label number for duration of program
  labelNode *n = calloc(1, sizeof(labelNode));
  n->label1 = labelList->label1;
  n->label2 = labelList->label2;
  n->next = labelList->next;
  labelList->next = n;
  labelList->label1 += 2;
  labelList->label2 += 2;

  // 'while'
  process(tab, index, buffer, token, "while", "keyword", filewrtr, filepntr);

  writeLabel(filewrtr, labelList->next->label1);

  // '('
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
  
  // expression
  compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);

  writeArithmetic(filewrtr, "~");
  writeIf(filewrtr, labelList->next->label2);

  // ')'
  process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // '{'
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // statements
  compileStatements(tab, index, buffer, token, nameOfClass, filewrtr, filepntr, firstTable, labelList);
  
  writeGoTo(filewrtr, labelList->next->label1);

  // '}'
  process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  writeLabel(filewrtr, labelList->next->label2);

  // delete the 2nd node in list -- we maintain the first to keep track of current label numbers
  if (labelList->next != NULL) {
    labelNode *ptr = labelList->next;
    labelList->next = ptr->next;
    free(ptr);
  }
}

void compileDo(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {
  char *token_type = NULL;

  // 'do'
  token_type = process(tab, index, buffer, token, "do", "keyword", filewrtr, filepntr);

  char *next_token = tokenLookAhead(index, filepntr, buffer);

  // subroutineCall
  char *vmSegment = kindOf(token, firstTable);
  char *subroutineName = calloc(1, strlen(token) + 1);
  strcpy(subroutineName, token);
  // if it is method of the implicit kind: this.methodName(args)
  if (strcmp(token_type, "identifier") == 0 && next_token[0] == '(') {
    // Push mapping of 'this'
    int zero = 0;
    writePush(filewrtr, "pointer", &zero);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

    int argcounter = compileExpressionList(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    argcounter++;
    process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 

    writeCall(filewrtr, nameOfClass, subroutineName, &argcounter);
    writePop(filewrtr, "temp", &zero);

    free(subroutineName);
  }
  // If it is a method
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '.' && strcmp(vmSegment, "NONE") != 0) {
    // push varName index
    int vmIndex = indexOf(token, firstTable);
    writePush(filewrtr, vmSegment, &vmIndex);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, ".", "symbol", filewrtr, filepntr); 

    char *subroutineName2 = calloc(1, strlen(token) + 1);
    strcpy(subroutineName2, token);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr); 

    int argcounter = compileExpressionList(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    argcounter++;
    process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 

    // the type column in symbol table will be the class name of that object 
    // (thus if we declare "var Square square;" then "square.run()" is "Square.run()")
    char *type = typeOf(subroutineName, firstTable);
    writeCall(filewrtr, type, subroutineName2, &argcounter);
    int zero = 0;
    writePop(filewrtr, "temp", &zero);

    free(subroutineName);
    free(subroutineName2);
  }
  // If it is not a method
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '.' && strcmp(vmSegment, "NONE") == 0) {
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, ".", "symbol", filewrtr, filepntr); 

    char *subroutineName2 = calloc(1, strlen(token) + 1);
    strcpy(subroutineName2, token);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr); 

    int argcounter = compileExpressionList(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 

    writeCall(filewrtr, subroutineName, subroutineName2, &argcounter);
    int zero = 0;
    writePop(filewrtr, "temp", &zero);

    free(subroutineName);
    free(subroutineName2);
  }

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  free(next_token);
}

void compileReturn(int *tab, int *index, char *buffer, char *token, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {
  char *token_type = NULL;

  // 'return'
  token_type = process(tab, index, buffer, token, "return", "keyword", filewrtr, filepntr);

  // expression?
  if (termCheck(token, token_type)) {
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
  }
  else {
    int vmIndex = 0;
    writePush(filewrtr, "constant", &vmIndex);
  }
  writeReturn(filewrtr);

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);
}

void compileExpression(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {

  // term
  token_type = compileTerm(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);

  // (op term)*
  while (opCheck(token)) {
    char *operator = calloc(1, strlen(token) + 2);
    strcpy(operator, token);
    strcat(operator, " ");
    token_type = advance(index, filepntr, buffer, token);
    compileTerm(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    writeArithmetic(filewrtr, operator);
    free(operator);
  }
}

char *compileTerm(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {

  char *next_token = tokenLookAhead(index, filepntr, buffer);
  char *vmSegment = kindOf(token, firstTable);
  char *subroutineName = calloc(1, strlen(token) + 1);
  strcpy(subroutineName, token);
  
  // integerConstant | stringConstant | keywordConstant 
  // | varName | varName '[' expression ']' | '(' expression ')'
  // | (unaryOp term) | subroutineCall

  // integerConstant
  if (strcmp(token_type, "integerConstant") == 0) { 
    int integerConstant = (int)strtol(token, NULL, 10);
    writePush(filewrtr, "constant", &integerConstant);
    token_type = advance(index, filepntr, buffer, token);
  }
  // stringConstant
  else if (strcmp(token_type, "stringConstant") == 0) {
    int stringLength = (int)strlen(token);
    writePush(filewrtr, "constant", &stringLength);
    int one = 1;
    writeCall(filewrtr, "String", "new", &one);
    for (int i = 0; i < (int)strlen(token); i++) {
      int charCode = getCharCode(token[i]); 
      writePush(filewrtr, "constant", &charCode);
      int two = 2;
      writeCall(filewrtr, "String", "appendChar", &two);
    }
    token_type = advance(index, filepntr, buffer, token);
  }
  // true | false | null | this
  else if (keywordConstantCheck(token)) {
    if (strcmp(token, "null") == 0) {
      int zero = 0;
      writePush(filewrtr, "constant", &zero);
    }
    else if (strcmp(token, "false") == 0) {
      int zero = 0;
      writePush(filewrtr, "constant", &zero);
    }
    else if (strcmp(token, "true") == 0) {
      int one = 1;
      writePush(filewrtr, "constant", &one);
      writeArithmetic(filewrtr, "-");
    }
    else if(strcmp(token, "this") == 0) {
      int zero = 0;
      writePush(filewrtr, "pointer", &zero);
    }
    token_type = advance(index, filepntr, buffer, token);
  }
  // varName '[' Expression ]'
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '[') {
    // push symbol mapping of varName
    char *vmSegment = kindOf(token, firstTable);
    int vmIndex = indexOf(token, firstTable);
    writePush(filewrtr, vmSegment, &vmIndex);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "[", "symbol", filewrtr, filepntr);
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    token_type = process(tab, index, buffer, token, "]", "symbol", filewrtr, filepntr);

    // after the expression writes the appropriate push commands, add the top two stack items and pop to 
    // pointer 1 (this is the address of the identified index)
    writeArithmetic(filewrtr, "+ ");
    int one = 1;
    int zero = 0;
    writePop(filewrtr, "pointer",  &one);
    writePush(filewrtr, "that", &zero);
  }
  // '(' Expression ')'
  else if (strcmp(token, "(") == 0) {
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);
  }
  // Unary op
  else if (strcmp(token, "-") == 0 || strcmp(token, "~") == 0) {
    char *operator = calloc(1, strlen(token) + 1);
    strcpy(operator, token);
    token_type = advance(index, filepntr, buffer, token);
    token_type = compileTerm(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    writeArithmetic(filewrtr, operator);
    free(operator);
  }
  // subroutineCall
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '(') {
    // Push mapping of 'this'
    int zero = 0;
    writePush(filewrtr, "pointer", &zero);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

    int argcounter = compileExpressionList(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    argcounter++;
    process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 

    writeCall(filewrtr, nameOfClass, subroutineName, &argcounter);

    free(subroutineName);
  }
  // If it is a method
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '.' && strcmp(vmSegment, "NONE") != 0) {
    // push varName index
    int vmIndex = indexOf(token, firstTable);
    writePush(filewrtr, vmSegment, &vmIndex);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, ".", "symbol", filewrtr, filepntr); 

    char *subroutineName2 = calloc(1, strlen(token) + 1);
    strcpy(subroutineName2, token);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr); 

    int argcounter = compileExpressionList(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    argcounter++;
    process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 

    // the type column in symbol table will be the class name of that object 
    // (thus if we declare "var Square square;" then "square.run()" is "Square.run()")
    char *type = typeOf(subroutineName, firstTable);
    writeCall(filewrtr, type, subroutineName2, &argcounter);

    free(subroutineName2);
  }
  // If it is not a method
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '.' && strcmp(vmSegment, "NONE") == 0) {
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, ".", "symbol", filewrtr, filepntr); 

    char *subroutineName2 = calloc(1, strlen(token) + 1);
    strcpy(subroutineName2, token);

    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr); 

    int argcounter = compileExpressionList(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 

    writeCall(filewrtr, subroutineName, subroutineName2, &argcounter);

    free(subroutineName2);
  }
  // varName
  else if (strcmp(token_type, "identifier") == 0) {
    char *vmSegment = kindOf(token, firstTable);
    int vmIndex = indexOf(token, firstTable);
    writePush(filewrtr, vmSegment, &vmIndex);
    token_type = advance(index, filepntr, buffer, token);
  }

  free(next_token);
  free(subroutineName);
  return token_type;
}

int compileExpressionList(int *tab, int *index, char *buffer, char *token, char *token_type, char *nameOfClass, FILE *filewrtr, FILE *filepntr, tableNode *firstTable) {

  int argcounter = 0;

  // (expression (',' expression)*)?
  if (termCheck(token, token_type)) {
    compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
    argcounter++;
    while (token[0] == ',') {
      token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
      compileExpression(tab, index, buffer, token, token_type, nameOfClass, filewrtr, filepntr, firstTable);
      argcounter++;
    }
  }

  return argcounter;
}
