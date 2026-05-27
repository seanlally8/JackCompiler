// SymbolTable.h 
#ifndef SYMBOLTABLE_H // check if myheader_h is not defined
#define SYMBOLTABLE_H

// Define node data type for Symbol Tables
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
void define(char *name, char *type, char *kind, node **hashTable);
unsigned long hash(unsigned char *str);

#endif // ends the #ifndef block
