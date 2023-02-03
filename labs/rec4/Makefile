CC=gcc --std=c99 -g

all: anagram time_dynarray_insert

time_dynarray_insert: time_dynarray_insert.c dynarray.o
	$(CC) time_dynarray_insert.c dynarray.o -o time_dynarray_insert

dynarray.o: dynarray.c dynarray.h
	$(CC) -c dynarray.c

anagram: anagram.c
	$(CC) anagram.c -o anagram

clean:
	rm -f *.o anagram time_dynarray_insert
