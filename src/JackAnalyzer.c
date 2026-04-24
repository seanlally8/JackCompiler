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

  char *filename = argv[1];
  int index = 0;
  char *buffer = calloc(200, sizeof(char));

  FILE *filepntr = openFileToRead(filename, buffer);
 
  getNextLine(buffer, filepntr);

  while (hasMoreTokens(buffer, &index, filepntr)) {
    printf("here\n");
  }
  printf("end reached");

  fclose(filepntr);
  free(buffer);
}
