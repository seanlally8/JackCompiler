JackAnalyzer: src/helper.c src/JackTokenizer.c src/JackAnalyzer.c 
	gcc -std=c99 -Wall -Wextra -g -Wpedantic -Wstrict-prototypes -Iinclude src/JackTokenizer.c src/JackAnalyzer.c src/helper.c -o build/JackAnalyzer
