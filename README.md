# JackCompiler
This is a compiler written for a 'toy language' called Jack, developed for the [nand2tetris course](https://www.nand2tetris.org/). 
I followed the APIs provided by The Elements of Computing Systems, written by Noam Nisan and Shimon Shocken.

It has successfully passed the unit tests given in the book, but I have not done any further optimization.
The only glaring issue to be fixed -- that I can see anyway -- is the presence of code duplication in at least
two areas: the compileDo function and the compileTerm function contain partially duplicated logic.
