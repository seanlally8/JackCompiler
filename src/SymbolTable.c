#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SymbolTable.h"

tableNode *initTable(tableNode *firstTable) {
  tableNode *n = calloc(1, sizeof(tableNode));
  if (n == NULL) {
    printf("calloc failed to return Table Node\n");
    return NULL;
  }
  n->nextTable = NULL;
  if (firstTable == NULL) {
    firstTable = n;
    firstTable->tableType = "Class";
  }
  else {
    firstTable->nextTable = n;
  }
  return firstTable;
}

void reset(tableNode *firstTable) {
  tableNode *tableSelector = selectTable(firstTable);

  free(tableSelector->tableType);
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (tableSelector->hashTable[i] != NULL) {
      node *ptr = tableSelector->hashTable[i];
      while (ptr != NULL) {
        node *next = ptr->next;
        free(ptr->name);
        free(ptr->kind);
        free(ptr->type);
        free(ptr);
        ptr = next;
      }
      tableSelector->hashTable[i] = NULL;
    }
  }
}

void define(char *name, char *type, char *kind, tableNode *firstTable) {
  tableNode *tableSelector = selectTable(firstTable);
  node *new = calloc(1, sizeof(node));
  if (new == NULL) {
    printf("calloc failed to return 'new' pointer\n");
    return;
  }
  new->name = calloc(1, strlen(name) + 1);
  new->kind = calloc(1, strlen(kind) + 1);
  new->type = calloc(1, strlen(type) +1);
  int hash_num = hash((unsigned char *)name) % HASH_TABLE_SIZE;

  // Prepend node (add entry to symbol table)
  strcpy(new->name, name);
  strcpy(new->kind, kind);
  strcpy(new->type, type);
  new->index = varCount(kind, firstTable);
  new->next = tableSelector->hashTable[hash_num];
  tableSelector->hashTable[hash_num] = new;
}

int varCount(char *kind, tableNode *firstTable) {
  int varcounter = 0;
  tableNode *ptr = firstTable;
  while (ptr != NULL) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
      if (ptr->hashTable[i] != NULL) {
        for (node *nodeptr = ptr->hashTable[i]; nodeptr != NULL; nodeptr=nodeptr->next) {
          if (strcmp(nodeptr->kind, kind) == 0) {
            varcounter++;
          }
        }
      }
    }
    ptr = ptr->nextTable;
  }
  return varcounter;
}

char *kindOf(char *name, tableNode *firstTable) {
  int hash_num = hash((unsigned char*)name) % 211;
  tableNode *tableptr = firstTable;
  while (tableptr != NULL) {
    for (node *ptr = tableptr->hashTable[hash_num]; ptr != NULL; ptr = ptr->next) {
      if (strcmp(ptr->name, name) == 0) {
        return ptr->kind;
      }
    }
    tableptr = tableptr->nextTable;
  }
  return "NONE";
}

char *typeOf(char *name, tableNode *firstTable) {
  int hash_num = hash((unsigned char*)name) % 211;
  tableNode *tableptr = firstTable;
  while (tableptr != NULL) {
      for (node *ptr = tableptr->hashTable[hash_num]; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->name, name) == 0) {
          return ptr->type;
        }
      }
    tableptr = tableptr->nextTable;
  }
  return "NONE";
}

int indexOf(char *name, tableNode *firstTable) {
  int hash_num = hash((unsigned char*)name) % 211;
  tableNode *tableptr = firstTable;
  while (tableptr != NULL) {
      for (node *ptr = tableptr->hashTable[hash_num]; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->name, name) == 0) {
          return ptr->index;
        }
      }
    tableptr = tableptr->nextTable;
  }
  return -1;
}

void printTable(tableNode *firstTable) {
  tableNode *tableSelector = selectTable(firstTable);
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    if (tableSelector->hashTable[i] != NULL) {
      for (node *ptr = tableSelector->hashTable[i]; ptr != NULL; ptr = ptr->next) {
        printf("%s   %s   %s   %i\n", ptr->name, ptr->type, ptr->kind, ptr->index);
      }
    }
  }
}

tableNode *selectTable(tableNode *firstTable) {
  tableNode *tableSelector = firstTable;
  while (tableSelector->nextTable != NULL) {
    tableSelector = tableSelector->nextTable;
  }
  return tableSelector;
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
