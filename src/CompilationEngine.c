#include <string.h>
#include <stdio.h>
#include "helper.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"

void compileClass(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  // I've opted to include a local variable 
  // (token_type) for each compiler function to reduce 
  // the bloated parameters list
  char *token_type = NULL;

  // Opening tag
  fputs("<class>\n", filewrtr);

  // First terminal in class grammar ('class') written 
  // to xml and next token fetched.
  token_type = process(tab, index, buffer, token, "class", "keyword", filewrtr, filepntr);

  // Second terminal (className)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // Third terminal ('{')
  token_type = process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // First non-terminal (classVarDec*)
  while (strcmp(token, "static") == 0 || strcmp(token, "field") == 0) {
    printTabs(tab, filewrtr);
    compileClassVarDec(tab, index, buffer, token, filewrtr, filepntr);
  }

  // Second non-terminal (subroutineDec*)
  while (strcmp(token, "constructor") == 0 || strcmp(token, "function") == 0
          || strcmp(token, "method") == 0) {
    printTabs(tab, filewrtr);
    compileSubroutine(tab, index, buffer, token, filewrtr, filepntr);
  }

  // Fourth terminal ('}')
  token_type = process(tab, index, buffer, token, "}", "symbol", filewrtr, filepntr);

  // Closing tag for class non-terminal
  fputs("</class>\n", filewrtr);
} 

void compileClassVarDec(int *tab, int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  // Increment number of tabs
  (*tab)++;

  // Opening tag for class-level variable declarations
  fputs("<classVarDec>\n", filewrtr);

  // First terminal ('static'|'field') 
  token_type = process(tab, index, buffer, token, token, "keyword", filewrtr, filepntr);
  
  // Second terminal ('int' | 'boolean' | 'char' | className)
  if (strcmp(token, "int") == 0 || strcmp(token, "boolean") == 0 
      || strcmp(token, "char") == 0 || strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  } 
  else {
    printf("Syntax Error in compileClassVarDec\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Third terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileClass\n");
  }

  // Get next token
  advance(index, filepntr, buffer, token);

  // Implements the optional variable list ((',' varName)*)
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (strcmp(token_type, "identifier") == 0) {
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
}

void compileSubroutine(int *tab, int *index, char *buffer, char *token, 
                          FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  (*tab)++;
  
  // Opening tag for subroutine 
  fputs("<subroutineDec>\n", filewrtr);

  // First terminal ('method' |'function' | 'method')
  token_type = process(tab, index, buffer, token, token, "keyword", filewrtr, filepntr);

  // Second terminal ('void' | type)
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

  // Third terminal (subroutineName)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileSubroutine: no subroutine name given");
  }

  advance(index, filepntr, buffer, token);

  // Fourth terminal ('(')
  token_type = process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // First non-terminal (parameterList)
  printTabs(tab, filewrtr);
  compileParameterList(tab, index, buffer, token, token_type, filewrtr, filepntr);

  // Fifth terminal (')')
  token_type = process(tab, index, buffer, token, ")", "symbol", filewrtr, filepntr);

  // Second non-terminal (subroutineBody)
  printTabs(tab, filewrtr);
  compileSubroutineBody(tab, index, buffer, token, filewrtr, filepntr);

  (*tab)--;

  // Closing tag for subroutine
  printTabs(tab, filewrtr);
  fputs("</subroutineDec>\n", filewrtr);

}

void compileParameterList(int *tab, int *index, char *buffer, char *token, char *token_type, 
                          FILE *filewrtr, FILE *filepntr) {

  (*tab)++;

  // Opening tag for parameterList
  fputs("<parameterList>\n", filewrtr);

  // First terminal (type) -- 
  // if not a type, assume empty parameter list
  if (typeCheck(token, token_type)) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    return;
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Second terminal (varName)
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax Error in compileParameterList: no varName\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Beginning of (',' varName)*
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);
    if (typeCheck(token, token_type)) {
      printXMLToken(tab, token, token_type, filewrtr);
    }
    else {
      printf("Syntax Error in compileParameterList: No type in one of the parameter\n");
    }
    token_type = advance(index, filepntr, buffer, token);
    if (strcmp(token_type, "identifier") == 0) {
      printXMLToken(tab, token, token_type, filewrtr);
    }
    else {
      printf("Syntax error in compileParamterList: no varName in one of the parameters\n");
    }
    token_type = advance(index, filepntr, buffer, token);
  }

  (*tab)--;

  // Closing tag for parameterList
  printTabs(tab, filewrtr);
  fputs("</parameterList>\n", filewrtr);
}

