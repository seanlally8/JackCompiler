// SymbolTable.h 
#ifndef SYMBOLTABLE_H // check if myheader_h is not defined
#define SYMBOLTABLE_H

typedef struct node {
  char *name;
  char *type;
  char *kind;
  int index;
  struct node *next;
} node;

// all your header file code (declarations, etc.) goes here
node **initTable(void);
void reset(node **hashTable, int *index);

#endif // ends the #ifndef block
