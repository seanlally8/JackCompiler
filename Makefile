JackAnalyzer: src/helper.c src/JackTokenizer.c src/JackAnalyzer.c src/CompilationEngine.c
	gcc -std=c99 -Wall -Wextra -g -Wpedantic -Wstrict-prototypes -Iinclude src/JackTokenizer.c src/JackAnalyzer.c src/helper.c src/CompilationEngine.c -o build/JackAnalyzer
