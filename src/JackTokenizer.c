#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "JackTokenizer.h"
#include "helper.h"

int hasMoreTokens(char *buffer, int *index, FILE *filepntr) {
  
  for (int temp_index = *index; temp_index < (int)strlen(buffer); temp_index++) {
    printf("%c", buffer[temp_index]);
    if (buffer[temp_index] == '\n' || buffer[temp_index] == '\r'){
      temp_index = getNextLine(buffer, filepntr);
      printf("\n");
    }
    else if (isspace(buffer[temp_index])) {
      while (isspace(buffer[temp_index+1])) {
        temp_index++;
      }
    }
    else if (buffer[temp_index] == '/' && buffer[temp_index+1] == '/') {
      temp_index = getNextLine(buffer, filepntr);
      printf("\n");
    }
    else if (buffer[temp_index] == '/' && buffer[temp_index+1] == '*') {
      temp_index = skipToEnd(buffer, filepntr, temp_index);
    }
    else if (isgraph(buffer[temp_index])) {
      int it_is_a_token = checkToken(&temp_index, buffer);
      if (it_is_a_token) {
        *index = temp_index;
        return 1;
      }
    }
  }
  if (feof(filepntr)) {
    return 0;
  }
  return 0;
}


/*int advance(char *token, FILE *file) {
}

char *tokenType(char *token) {

  return "temp";
}

char *keyword(char *token) {

  return "temp";
}

char symbol(char *token) {

  return 't';
}

char *identifier(char *token) {

  return "temp";
}

int intVal(char *token) {

  return 0;
}

char *stringVal(char *token) {

  return "temp";
}*/
