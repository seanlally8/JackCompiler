#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JackTokenizer.h"
#include "helper.h"

int main(int argc, char *argv[]) {

  // CHECK FOR TWO ARGUMENTS IN CLI
  if (argc != 2) {
    printf("No file or directory name given");
    return 1;
  }

  int index = 0;
  FILE *filepntr = fopen(argv[1], "r");
  if (filepntr == NULL) {
    printf("Can't open file");
    return 1;
  }
  char *buffer = calloc(200, sizeof(char));
  char *token = calloc(200, sizeof(char));

  getNextLine(&index, buffer, filepntr);

  while (!feof(filepntr)) {
    advance(&index, filepntr, buffer, token);
    printf("%s\n", token);
    memset(token, 0, 200);
  }
  printf("reached the end\n");

  fclose (filepntr);
  free(buffer);
  free(token);


}
