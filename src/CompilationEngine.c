#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include "SymbolTable.h"

void compileClass(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  // I've opted to include a local variable 
  // (token_type) for each compiler function to reduce 
  // the bloated parameters list
  char *token_type = NULL;
  

  node **classTable = initTable(); 

  // Opening tag
  fputs("<class>\n", filewrtr);

  // 'class'  
  token_type = process(tab, index, buffer, token, "class", "keyword", filewrtr, filepntr);
  
  char *nameOfClass = calloc(1, strlen(token) + 1);

  // className 
  if (strcmp(token_type, "identifier") == 0) {
    strcpy(nameOfClass, token);
    printXMLToken(tab, token, token_type, filewrtr);
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
    printTabs(tab, filewrtr);
    compileClassVarDec(tab, index, buffer, token, filewrtr, filepntr, classTable);
  }

  printf("CLASS TABLE\n");
  printTable(classTable);
  printf("----------------\n");

  // subroutineDec*
  while (strcmp(token, "constructor") == 0 || strcmp(token, "function") == 0
          || strcmp(token, "method") == 0) {
    printTabs(tab, filewrtr);
    compileSubroutine(tab, index, buffer, token, nameOfClass, filewrtr, filepntr);
  }

  // '}'
  token_type = process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  // Closing tag for class non-terminal
  fputs("</class>\n", filewrtr);

  reset(classTable);
  free(classTable);
  free(nameOfClass);
} 

void compileClassVarDec(int *tab, int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr, node **hashTable) {
  char *token_type = NULL;
  char *tableItems[3] = {0};

  // Increment number of tabs
  (*tab)++;

  // Opening tag for class-level variable declarations
  fputs("<classVarDec>\n", filewrtr);

  // ('static'|'field') 
  tableItems[0] = calloc(1, (strlen(token) + 1));
  strcpy(tableItems[0], token);
  token_type = process(tab, index, buffer, token, token, "keyword", filewrtr, filepntr);
  
  // ('int' | 'boolean' | 'char' | className)
  if (strcmp(token, "int") == 0 || strcmp(token, "boolean") == 0 
      || strcmp(token, "char") == 0 || strcmp(token_type, "identifier") == 0) {
    tableItems[1] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[1], token);
    printXMLToken(tab, token, token_type, filewrtr);
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
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  define(tableItems[2], tableItems[1], tableItems[0], hashTable);
  free(tableItems[2]);

  // Get next token
  advance(index, filepntr, buffer, token);

  // Implements the optional variable list ((',' varName)*)
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (strcmp(token_type, "identifier") == 0) {
      tableItems[2] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[2], token);
      define(tableItems[2], tableItems[1], tableItems[0], hashTable);
      free(tableItems[2]);
      printXMLToken(tab, token, token_type, filewrtr);
    }
    else {
      printf("Syntax Error in compileClassVarDec: No varName");
    }
    advance(index, filepntr, buffer, token);
  }

  // Final terminal (";")
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  // Decrement number of tabs before returning
  (*tab)--;

  // Closing tag for class-level variable declarations
  printTabs(tab, filewrtr);
  fputs("</classVarDec>\n", filewrtr);

  for (int i = 0; i < 2; i++) {
    free(tableItems[i]);
  }
}

void compileSubroutine(int *tab, int *index, char *buffer, char *token, char *nameOfClass, 
                          FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  node **subroutineTable = initTable();

  (*tab)++;
  
  // Opening tag for subroutine 
  fputs("<subroutineDec>\n", filewrtr);

  // First terminal ('method' |'function' | 'constructor')
  if (strcmp(token, "method") ==0) {
    define("this", nameOfClass, "arg", subroutineTable);
  }
  token_type = process(tab, index, buffer, token, token, "keyword", filewrtr, filepntr);

  // ('void' | type)
  if (strcmp(token, "void") == 0 || strcmp(token, "int") == 0 
      || strcmp(token, "boolean") == 0 || strcmp(token, "char") == 0
      || strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Erroer in compileSubroutine: no type given\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // subroutineName
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileSubroutine: no subroutine name given");
  }

  advance(index, filepntr, buffer, token);

  // '('
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // parameterList
  printTabs(tab, filewrtr);
  compileParameterList(tab, index, buffer, token, token_type, filewrtr, filepntr, subroutineTable);

  // ')'
  token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // subroutineBody
  printTabs(tab, filewrtr);
  compileSubroutineBody(tab, index, buffer, token, filewrtr, filepntr, subroutineTable);

  (*tab)--;

  // Closing tag for subroutine
  printTabs(tab, filewrtr);
  fputs("</subroutineDec>\n", filewrtr);

  printf("SUBROUTINE TABLE\n");
  printTable(subroutineTable);
  printf("-------------\n");

  reset(subroutineTable);
  free(subroutineTable);

}

