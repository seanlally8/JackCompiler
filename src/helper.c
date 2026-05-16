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

char *process(int *tab, int *index, char *buffer, char *token, char *expected_token, 
              char *token_type, FILE *filewrtr, FILE *filepntr) {
  if (strcmp(token, expected_token) == 0) {
    printXMLToken(tab, expected_token, token_type, filewrtr);
  } 
  else {
    printf("Syntax Error in process: level: %i, token: %s, expected_token: %s\n", *tab, token, expected_token);
  }
  char *new_token_type = advance(index, filepntr, buffer, token);
  return new_token_type;
}

void printTabs(int *tab, FILE *filewrtr) {
  for (int i = 0; i < *tab; i++) {
    fputc('\t', filewrtr);
  }
}

void printXMLToken(int *tab, char *token, char *token_type, FILE *filewrtr) {
  printTabs(tab, filewrtr);
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

int opCheck(char *token) {
  // Checks to see if token is an operation
  char *opList = "+-*/&|<>=";

  for (int i = 0; i < (int)strlen(opList); i++) {
    if (opList[i] == token[0]) {
      return 1;
    }
  }

  return 0;
}

int keywordConstantCheck(char *token) {
  // Checks to see if token is a keywordConstant
  char *keywordList[4] = {"true", "false", "null", "this"};

  for (int i = 0; i < 4; i++) {
    if (strcmp(token, keywordList[i]) == 0) {
      return 1;
    } 
  }

  return 0;
}

int termCheck(char *token, char *token_type) {
  // Checks whether token is a term or the beginning of a term
  if (strcmp(token_type, "identifier") == 0 || strcmp(token_type, "integerConstant") == 0 
    || strcmp(token_type, "stringConstant") == 0 || keywordConstantCheck(token) || token[0] == '('
    || token[0] == '-' || token[0] == '~') {
    return 1;
  }

  return 0;
}

int fileCheck(char *inputname) {
  int count = 0;
  char ext[5] = {0};

  for (int g = 0; g < (int)strlen(inputname); g++) {
    if (inputname[g] == '.') {
      for (int i = g + 1; i < (int)strlen(inputname); i++) {
        ext[count] = inputname[i];
        count++;
      }
      if (strcmp("jack", ext) == 0) {
        return 1;
      }
      else {
        return -1; 
      }
    }
  }
  return 0;
}
