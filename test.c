#include <stdlib.h>
#include <stdio.h>
#include "SymbolTable.h"
#define HASH_TABLE_SIZE 211

int main(void) {
  node **classTable = initTable();
  node **subroutineTable = initTable();

  int index = 0;     

  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    node *new = calloc(1, sizeof(node));
    new->kind = "FIELD";
    new->name = "varName";
    new->type = "char";
    new->index = index; 
    new->next = NULL;
    classTable[i] = new;
    index++;
  }

  index = 0;

  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    node *new = calloc(1, sizeof(node));
    new->kind = "FIELD";
    new->name = "varName";
    new->type = "char";
    new->index = index; 
    new->next = NULL;
    subroutineTable[i] = new;
    index++;
  }

  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    printf("%s %i\n", classTable[i]->name, classTable[i]->index);
    printf("%s %i\n", subroutineTable[i]->name, subroutineTable[i]->index);
  }

  reset(classTable, &index);
  reset(subroutineTable, &index);


  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (classTable[i] == NULL & subroutineTable[i] == NULL){
      printf("empty %i\n", ++index);
      continue;
    }
    printf("%s %i\n", classTable[i]->name, classTable[i]->index);
    printf("%s %i\n", subroutineTable[i]->name, subroutineTable[i]->index);
  }

  free(classTable);
  free(subroutineTable);

}

