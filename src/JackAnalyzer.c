#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "JackTokenizer.h"
#include "helper.h"
#include "CompilationEngine.h"

int main(int argc, char *argv[]) {

  // CHECK FOR TWO ARGUMENTS IN CLI
  if (argc != 2) {
    printf("No file or directory name given");
    return 1;
  }

  // initialize index for tracking characters in stream, tab for formatting xml
  // inputname to give a meaningful name to the cli input, and a node for building
  // linked list.
  int index = 0;
  int tab = 1;
  char *inputname = argv[1];

  typedef struct node {
    char filename[200];
    struct node *next;
  } node;

  node *head = NULL;

  // Create linked list of 1 or more nodes containing the filenames/paths
  if (fileCheck(inputname) == -1) {
    printf("Not a .jack file\n");
    return 1;
  }
  else if (fileCheck(inputname) == 1) {
    node *n = calloc(1, sizeof(node));
    if (n == NULL) {
      printf("No memory for node\n");
      return 1;
    }
    strcpy(n->filename, inputname);
    n->next = NULL;
    head = n;
  }
  else {
    DIR *dir = opendir(inputname);
    if (dir == NULL) {
      printf("Not a directory\n");
      return 1;
    }
    struct dirent *file;
    while ((file = readdir(dir))) {
      if (fileCheck(file->d_name) != 1) {
        continue;
      }
      node *n = calloc(1, sizeof(node));
      if (n == NULL) {
        printf("No memory for node\n");
        return 1;
      }

      strcpy(n->filename, file->d_name);
      n->next = NULL;

      n->next = head;
      head = n;
    }
  }

  // Iterate over linked list and compile each file named in list
  for (node *iterator = head; iterator->next != NULL; iterator = iterator->next){
    FILE *filepntr = fopen(inputname, "r");
    if (filepntr == NULL) {
      printf("Can't open input file");
      return 1;
    }
  }





  char *xmlfile = nameFile(argv[1], ".xml");
  FILE *filewrtr = fopen(xmlfile, "w");
  if (filewrtr == NULL) {
    printf("Could not open .xml file");
    return 1;
  }

  // Buffer will store 1 line of text from the input file to
  // facilitate parsing and token will hold the current token 
  char *buffer = calloc(200, sizeof(char));
  char *token = calloc(200, sizeof(char));


  // Get first line of input file
  getNextLine(&index, buffer, filepntr);
  advance(&index, filepntr, buffer, token);

  // Iterate over file to extract tokens, determine classification, 
  // and print to xml file
  compileClass(&tab, &index, buffer, token, filewrtr, filepntr);

  // Close files and free memory
  fclose (filepntr);
  fclose (filewrtr);
  free(buffer);
  free(token);
}