void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr, node **hashTable) {

  char *tableItems[2] = {0};

  (*tab)++;

  // Opening tag for parameterList
  fputs("<parameterList>\n", filewrtr);

  // type 
  // if not a type, assume empty parameter list
  if (typeCheck(token, token_type)) {
    tableItems[0] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[0], token);
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
  }
  else {
    (*tab)--;
    printTabs(tab, filewrtr);
    fputs("</parameterList>\n", filewrtr);
    return;
  }

  // Second terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    tableItems[1] = calloc(1, (strlen(token) + 1));
    strcpy(tableItems[1], token);
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
  }
  else {
    printf("Syntax Error in compileParameterList: no varName\n");
  }

  // Add first parameter to table
  define(tableItems[1], tableItems[0], "arg", hashTable);

  for (int i = 0; i < 2; i++) {
    free(tableItems[i]);
  }


  // Beginning of (',' type varName)*
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (typeCheck(token, token_type)) {
      tableItems[0] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[0], token);
      printXMLToken(tab, token, token_type, filewrtr);
      token_type = advance(index, filepntr, buffer, token);
    }
    else {
      printf("Syntax Error in compileParameterList: No type in one of the parameter\n");
    }
    if (strcmp(token_type, "identifier") == 0) {
      tableItems[1] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[1], token);
      printXMLToken(tab, token, token_type, filewrtr);
      token_type = advance(index, filepntr, buffer, token);
    }
    else {
      printf("Syntax error in compileParamterList: no varName in one of the parameters\n");
    }
    define(tableItems[1], tableItems[0], "arg", hashTable);
    for (int i = 0; i < 2; i++) {
      free(tableItems[i]);
    }
  }

  (*tab)--;

  // Closing tag for parameterList
  printTabs(tab, filewrtr);
  fputs("</parameterList>\n", filewrtr);
}

void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, 
                           FILE *filepntr, node **hashTable) {

  (*tab)++;
  // Opening tag for subroutineBody
  fputs("<subroutineBody>\n", filewrtr);

  // First terminal ('{')
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // Implementation of varDec*
  while (strcmp(token, "var") == 0) {
    printTabs(tab, filewrtr);
    compileVarDec(tab, index, buffer, token, filewrtr, filepntr, hashTable);
  }


  // Implementation of statements rule
  printTabs(tab, filewrtr);
  compileStatements(tab, index, buffer, token, filewrtr, filepntr);

  // Second terminal ('}')
  process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  (*tab)--;

  // Closing tag for subroutineBody
  printTabs(tab, filewrtr);
  fputs("</subroutineBody>\n", filewrtr);
}

void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr, node **hashTable) {
  char *token_type = NULL;
  char *tableItems[2] = {0};

  (*tab)++;

  // Opening tag for varDec
  fputs("<varDec>\n", filewrtr);

  // First terminal ("var")
  token_type = process(tab, index, buffer, token, "var", "keyword", filewrtr, filepntr);

  // Second terminal (type)
  if (typeCheck(token, token_type)) {
    tableItems[0] = calloc(1, strlen(token) + 1);
    strcpy(tableItems[0], token);
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax error in compileVarDec: no data type\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Check for varName
  if (strcmp(token_type, "identifier") == 0) {
    tableItems[1] = calloc(1, strlen(token) + 1);
    strcpy(tableItems[1], token);
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax error in compileVarDec: no varName given\n");
  }

  define(tableItems[1], tableItems[0], "var", hashTable);
  free(tableItems[1]);

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Implements (',' varName)*
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);

    if (strcmp(token_type, "identifier") == 0) {
      tableItems[1] = calloc(1, strlen(token) + 1);
      strcpy(tableItems[1], token);
      define(tableItems[1], tableItems[0], "var", hashTable);
      free(tableItems[1]);
      printXMLToken(tab, token, token_type, filewrtr);
    }
    else {
      printf("Syntax error in compileVarDec: missing varName following ','\n");
    }

    token_type = advance(index, filepntr, buffer, token);
  }

  free(tableItems[0]);

  // Last terminal ';'
  token_type = process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  (*tab)--;

  // Closing tag for varDec
  printTabs(tab, filewrtr);
  fputs("</varDec>\n", filewrtr);
}

