// VMWriter.h 
#ifndef VMWRITER_H // check if myheader_h is not defined
#define VMWRITER_H

typedef struct labelNode {
  int label1;
  int label2;
  struct labelNode *next;
} labelNode;

// all your header file code (declarations, etc.) goes here
void writePush(FILE *filewrtr, char *vmSegment, int *vmIndex);
void writeFunction(FILE *filewrtr, char *className, char *subroutineName, int *nVars);
void writeArithmetic(FILE *filewrtr, char *operator);
void writeCall(FILE *filewrtr, char *className, char *subroutineName, int *nArgs);
void writePop(FILE *filewrtr, char *vmSegment, int *vmIndex);
void writeReturn(FILE *filewrtr);
void writeLabel(FILE *filewrtr, int labelNumber);
void writeIf(FILE *filewrtr, int labelNumber);
void writeGoTo(FILE *filewrtr, int labelNumber);

#endif // ends the #ifndef block
