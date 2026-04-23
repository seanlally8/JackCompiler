#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JackTokenizer.h"

int main(int argc, char *argv[]) {

  // CHECK FOR TWO ARGUMENTS IN CLI
  if (argc != 2) {
    printf("No file or directory name given");
    return 1;
  }

  char *token = calloc(200, sizeof(char));

  FILE *file = fopen(argv[1], "r");
  while (advance(token, file)) {
    printf("%s\n", token);
    memset(token, 0, 200);
  }

  fclose(file);
  free(token);
}
