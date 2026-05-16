#include <stdio.h>
#include <stdlib.h>
#include "JackTokenizer.h"
#include "helper.h"

int main(int argc, char *argv[]) {
  (void)argc;
  int index = 0;
  FILE *filepntr = fopen(argv[1], "r");
  char *buffer = calloc(200, sizeof(char));
  char *token = calloc(200, sizeof(char));

  getNextLine(&index, buffer, filepntr);
  advance(&index, filepntr, buffer, token);
  printf("%s\n", token);
  char *next_token = tokenLookAhead(&index, filepntr, buffer);
  printf("%s\n", next_token);
  printf("%s\n", token);

  free(buffer);
  free(token);
  free(next_token);
  fclose(filepntr);
}
