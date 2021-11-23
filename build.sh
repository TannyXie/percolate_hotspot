rm -f *.o percolate
gcc -g -c arralloc.c uni.c percolate.c
gcc -g -o percolate arralloc.o uni.o percolate.o -lm
