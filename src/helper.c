#include <string.h>

int checkKeyword(int i, char *buffer, char *token) {
  char *keywords[21] = {"class", "constructor", "function", 
                        "method", "field", "static", "var",
                        "int", "char", "boolean", "void",
                        "true", "false", "null", "this", "let",
                        "do", "if", "else", "while", "return"};
  int index = 0;

  for (int k = 0; k < 21; k++) {
    for (int m = i; m < (int)strlen(keywords[k]); m++)  {
      token[index] = buffer[m];
      index++;
    }
    if (strcmp(token, keywords[k]) == 0) {
      return 1;
    }
    index = 0;
    memset(token, 0, 200);
  }
  return 0;
}
