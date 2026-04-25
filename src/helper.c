#include <stdio.h>
#include <string.h>

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

int checkIfKeywordOrSymbol(int *index, char *token, char *buffer) {
  // Checks whether the current character is a symbol or the beginning
  // of a keyword.

  // Array of all keywords and symbols
  char *KeywordsAndSymbols[40] = {"class", "constructor", "function", "method",
                                  "field", "static", "var", "int", "char", 
                                  "boolean", "void", "true", "false", "null",
                                  "this", "let", "do", "if", "else", "while", 
                                  "return", "{", "}", "(", ")", "[", "]", ".",
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
  for (k = 0; k < 40; k++) {
    for (m = *index; m < *index + (int)strlen(KeywordsAndSymbols[k]); m++) {
      token[temp_index] = buffer[m];
      temp_index++;
    }
    if (strcmp(token, KeywordsAndSymbols[k]) == 0) {
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
