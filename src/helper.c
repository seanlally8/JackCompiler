#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

int checkToken(int *index, char *buffer) {
  char temp_token[50] = {0};
  char *tokens[40] = {"class", "constructor", "function", 
                        "method", "field", "static", "var",
                        "int", "char", "boolean", "void",
                        "true", "false", "null", "this", "let",
                        "do", "if", "else", "while", "return",
                        "{", "}", "(", ")", "[", "]", ".", ",",
                        ";", "+", "-", "*", "/", "&", "|", "<",
                        ">", "=", "~"};
  int token_index = 0;
  int m;
  for (int k = 0; k < 40; k++) {
    for (m = *index; m < (int)strlen(tokens[k]); m++)  {
      temp_token[token_index] = buffer[m];
      token_index++;
    }
    if (strcmp(temp_token, tokens[k]) == 0) {
      *index = m;
      memset(temp_token, 0, 50);
      return 1;
    }
    token_index = 0;
    memset(temp_token, 0, 50);
  }
  return 0;
}

int skipToEnd(char *buffer, FILE *filepntr, int index) {
  int temp_index = index;
  do {
    for (temp_index = temp_index; temp_index < (int)strlen(buffer); temp_index++) {
      if (buffer[temp_index] == '*' && buffer[temp_index+1] == '/') {
        return temp_index;
     }
    }
    getNextLine(buffer, filepntr);
    temp_index = 0;
  } while (!feof(filepntr));
  return -1;
}

int getNextLine(char *buffer, FILE *filepntr){
  memset(buffer, 0, 200);
  fgets(buffer, 200, filepntr);
  return -1;
}

FILE *openFileToRead(char *filename, char *buffer) {
  FILE *filepntr = fopen(filename, "r");
  if (filepntr == NULL) {
    free(buffer);
    exit(1);
  }
  return filepntr;
}
