#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JackTokenizer.h"
#include "helper.h"

/*int hasMoreTokens(FILE *file) {

  return 0;
}*/

int advance(char *token, FILE *file) {
  char buffer[200] = {0};
  int comment_check = 0;

  while (1) {
    memset(buffer, 0, 200);
    fgets(buffer, 200, file);
    for (int i = 0; i < (int)strlen(buffer); i++) {
      printf("%s\n", buffer);
      if (comment_check == 1) {
        while (i < (int)strlen(buffer)) {
          if (buffer[i] == '*'  && buffer[i+1] == '/') {
            comment_check = 0;
            break;
          }
          else {
            i++;
          }
        }
      }
      else if (isspace(buffer[i])){
        while (isspace(buffer[i+1])) {
          i++;
        }
      }
      else if (buffer[i] == '/' && buffer[i+1] == '/') {
        break;
      }
      else if (buffer[i] == '/' && buffer[i+1] == '*') {
        comment_check = 1;
        while (i < (int)strlen(buffer)) {
          if (buffer[i] == '*'  && buffer[i+1] == '/') {
            comment_check = 0;
            break;
          }
          else {
            i++;
          }
        }
      }
      else if (isalpha(buffer[i])) {
        int it_is_a_keyword = checkKeyword(i, buffer, token);
        if (it_is_a_keyword) {
          printf("%s\n", token);
          return 1;
        }
      }
    }
    if (feof(file)) {
      printf("end\n");
      return 0;
    }
  }
}

/*char *tokenType(char *token) {

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
