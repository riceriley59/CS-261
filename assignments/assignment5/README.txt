Grader(I think):
    /$$$$$                 /$$          
   |__  $$                | $$          
      | $$ /$$   /$$  /$$$$$$$  /$$$$$$ 
      | $$| $$  | $$ /$$__  $$ /$$__  $$
 /$$  | $$| $$  | $$| $$  | $$| $$$$$$$$
| $$  | $$| $$  | $$| $$  | $$| $$_____/
|  $$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$
 \______/  \______/  \_______/ \_______/


Riley Rice
ONID: 934127342
3-13-2023

Description:

This program is a hash table that's implemented using a dynamic array for the table and open
addressing for hash collisions. A hash table is a data structure which is really useful as it
allows for constant runtime complexity for insertion, remove, and lookup. This is all done 
by calculating a metric called a load factor which allows us to know when the table is a little 
too full and we need to resize it. By keeping this load factor under a certain limit we can maintain
the fast runtime complexity of a hash table. It allows you to enter in a key through a hash function
and instantly find where your data is in the table. This hash table uses a signed 32 bit mix hash 
function which has good uniformity, speed, and distribution. Overall, if you want a way to store 
your data with really fast insertion, removal, and lookup then this is what your going to want to use
it's extremely useful in multiple use cases and is used all the time in the industry for data
storage, software engineering, and computer science.

Instructions:

1. I compile this using make, as it makes everything a lot easier especially when there are a lot of files.
   The main commands for compiling and running this program are:
        make - will compile all the code.
        make clean - will remove all the object files from compilation
        ./test_ht - This isthe test script for testing the hash table. Once you have
                    compiled your program with the make command you can run this command to 
                    make sure that it works.
2. when you run ./test_ht it will prompt a lot of different prompts with expected outputs as long
   as all the values match and you see OK for all the other test cases then that means that your
   hash table should be working properly.
3. If you want to make sure that the data structure doesn't have any memory leaks then you can run:
   valgrind ./test_ht and this will tell you whether there are memory leaks or not. 

Limitations:

The only limitations of this data structure and it's implementation is that the starting capacity of
the hash table when creating it is only 2 which means that your going to have to resize a few times in the
beginning which can make your runtime complexity worse. Although you can fix this pretty easity by changing
the starting capacity to some number that is a power of 2 or even (This is so we can maintain O(1) time
complexity for resizing our dynarray).