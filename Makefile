JackCompiler: src/helper.c src/JackTokenizer.c src/JackCompiler.c src/CompilationEngine.c src/SymbolTable.c src/VMWriter.c
	gcc -std=c99 -Wall -Wextra -g -Wpedantic -Wstrict-prototypes -Iinclude src/JackTokenizer.c src/JackCompiler.c src/helper.c src/CompilationEngine.c src/SymbolTable.c src/VMWriter.c -o build/JackCompiler
