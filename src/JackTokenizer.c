#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "helper.h"

void advance(int *index, FILE* filepntr, char *buffer, char *token) {
  char *KeywordsAndSymbols[40] = {"class", "constructor", "function", "method",
                                  "field", "static", "var", "int", "char", 
                                  "boolean", "void", "true", "false", "null",
                                  "this", "let", "do", "if", "else", "while", 
                                  "return", "{", "}", "(", ")", "[", "]", ".",
                                  ",", ";", "+", "-", "*", "/", "&", "|", "<",
                                  ">", "=", "~"};
  int found = 0;
  int temp_index = 0;
  //iterators
  int k;
  int m;

  for (*index = *index; *index < (int)strlen(buffer); (*index)++) {
    if (buffer[*index] == '\n'){
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[(*index)+1] == '/') {
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[(*index)+1] == '*') {
      while (found == 0) {
        for (int m = *index; m < (int)strlen(buffer); m++) {
          if (buffer[m] == '*' && buffer[m+1] == '/') {
            *index = m + 1;
            found = 1;
          }
        }
        if (found == 0) {
          getNextLine(index, buffer, filepntr);
        }
      }
    }
    else if (isgraph(buffer[*index])) {
      for (k = 0; k < 40; k++) {
        for (m = *index; m < *index + (int)strlen(KeywordsAndSymbols[k]); m++) {
          token[temp_index] = buffer[m];
          temp_index++;
        }
        if (strcmp(token, KeywordsAndSymbols[k]) == 0) {
          *index = m;
          return;
        }
        else {
          temp_index = 0;
          memset(token, 0, 200);
        }
      }
    }
  }
}
