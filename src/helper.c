#include <stdio.h>
#include <string.h>
#include "helper.h"

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
