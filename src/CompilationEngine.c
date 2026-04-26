#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"

void compileClass(int *index, char *buffer, char *token, FILE *filewrtr, FILE *filepntr) {
  const char *token_type = NULL;

  fputs("<class>\n", filewrtr);

  token_type = process(index, buffer, token, "class", "keyword", filewrtr, filepntr);

  if (strcmp(token_type, "identifier") == 0) {
    fprintf(filewrtr, "<%s> %s </%s>\n", token_type, token, token_type);
  }
  else {
    printf("Syntax Error");
  }

  advance(index, filepntr, buffer, token);

  token_type = process(index, buffer, token, "{", "symbol", filewrtr, filepntr);

  if (strcmp(token, "static") == 0 || strcmp(token, "field") == 0) {
    compileClassVarDec(index, buffer, token, filewrtr, filepntr);
  }
} 


void compileClassVarDec(int *index, char *buffer, char *token, 
                        FILE *filewrtr, FILE *filepntr) {
  return;
}


