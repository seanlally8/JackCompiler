#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"
#define HASH_TABLE_SIZE 211

node **initTable(void) {
  node **hashTable = calloc(HASH_TABLE_SIZE, sizeof(node*));
  return hashTable;
}

void reset(node **hashTable, int *index) {
  *index = 0;
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (hashTable[i] != NULL) {
      node *ptr = hashTable[i];
      while (ptr != NULL) {
        node *next = ptr->next;
        free(ptr);
        ptr = next;
      }
      hashTable[i] = NULL;
    }
  }
}
