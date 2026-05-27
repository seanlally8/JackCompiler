#include <stdlib.h>
#include <stdio.h>
#include "SymbolTable.h"
#define HASH_TABLE_SIZE 211

int main(void) {
  node **classTable = initTable();
  node **subroutineTable = initTable();

  char *name = "counter";
  char *type = "int";
  char *kind = "STATIC";
  char *name2 = "counter2";
  char *type2 = "char";
  char *kind2 = "STATIC";

  define(name, type, kind, classTable);
  define(name2, type2, kind2, classTable);

  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (classTable[i] != NULL) {
      for (node *ptr = classTable[i]; ptr != NULL; ptr = ptr->next) {
        printf("%s     %s     %s     %i\n", ptr->name, ptr->type, ptr->kind, ptr->index);
      }
    }
  }

  int indexof = indexOf(name2, classTable);
  printf("%i\n", indexof);

  reset(classTable);
  reset(subroutineTable);

  


  free(classTable);
  free(subroutineTable);

}

