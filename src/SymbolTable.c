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

void define(char *name, char *type, char *kind, node **hashTable) {
  node *new = calloc(1, sizeof(node));
  if ()
}

//http://www.cse.yorku.ca/~oz/hash.html -- the djb2 hash function developed by Dan Bernstein
unsigned long hash(unsigned char *str){
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}
