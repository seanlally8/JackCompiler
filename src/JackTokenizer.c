#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "helper.h"

void advance(int *index, FILE* filepntr, char *buffer, char *token) {
  // Extracts all the tokens so they can be classified 
  // and cleanly printed to xml
  for (*index = *index; *index < (int)strlen(buffer); (*index)++) {
    if (buffer[*index] == '\n'){
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[(*index)+1] == '/') {
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[(*index)+1] == '*') {
      skipToEndOfComment(buffer, index, filepntr);
    }
    else if (isgraph(buffer[*index])) {
      if (checkIfKeywordOrSymbol(index, token, buffer)) {
        return;
      }
    }
  }
}
