#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "JackTokenizer.h"
#include "helper.h"
#include "CompilationEngine.h"

int main(int argc, char *argv[]) {

  // Check for two arguments in cli
  if (argc != 2) {
    printf("No file or directory name given");
    return 1;
  }

  // Meaningful var name for program input
  char *inputname = argv[1];

  // buffer holds current line from .jack file
  // token holds the current token
  // new_path is for writing the output files into an output directory
  char *buffer = calloc(200, sizeof(char));
  char *token = calloc(200, sizeof(char));
  char *new_path = calloc(200, sizeof(char));

  // Declare node to build linked list for multiple files in a directory
  typedef struct node {
    char filename[200];
    struct node *next;
  } node;

  // Pointer to first node in list
  node *head = NULL;
  //
  // If it's a file and not a jack file, exit program
  if (fileCheck(inputname) == -1) { 
    printf("Not a .jack file\n");
    return 1;
  }
 
  // If it is a jack file, create a linked list with 1 node
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

  // Otherwise, we can assume it's a directory and prepend each pathname to a linked list
  else { 
    DIR *dir = opendir(inputname);
    if (dir == NULL) {
      printf("Not a directory\n");
      return 1;
    }
    struct dirent *file;
    char *inputnamebuffer = calloc(200, sizeof(char));
    strcat(inputname, "/");
    while ((file = readdir(dir))) {
      if (fileCheck(file->d_name) != 1) {
        continue;
      }

      strcpy(inputnamebuffer, inputname);
      strcat(inputnamebuffer, file->d_name);

      node *n = calloc(1, sizeof(node));
      if (n == NULL) {
        printf("No memory for node\n");
        return 1;
      }

      strcpy(n->filename, inputnamebuffer);
      n->next = NULL;

      n->next = head;
      head = n;

      memset(inputnamebuffer, 0, 200);
    }
    free(inputnamebuffer);
    closedir(dir);
  }

  // Iterate over linked list and compile each file named in list
  for (node *iterator = head; iterator != NULL; iterator = iterator->next){

    // index tracks character in buffer from stream
    int index = 0;

    // No. of tabs for xml output
    int tab = 1;

    // Open first file in list to compile
    FILE *filepntr = fopen(iterator->filename, "r");
    if (filepntr == NULL) {
      printf("Can't open input file");
      return 1;
    }

    // Name output file (and place in 'output/') then open for writing
    char *xmlfile = nameOutputFile(iterator->filename, new_path);
    FILE *filewrtr = fopen(xmlfile, "w");
    if (filewrtr == NULL) {
      printf("Could not open .xml file");
      return 1;
    }

    // Get first line of current file and advance to first token
    getFirstLine(buffer, filepntr);
    advance(&index, filepntr, buffer, token);

    // Recursively compile the current file
    compileClass(&tab, &index, buffer, token, filewrtr, filepntr);

    // Close current files and zero out buffers for clean writes
    fclose (filepntr);
    fclose (filewrtr);
    zeroBuffers(token, buffer, new_path);
  }

  // Free memory (including nodes in linked list)
  free(buffer);
  free(token);
  free(new_path);

  node *ptr = head;

  while (ptr != NULL) {
    node *next = ptr->next;
    free(ptr);
    ptr = next;
  }

  return 0;
}