void compileSubroutineBody(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, 
                           FILE *filepntr) {

  (*tab)++;
  // Opening tag for subroutineBody
  fputs("<subroutineBody>\n", filewrtr);

  // First terminal ('{')
  process(tab, index, buffer, token, "{", "symbol", filewrtr, filepntr);

  // Implementation of varDec*
  while (strcmp(token, "var") == 0) {
    printTabs(tab, filewrtr);
    compileVarDec(tab, index, buffer, token, filewrtr, filepntr);
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

void compileVarDec(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;

  (*tab)++;

  // Opening tag for varDec
  fputs("<varDec>\n", filewrtr);

  // First terminal ("var")
  token_type = process(tab, index, buffer, token, "var", "keyword", filewrtr, filepntr);

  // Second terminal (type)
  if (typeCheck(token, token_type)) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax error in compileVarDec: no data type\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Check for varName
  if (strcmp(token_type, "identifier") == 0) {
    printXMLToken(tab, token, token_type, filewrtr);
  }
  else {
    printf("Syntax error in compileVarDec: no varName given\n");
  }

  // Get next token
  token_type = advance(index, filepntr, buffer, token);

  // Implements (',' varName)*
  while (strcmp(token, ",") == 0) {
    token_type = process(tab, index, buffer, token, ",", "symbol", filewrtr, filepntr);

    if (strcmp(token_type, "identifier") == 0) {
      printXMLToken(tab, token, token_type, filewrtr);
    }
    else {
      printf("Syntax error in compileVarDec: missing varName following ','\n");
    }

    token_type = advance(index, filepntr, buffer, token);
  }

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
  fputs("<statement> ", filewrtr);

  if (strcmp(token, "let") == 0) {
    compileLet(tab, index, buffer, token, filewrtr, filepntr);
    fputs(" </statement>\n", filewrtr);
  }
  else if (strcmp(token, "if") == 0) {
    compileIf(tab, index, buffer, token, filewrtr, filepntr);
    fputs(" </statement>\n", filewrtr);
  }
  else if (strcmp(token, "while") == 0) {
    compileWhile(tab, index, buffer, token, filewrtr, filepntr);
    fputs(" </statement>\n", filewrtr);
  }
  else if (strcmp(token, "do") == 0) {
    compileDo(tab, index, buffer, token, filewrtr, filepntr);
    fputs(" </statement>\n", filewrtr);
  }
  else if (strcmp(token, "return") == 0) {
    compileReturn(tab, index, buffer, token, filewrtr, filepntr);
    fputs(" </statement>\n", filewrtr);
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
    compileExpression(tab, index, buffer, token, filewrtr, filepntr);

    token_type = process(tab, index, buffer, token, "]", "symbol", filewrtr, filepntr);
  }

  // '='
  token_type = process(tab, index, buffer, token, "=", "symbol", filewrtr, filepntr);

  // expression
  printTabs(tab, filewrtr);
  compileExpression(tab, index, buffer, token, filewrtr, filepntr);

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  (*tab)--;

  printTabs(tab, filewrtr);
  fputs("</letStatement>", filewrtr);

}

void compileIf(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  (*tab)++;

  fputs("<ifStatement>\n", filewrtr);

  // 'if'
  process(tab, index, buffer, token, "if", "keyword", filewrtr, filepntr);

  // '('
  process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);

  // expression
  printTabs(tab, filewrtr);
  compileExpression(tab, index, buffer, token, filewrtr, filepntr);

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
  fputs("</ifStatement>", filewrtr);
}

void compileWhile(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {

  (*tab)++;
  
  // Opening tag
  fputs("<whileStatement>\n", filewrtr);

  // 'while'
  process(tab, index, buffer, token, "while", "keyword", filewrtr, filepntr);

  // '('
  process(tab, index, buffer, token, "(", "symbol", filewrtr, filepntr);
  
  // expression
  printTabs(tab, filewrtr);
  compileExpression(tab, index, buffer, token, filewrtr, filepntr);

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
  fputs("</whileStatement>", filewrtr);
}

void compileDo(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  (*tab)++;

  fputs("<doStatement>\n", filewrtr);

  // 'do'
  process(tab, index, buffer, token, "do", "statement", filewrtr, filepntr);

  // subroutineCall
  printTabs(tab, filewrtr);
  compileTerm(tab, index, buffer, token, filewrtr, filepntr);

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);

  (*tab)--;

  // Closing tag
  printTabs(tab, filewrtr);
  fputs("</doStatement>", filewrtr);
}

void compileReturn(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  char *token_type = NULL;
  (*tab)++;

  // Opening tag
  fputs("<returnStatement>\n", filewrtr);

  // 'return'
  token_type = process(tab, index, buffer, token, "return", "keyword", filewrtr, filepntr);

  // expression?
  if (strcmp(token_type, "identifier") == 0) {
    printTabs(tab, filewrtr);
    compileExpression(tab, index, buffer, token, filewrtr, filepntr);
  }

  // ';'
  process(tab, index, buffer, token, ";", "symbol", filewrtr, filepntr);
  
  (*tab)--;

  // Closing Tag
  printTabs(tab, filewrtr);
  fputs("</returnStatement>", filewrtr);
}

void compileExpression(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  (*tab)++;
  fputs("<expression>\n", filewrtr);

  advance(index, filepntr, buffer, token);

  (*tab)--; 
  printTabs(tab, filewrtr);
  fputs("</expression>\n", filewrtr);
}

void compileTerm(int *tab, int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  (*tab)++;
  fputs("<expression>\n", filewrtr);

  advance(index, filepntr, buffer, token);

  (*tab)--; 
  printTabs(tab, filewrtr);
  fputs("</expression>\n", filewrtr);
}
