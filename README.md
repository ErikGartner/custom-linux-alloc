# Heap Managers - OS project

## Running
1. `cd ./tests/gawk-3.1.8` and `./configure`.
2. `cd ../../` to root folder and run one of `make linked_gawk` or `make buddy_gawk` or `make minitest` to test.

## Description
1. Your task is to implement two versions of malloc/calloc/realloc and free.
2. One should be a linked-list implementation and the other an implementation of the buddy system.
3. You must use sbrk to allocate memory from the kernel.
4. Your implementations should work with gawk.
5. Download gawk and type configure and make check.
6. Then edit the Makefile to use your malloc implementation. To edit the Makefile, look for places where version is used and add your file in the same way.
7. Then type make check again.
8. Repeat for your other malloc implementation.

### Linked list
This rather naïve implementation resides in [linked_alloc.c](./linked_alloc.c).

### Buddy allocator
The implementation resides in [buddy_alloc.c](./buddy_alloc.c).

### Useful links
* [A Memory Allocator by Doug Lea](http://g.oswego.edu/dl/html/malloc.html)
* [Implementing Malloc (iit.edu)](http://moss.cs.iit.edu/cs351/slides/slides-malloc.pdf)
* [Stackoverflow answer](http://stackoverflow.com/questions/5422061/malloc-implementation)
* [A quick tutorial on implementing and debugging malloc, free, calloc, and realloc by Danluu](http://danluu.com/malloc-tutorial/)