void compileStatements(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  (*tab)++;

  fputs("<statements>\n", filewrtr);

  // statement*
  while (strcmp(token, "let") == 0 || strcmp(token, "while") == 0 || strcmp(token, "if") == 0 
      || strcmp(token, "do") == 0 || strcmp(token, "return") == 0) {
    printTabs(tab, filewrtr);
    compileStatement(tab, index, buffer, token, filewrtr, filepntr);
  }

  (*tab)--;

  printTabs(tab, filewrtr);
  fputs("</statements>\n", filewrtr);
}

void compileStatement(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  if (strcmp(token, "let") == 0) {
    compileLet(tab, index, buffer, token, filewrtr, filepntr);
  }
  else if (strcmp(token, "if") == 0) {
    compileIf(tab, index, buffer, token, filewrtr, filepntr);
  }
  else if (strcmp(token, "while") == 0) {
    compileWhile(tab, index, buffer, token, filewrtr, filepntr);
  }
  else if (strcmp(token, "do") == 0) {
    compileDo(tab, index, buffer, token, filewrtr, filepntr);
  }
  else if (strcmp(token, "return") == 0) {
    compileReturn(tab, index, buffer, token, filewrtr, filepntr);
  }
  
}

void compileLet(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  (*tab)++;

  // FIGURE OUT FORMATTING FOR STATEMENTS
  fputs("<letStatement>\n", filewrtr);

  // 'let'
  token_type = process(tab, index, buffer, token, "let", "keyword", filewrtr, filepntr);

  // varName
  if (strcmp(token_type, "identifier") == 0) {
        printXMLToken(tab, token, token_type, filewrtr);
      }
  else {
    printf("Syntax error in compileVarDec: missing varName following ','\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // ('[' expression ']')?
  if (strcmp(token, "[") == 0) {
    token_type = process(tab, index, buffer, token, "[", "symbol", filewrtr, filepntr);

    printTabs(tab, filewrtr);
    compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);

    token_type = process(tab, index, buffer, token, "]", "symbol", filewrtr, filepntr);
  }

  // '='
  token_type = process(tab, index, buffer, token, "=", "symbol", filewrtr, filepntr);

  // expression
  printTabs(tab, filewrtr);
  compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  (*tab)--;

  printTabs(tab, filewrtr);
  fputs("</letStatement>\n", filewrtr);

}

void compileIf(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  (*tab)++;

  fputs("<ifStatement>\n", filewrtr);

  // 'if'
  process(tab, index, buffer, token, "if", "keyword", filewrtr, filepntr);

  // '('
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // expression
  printTabs(tab, filewrtr);
  compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);

  // ')'
  process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // '{'
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // statements
  printTabs(tab, filewrtr);
  compileStatements(tab, index, buffer, token, filewrtr, filepntr);

  // '}'
  process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  // ('else' '{' statements '}')?
  if (strcmp(token, "else") == 0) {
    process(tab, index, buffer, token, "else", "keyword", filewrtr, filepntr);
    process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);
    printTabs(tab, filewrtr);
    compileStatements(tab, index, buffer, token, filewrtr, filepntr);
    process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);
  }

  (*tab)--;

  printTabs(tab, filewrtr);
  fputs("</ifStatement>\n", filewrtr);
}

void compileWhile(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  (*tab)++;
  
  // Opening tag
  fputs("<whileStatement>\n", filewrtr);

  // 'while'
  process(tab, index, buffer, token, "while", "keyword", filewrtr, filepntr);

  // '('
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
  
  // expression
  printTabs(tab, filewrtr);
  compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);

  // ')'
  process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // '{'
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // statements
  printTabs(tab, filewrtr);
  compileStatements(tab, index, buffer, token, filewrtr, filepntr);
  
  // '}'
  process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  (*tab)--;

  printTabs(tab, filewrtr);
  fputs("</whileStatement>\n", filewrtr);
}

