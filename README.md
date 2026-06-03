# JackCompiler
This is a compiler written for a 'toy language' called Jack, developed for the [nand2tetris course](https://www.nand2tetris.org/). 
I followed the APIs provided by The Elements of Computing Systems, written by Noam Nisan and Shimon Shocken.

The compiler has successfully passed the unit tests given in the book, but I have not done any further optimization or code cleaning.

Specifically: 

1. The compileDo function and the compileTerm function contain partially duplicated logic. 
2. Due to the explicit caveat given in the book -- that we, as students, should expect perfect user input -- there
is no error handling
3. The formatting needs some cleaning -- e.g. 8 space tabs, intentional white space to enhance clarity, updated names (filereadr instead of filepntr),
comments that offer clarity to potentially confusing code.
