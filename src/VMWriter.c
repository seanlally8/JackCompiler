#include <stdio.h>
#include <string.h>

void writePush(FILE *filewrtr, char *vmSegment, int *vmIndex) {
  if (strcmp(vmSegment, "field") == 0) {
    fprintf(filewrtr, "\tpush this %i\n", *vmIndex);
  }
  else {
    fprintf(filewrtr, "\tpush %s %i\n", vmSegment, *vmIndex);
  }
}

void writeFunction(FILE *filewrtr, char *className, char *subroutineName, int *nVars) {
  fprintf(filewrtr, "function %s.%s %i\n", className, subroutineName, *nVars);
}

void writeArithmetic(FILE *filewrtr, char *operator) {
  if (strcmp(operator, "+ ") == 0) {
    fputs("\tadd\n", filewrtr);
  }
  else if (strcmp(operator, "* ") == 0) {
    fputs("\tcall Math.multiply 2\n", filewrtr);
  }
  else if (strcmp(operator, "-") == 0) {
    fputs("\tneg\n", filewrtr);
  }
  else if (strcmp(operator, "- ") == 0) {
    fputs("\tsub\n", filewrtr);
  }
  else if (strcmp(operator, "~") == 0) {
    fputs("\tnot\n", filewrtr);
  }
  else if (strcmp(operator, "&gt; ") == 0) {
    fputs("\tgt\n", filewrtr);
  }
  else if (strcmp(operator, "= ") == 0) {
    fputs("\teq\n", filewrtr);
  }
  else if (strcmp(operator, "&amp; ") == 0) {
    fputs("\tand\n", filewrtr);
  }
}

void writeCall(FILE *filewrtr, char *className, char *subroutineName, int *nArgs) {
  fprintf(filewrtr, "\tcall %s.%s %i\n", className, subroutineName, *nArgs);
}

void writePop(FILE *filewrtr, char *vmSegment, int *vmIndex) {
  if (strcmp(vmSegment, "field") == 0) {
    fprintf(filewrtr, "\tpop this %i\n", *vmIndex);
  }
  else {
    fprintf(filewrtr, "\tpop %s %i\n", vmSegment, *vmIndex);
  }
}

void writeReturn(FILE *filewrtr) {
  fputs("\treturn\n", filewrtr);
}

void writeLabel(FILE *filewrtr, int labelNumber) {
  fprintf(filewrtr, "label L%i\n", labelNumber);
}

void writeIf(FILE *filewrtr, int labelNumber) {
  fprintf(filewrtr, "\tif-goto L%i\n", labelNumber);
}

void writeGoTo(FILE *filewrtr, int labelNumber) {
  fprintf(filewrtr, "\tgoto L%i\n", labelNumber);
}
