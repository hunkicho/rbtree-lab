#!/bin/bash

# complie
sed -i '2s%<rbtree.h>%"../src/rbtree.h"%' test/test-rbtree.c
gcc -Wall -g -fsanitize=address -DSENTINEL src/rbtree.c test/test-rbtree.c
sed -i '2s%"../src/rbtree.h"%<rbtree.h>%' test/test-rbtree.c

# run
./a.out

# check memory leak
valgrind -s --leak-check=full ./a.out
     
