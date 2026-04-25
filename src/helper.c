#include <stdio.h>
#include <string.h>

void getNextLine(int *index, char *buffer, FILE *filepntr) {
  memset(buffer, 0, 200);
  *index = -1;
  fgets(buffer, 200, filepntr);
}
