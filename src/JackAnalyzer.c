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

  // Initialize index (which will keep track of the current 
  // index of buffer), and open input and output files
  int index = 0;
  FILE *filepntr = fopen(argv[1], "r");
  if (filepntr == NULL) {
    printf("Can't open input file");
    return 1;
  }
  char *filename = nameFile(argv[1], ".xml");
  FILE *filewrtr = fopen(filename, "w");
  if (filewrtr == NULL) {
    printf("Could not open .xml file");
    return 1;
  }

  // Buffer will store 1 line of text from the input file to
  // facilitate parsing and token will hold the current token 
  char *token_type = NULL;
  char *buffer = calloc(200, sizeof(char));
  char *token = calloc(200, sizeof(char));

  // Get first line of input file
  getNextLine(&index, buffer, filepntr);

  // Iterate over file to extract tokens, determine classification, 
  // and print to xml file
  fputs("<tokens>\n", filewrtr);
  while (hasMoreTokens(&index, filepntr, buffer)) {
    token_type = advance(&index, filepntr, buffer, token);
    fprintf(filewrtr, "<%s> %s </%s>\n", token_type, token, token_type);
  }
  fputs("</tokens>\n", filewrtr);

  // Close files and free memory
  fclose (filepntr);
  fclose (filewrtr);
  free(buffer);
  free(token);
}
