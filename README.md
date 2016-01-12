# PageReplacementAlgorithms
COEN283 - Operating Systems Programming Assignment-1
Implementation of LRU and Optimal Algorithms using C++
The input consists of 
1) Word Size, 
2) Page Size ,
3) Physical Memory Size and
4) List of read/write operation specifying the number of bytes to be read and the starting address

The Page Table is implemented using C++ maps.
In LRU implementation a software counter is used to keep track of how often the page is referenced.
each page is associated with the counter initially set to zero.The counter is shifted right one bit before adding the R bit.
R bit is set to 1 if the page is referenced at the clock interrupt else set to 0 and is added to leftmost rather than rightmost bit.

When more than one page can be replaced at same time,page which is clean(dirty bit = 0) is replaced.
If all are clean or all dirty, page is chosen random




