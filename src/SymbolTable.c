#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SymbolTable.h"

node **initTable(void) {
  node **hashTable = calloc(HASH_TABLE_SIZE, sizeof(node*));
  if (hashTable == NULL) {
    printf("calloc failed to return hashTable\n");
    return NULL;
  }
  return hashTable;
}

void reset(node **hashTable) {
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
  if (new == NULL) {
    printf("calloc failed to return 'new' pointer\n");
    return;
  }
  int hash_num = hash((unsigned char *)name) % 211;

  // Prepend node (add entry to symbol table)
  new->name = name;
  new->kind = kind;
  new->type = type;
  new->index = varCount(kind, hashTable);
  new->next = hashTable[hash_num];
  hashTable[hash_num] = new;
}

int varCount(char *kind, node **hashTable) {
  int varcounter = 0;
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (hashTable[i] != NULL) {
      for (node *ptr = hashTable[i]; ptr != NULL; ptr=ptr->next) {
        if (ptr->kind == kind) {
          varcounter++;
        }
      }
    }
  }
  return varcounter;
}

char *kindOf(char *name, node **hashTable) {
  int hash_num = hash((unsigned char*)name) % 211;
  if (hashTable[hash_num] == NULL) {
    return "NONE";
  }
  else {
    for (node *ptr = hashTable[hash_num]; ptr != NULL; ptr = ptr->next) {
      if (strcmp(ptr->name, name) == 0) {
        return ptr->kind;
      }
    }
  }
  return "NONE";
}

char *typeOf(char *name, node **hashTable) {
  int hash_num = hash((unsigned char*)name) % 211;
  if (hashTable[hash_num] == NULL) {
    return "NONE";
  }
  else {
    for (node *ptr = hashTable[hash_num]; ptr != NULL; ptr = ptr->next) {
      if (strcmp(ptr->name, name) == 0) {
        return ptr->type;
      }
    }
  }
  return "NONE";
}

int indexOf(char *name, node **hashTable) {
  int hash_num = hash((unsigned char*)name) % 211;
  if (hashTable[hash_num] == NULL) {
    return -1;
  }
  else {
    for (node *ptr = hashTable[hash_num]; ptr != NULL; ptr = ptr->next) {
      if (strcmp(ptr->name, name) == 0) {
        return ptr->index;
      }
    }
  }
  return -1;
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
