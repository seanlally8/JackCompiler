#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "JackTokenizer.h"

int hasMoreTokens(int *index, FILE *filepntr, char *buffer) {
  // Determines whether there are more tokens in the file before
  // 'advance()' continues delineating tokens
  for (*index = *index; *index < (int)strlen(buffer); (*index)++) {
    if (buffer[*index] == '\n'){
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[*index+1] == '/') {
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[*index+1] == '*') {
      skipToEndOfComment(buffer, index, filepntr);
    }
    else if (isgraph(buffer[*index])) {
      return 1;
    }
  }
  return 0;
}

char *advance(int *index, FILE* filepntr, char *buffer, char *token) {
  // Extracts all the tokens and returns their classification 

  memset(token, 0, 200);
  for (*index = *index; *index < (int)strlen(buffer); (*index)++) {
    if (buffer[*index] == '\n'){
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[*index+1] == '/') {
      getNextLine(index, buffer, filepntr);
    }
    else if (buffer[*index] == '/' && buffer[*index+1] == '*') {
      skipToEndOfComment(buffer, index, filepntr);
    }
    else if (buffer[*index] == '"') {
      extractStringConstant(index, buffer, token);
      return "stringConstant";
    }
    else if (isgraph(buffer[*index]) && !isdigit(buffer[*index])) {
      if (extractKeyword(index, token, buffer)) {
        return "keyword";
      }
      else if (extractSymbol(index, token, buffer)) {
        return "symbol";
      }
      else {
        extractIdentifier(index, buffer, token);
        return "identifier";
      }
    }
    else if (isdigit(buffer[*index])) {
      extractIntegerConstant(index, buffer, token);
      return "integerConstant";
    }
  }
  return "undefined";
}

void extractStringConstant(int *index, char *buffer, char *token) {
  // Extracts text between quotation marks.
  int temp_index = 0;
  int m = 0;
  for (m = *index + 1; buffer[m] != '"'; m++) {
    token[temp_index] = buffer[m];
    temp_index++;
  }
  *index = m + 1;
}

int extractKeyword(int *index, char *token, char *buffer) {
  // Checks whether the current character is the beginning
  // of a keyword, and then extracts the keyword.

  // Array of all keywords and symbols
  char *Keywords[21] = {"class", "constructor", "function", "method",
                        "field", "static", "var", "int", "char", 
                        "boolean", "void", "true", "false", "null",
                        "this", "let", "do", "if", "else", "while", "return"};

  // Index to be used for testing possible tokens
  int temp_index = 0;

  // Iterators
  int k = 0;
  int m = 0;

  // Iterate over all keywords and symbols and compare each entry 
  // to the substring of equal length starting at the currently selected 
  // character in the buffer.
  for (k = 0; k < 21; k++) {
    for (m = *index; m < *index + (int)strlen(Keywords[k]); m++) {
      token[temp_index] = buffer[m];
      temp_index++;
    }
    if (strcmp(token, Keywords[k]) == 0) {
      *index = m;
      return 1;
    }
    else {
      temp_index = 0;
      memset(token, 0, 200);
    }
  }
  return 0;
}

int extractSymbol(int *index, char *token, char *buffer) {
  // Checks whether the current character is a symbol or the beginning
  // of a keyword, and extracts the symbol or keyword.

  // Array of all keywords and symbols
  char *Symbols[19] = { "{", "}", "(", ")", "[", "]", ".",
                        ",", ";", "+", "-", "*", "/", "&", "|", "<",
                        ">", "=", "~"};

  // Index to be used for testing possible tokens
  int temp_index = 0;

  // Iterators
  int k = 0;
  int m = 0;

  // Iterate over all keywords and symbols and compare each entry 
  // to the substring of equal length starting at the currently selected 
  // character in the buffer.
  for (k = 0; k < 19; k++) {
    for (m = *index; m < *index + (int)strlen(Symbols[k]); m++) {
      token[temp_index] = buffer[m];
      temp_index++;
    }
    if (token[0] == Symbols[13][0]) {
      memset(token, 0, 200);
      strcpy(token, "&amp;");
      *index = m;
      return 1;
    }
    else if (token[0] == Symbols[15][0]) {
      memset(token, 0, 200);
      strcpy(token, "&lt;");
      *index = m;
      return 1;
    }
    else if (token[0] == Symbols[16][0]) {
      memset(token, 0, 200);
      strcpy(token, "&gt;");
      *index = m;
      return 1;
    }
    else if (strcmp(token, Symbols[k]) == 0) {
      *index = m;
      return 1;
    }
    else {
      temp_index = 0;
      memset(token, 0, 200);
    }
  }
  return 0;
}
void extractIdentifier(int *index, char *buffer, char *token) {
  // Extracts any bespoke identifiers (class names, variable names)
  // determines where the non-digit string ends by finding either a symbol
  // or a space.
  char *symbols[20] = {"{", "}", "(", ")", "[", "]", ".",
                  ",", ";", "+", "-", "*", "/", "&", 
                  "|", "<", ">", "=", "~", " "};
  int m = 0;
  int k = 0;
  int temp_index = 0;
  for (m = *index; m < (int)strlen(buffer); m++) {
    for (k = 0; k < 20; k++) {
      if (buffer[m] == symbols[k][0]) {
        *index = m;
        return;
      }
    }
    token[temp_index] = buffer[m];
    temp_index++;
  } 
}

void extractIntegerConstant(int *index, char *buffer, char *token) {
  // Extracts any integer constant. Stops when it hits a non-digit character. 
  int temp_index = 0;
  int m = 0;
  for (m = *index; isdigit(buffer[m]); m++) {
    token[temp_index] = buffer[m];
    temp_index++;
  }
  *index = m;
}