void compileDo(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;
  (*tab)++;

  fputs("<doStatement>\n", filewrtr);

  // 'do'
  token_type = process(tab, index, buffer, token, "do", "keyword", filewrtr, filepntr);

  char *next_token = tokenLookAhead(index, filepntr, buffer);

  // subroutineCall
  if (strcmp(token_type, "identifier") == 0 && next_token[0] == '(') {
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
    printTabs(tab, filewrtr);
    compileExpressionList(tab, index, buffer, token, token_type, filewrtr, filepntr);
    process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 
  }
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '.') {
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, ".", "symbol", filewrtr, filepntr); 
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr); 
    printTabs(tab, filewrtr);
    compileExpressionList(tab, index, buffer, token, token_type, filewrtr, filepntr);
    process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 
  }

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  free(next_token);

  (*tab)--;

  // Closing tag
  printTabs(tab, filewrtr);
  fputs("</doStatement>\n", filewrtr);
}

void compileReturn(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;
  (*tab)++;

  // Opening tag
  fputs("<returnStatement>\n", filewrtr);

  // 'return'
  token_type = process(tab, index, buffer, token, "return", "keyword", filewrtr, filepntr);

  // expression?
  if (termCheck(token, token_type)) {
    printTabs(tab, filewrtr);
    compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);
  }

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);
  
  (*tab)--;

  // Closing Tag
  printTabs(tab, filewrtr);
  fputs("</returnStatement>\n", filewrtr);
}

void compileExpression(int *tab, int *index, char *buffer, char *token, char *token_type, FILE *filewrtr, FILE *filepntr) {

  (*tab)++;
  fputs("<expression>\n", filewrtr);

  // term
  printTabs(tab, filewrtr);
  token_type = compileTerm(tab, index, buffer, token, token_type, filewrtr, filepntr);

  // (op term)*
  while (opCheck(token)) {
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
    printTabs(tab, filewrtr);
    compileTerm(tab, index, buffer, token, token_type, filewrtr, filepntr);
  }

  (*tab)--; 
  printTabs(tab, filewrtr);
  fputs("</expression>\n", filewrtr);
}

char *compileTerm(int *tab, int *index, char *buffer, char *token, char *token_type, FILE *filewrtr, FILE *filepntr) {

  (*tab)++;
  fputs("<term>\n", filewrtr);

  char *next_token = tokenLookAhead(index, filepntr, buffer);
  
  // integerConstant | stringConstant | keywordConstant 
  // | varName | varName '[' expression ']' | '(' expression ')'
  // | (unaryOp term) | subroutineCall

  if (strcmp(token_type, "integerConstant") == 0) { 
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
  }
  else if (strcmp(token_type, "stringConstant") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
  }
  else if (keywordConstantCheck(token)) {
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
  }
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '[') {
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "[", "symbol", filewrtr, filepntr);
    printTabs(tab, filewrtr);
    compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);
    token_type = process(tab, index, buffer, token, "]", "symbol", filewrtr, filepntr);
  }
  else if (strcmp(token, "(") == 0) {
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
    printTabs(tab, filewrtr);
    compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);
    token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);
  }
  else if (strcmp(token, "-") == 0 || strcmp(token, "~") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
    printTabs(tab, filewrtr);
    token_type = compileTerm(tab, index, buffer, token, token_type, filewrtr, filepntr);
  }
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '(') {
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
    printTabs(tab, filewrtr);
    compileExpressionList(tab, index, buffer, token, token_type, filewrtr, filepntr);
    token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 
  }
  else if (strcmp(token_type, "identifier") == 0 && next_token[0] == '.') {
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, ".", "symbol", filewrtr, filepntr); 
    printXMLToken(tab, token, token_type, filewrtr);
    advance(index, filepntr, buffer, token);
    token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr); 
    printTabs(tab, filewrtr);
    compileExpressionList(tab, index, buffer, token, token_type, filewrtr, filepntr);
    token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr); 
  }
  else if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
  }

  free(next_token);
  

  (*tab)--; 
  printTabs(tab, filewrtr);
  fputs("</term>\n", filewrtr);

  return token_type;
}

void compileExpressionList(int *tab, int *index, char *buffer, char *token, char *token_type, FILE *filewrtr, FILE *filepntr) {
  (*tab)++;

  fputs("<expressionList>\n", filewrtr);

  // (expression (',' expression)*)?
  if (termCheck(token, token_type)) {
    printTabs(tab, filewrtr);
    compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);
    while (token[0] == ',') {
      token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
      printTabs(tab, filewrtr);
      compileExpression(tab, index, buffer, token, token_type, filewrtr, filepntr);
    }
  }

  (*tab)--;
  printTabs(tab, filewrtr);
  fputs("</expressionList>\n", filewrtr);
}
