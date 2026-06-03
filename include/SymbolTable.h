// SymbolTable.h 
#ifndef SYMBOLTABLE_H // check if myheader_h is not defined
#define SYMBOLTABLE_H
#define HASH_TABLE_SIZE 211

// Define node data type for Symbol Tables
typedef struct node {
  char *name;
  char *type;
  char *kind;
  int index;
  struct node *next;
} node;

// Define classNode datatype to hold a hashtable and a pointer to another hashtable
typedef struct tableNode {
  char *tableType;
  node *hashTable[HASH_TABLE_SIZE];
  struct tableNode *nextTable;
} tableNode;

// all your header file code (declarations, etc.) goes here
tableNode *initTable(tableNode *firstTable);
void reset(tableNode *firstTable);
void define(char *name, char *type, char *kind, tableNode *firstTable);
int varCount(char *kind, tableNode *firstTable);
char *kindOf(char *name, tableNode *firstTable);
char *typeOf(char *name, tableNode *firstTable);
int indexOf(char *name, tableNode *firstTable);
void printTable(tableNode *firstTable);
tableNode *selectTable(tableNode*firstTable);
unsigned long hash(unsigned char *str);

#endif // ends the #ifndef block
