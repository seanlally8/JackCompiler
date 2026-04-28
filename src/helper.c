#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "JackTokenizer.h"

void getNextLine(int *index, char *buffer, FILE *filepntr) {
  // Jumps to next line of file, resets index to 0 
  // (i.e. the beginning of the line), and zeros out the buffer
  // for a clean read.
  memset(buffer, 0, 200);
  *index = -1;
  fgets(buffer, 200, filepntr);
}

void skipToEndOfComment(char *buffer, int *index, FILE *filepntr) {
  // Skips to the last character of comments starting with /*
  int found = 0;
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

char *nameFile(char *filename, char *extension) {
  for (int t = 0; t < (int)strlen(filename); t++){
    if (filename[t] == '.'){
      filename[t] = 'S';
      filename[t + 1] = '\0';
      strcat(filename, extension);
      break;
    }
  }
  return filename;
}

char *process(int *index, char *buffer, char *token, char *expected_token, 
              char *token_type, FILE *filewrtr, FILE *filepntr) {
  if (strcmp(token, expected_token) == 0) {
    printXMLToken(expected_token, token_type, filewrtr);
  } 
  else {
    printf("Syntax Error in process\n");
  }
  char *new_token_type = advance(index, filepntr, buffer, token);
  return new_token_type;
}

void printXMLToken(char *token, char *token_type, FILE *filewrtr) {
  fprintf(filewrtr, "<%s> %s </%s>\n", token_type, token, token_type);
}

int typeCheck(char *token, char *token_type) {
  // Checks whether the given token is a data type 
  if (strcmp(token, "int") == 0 || strcmp(token, "char") == 0
      || strcmp(token, "boolean") == 0 || strcmp(token_type, "identifier") == 0) {
    return 1;
  }
  else {
    return 0;
  }
}
