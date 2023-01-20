CC=gcc --std=c99 -g

all: dynarray test_list

dynarray: dynarray.c 
	$(CC) dynarray.c -o dynarray

test_list: ll.o ll.h test_list.c
	$(CC) test_list.c ll.o -o test_list

ll.o: ll.h ll.c
	$(CC) -c ll.c 

clean: 
	rm -f test_list dynarray *.o