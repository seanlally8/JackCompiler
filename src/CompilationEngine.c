#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"

void compileClass(int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  // I've opted to include a local variable 
  // (token_type) for each compiler function to reduce 
  // the bloated parameters list
  char *token_type = NULL;

  // Opening tag
  fputs("<class>\n", filewrtr);

  // First terminal in class grammar ('class') written 
  // to xml and next token fetched.
  token_type = process(index, buffer, token, "class", "keyword", filewrtr, filepntr);

  // Second terminal (className)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // Third terminal ('{')
  token_type = process(index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // First non-terminal (classVarDec*)
  while (strcmp(token, "static") == 0 || strcmp(token, "field") == 0) {
    compileClassVarDec(index, buffer, token, filewrtr, filepntr);
  }

  // Second non-terminal (subroutineDec*)
  while (strcmp(token, "constructor") == 0 || strcmp(token, "function") == 0
          || strcmp(token, "method") == 0) {
    compileSubroutine(index, buffer, token, filewrtr, filepntr);
  }

  // Fourth terminal ('}')
  token_type = process(index, buffer, token, "}", "symbol", filewrtr, filepntr);

  // Closing tag for class non-terminal
  fputs("</class>\n", filewrtr);
} 

void compileClassVarDec(int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  // Opening tag for class-level variable declarations
  fputs("<classVarDec>", filewrtr);

  // First terminal ('static'|'field') 
  token_type = process(index, buffer, token, token, "keyword", filewrtr, filepntr);
  
  // Second terminal ('int' | 'boolean' | 'char' | className)
  if (strcmp(token, "int") == 0 || strcmp(token, "boolean") == 0 
      || strcmp(token, "char") == 0 || strcmp(token_type, "identifier") == 0) {
    printXMLToken(token, token_type, filewrtr);
  } 
  else {
    printf("Syntax Error in compileClassVarDec\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Third terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // Implements the optional variable list ((',' varName)*)
  while (strcmp(token, ",") == 0) {
    token_type = process(index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (strcmp(token_type, "identifier") == 0) {
      printXMLToken(token, token_type, filewrtr);
    }
    else {
      printf("Syntax Error in compileClassVarDec: No varName");
    }
    advance(index, filepntr, buffer, token);
  }

  // Final terminal (";")
  process(index, buffer, token, ";", "symbol", filewrtr, filepntr);

  // Closing tag for class-level variable declarations
  fputs("</classVarDec>", filewrtr);
}

void compileSubroutine(int *index, char *buffer, char *token, 
                          FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;
  
  // Opening tag for subroutine 
  fputs("<subroutineDec>\n", filewrtr);

  // First terminal ('method' |'function' | 'method')
  token_type = process(index, buffer, token, token, "keyword", filewrtr, filepntr);

  // Second terminal ('void' | type)
  if (strcmp(token, "void") == 0 || strcmp(token, "int") == 0 
      || strcmp(token, "boolean") == 0 || strcmp(token, "char") == 0
      || strcmp(token_type, "identifier") == 0) {
    printXMLToken(token, token_type, filewrtr);
  }
  else {
    printf("Syntax Erroer in compileSubroutine: no type given\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Third terminal (subroutineName)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileSubroutine: no subroutine name given");
  }

  // Fourth terminal ('(')
  token_type = process(index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // First non-terminal (parameterList)
  compileParameterList(index, buffer, token, token_type, filewrtr, filepntr);

  // Second non-terminal (subroutineBody)
  compileSubroutineBody(index, buffer, token, filewrtr, filepntr);

  // Closing tag for subroutine
  fputs("</subroutineDec>\n", filewrtr);
}

void compileParameterList(int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr) {

  // Opening tag for parameterList
  fputs("<parameterList>\n", filewrtr);

  // First terminal (type) -- 
  // if not a type, assume empty parameter list
  if (typeCheck(token, token_type)) {
    printXMLToken(token, token_type, filewrtr);
  }
  else {
    return;
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Second terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileParameterList: no varName\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Beginning of (',' varName)*
  while (strcmp(token, ",") == 0) {
    printXMLToken(token, token_type, filewrtr);
    token_type = advance(index, filepntr, buffer, token);
    if (typeCheck(token, token_type)) {
      printXMLToken(token, token_type, filewrtr);
    }
    else {
      printf("Syntax Error in compileParameterList: No type in one of the parameter\n");
    }
    token_type = advance(index, filepntr, buffer, token);
    if (strcmp(token_type, "identifier") == 0) {
      printXMLToken(token, token_type, filewrtr);
    }
    else {
      printf("Syntax error in compileParamterList: no varName in one of the parameters\n");
    }
    token_type = advance(index, filepntr, buffer, token);
  }

  // Closing tag for parameterList
  fputs("</parameterList>\n", filewrtr);
}

void compileSubroutineBody(int *index, char *buffer, char *token, FILE *filewrtr, 
                           FILE *filepntr) {

}
