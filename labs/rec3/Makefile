CC=gcc --std=c99 -g

all: buggy-list-sort buggy-pointers buggy-array-sort

buggy-list-sort: buggy-list-sort.c
	$(CC) buggy-list-sort.c -o buggy-list-sort

buggy-pointers: buggy-pointers.c
	$(CC) buggy-pointers.c -o buggy-pointers

buggy-array-sort: buggy-array-sort.c
	$(CC) buggy-array-sort.c -o buggy-array-sort

clean:
	rm -f *.o buggy-list-sort buggy-pointers buggy-array-sort
