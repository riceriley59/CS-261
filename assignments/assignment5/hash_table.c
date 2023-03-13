/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Riley Rice
 * Email: riceri@oregonstate.edu
 */

#include <stdlib.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"

//make constant value for our tombstone value
#define __TS__ 9999

/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */
struct ht {
    struct dynarray* da;
    int size;
    int capacity;
};

//this struct allows us to store key value pair
//in dynarray
struct ht_node {
    void* key;
    void* val;
};


/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* ht = malloc(sizeof(struct ht));
    
    ht->da = dynarray_create();

    //initialize values
    ht->size = 0;
    ht->capacity = 2;

    //set all values to NULL
    for(int i = 0; i < ht->capacity; i++) dynarray_set(ht->da, i, NULL);

    return ht;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    //go through the whole hash table
    for(int i = 0; i < ht->capacity; i++) {
        //get current value at index in hash table 
        void* temp = dynarray_get(ht->da, i);

        //if the current value isn't empty then free it and
        //set to null
        if(temp != NULL && temp != (void*)__TS__) {
            free(temp);
            temp = NULL;
        }
    }

    //free dynarray
    dynarray_free(ht->da);
    ht->da = NULL;

    //free hash table
    free(ht);
    ht = NULL;

    return;
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){
    return !ht_size(ht); //if size is 0 returns 1
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 */
int ht_size(struct ht* ht){
    return ht->size; //return our size stored in our hash table struct
}

/*
 * This function should return the load factor of the hash table by calculating 
 * the number of elements divided by the number of buckets.
 *
 * Params:
 *   ht - the hash table whose load factor is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return passed hash table's load factor.
 */
float load_factor(struct ht* ht){
    return ht->size / ht->capacity;
}

/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash algorithm is totally up to you. Make sure to consider
 * Determinism, Uniformity, and Speed when design the hash algorithm
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element to be stored
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
    //convert void* key to integer
    int keyint = convert(key);
    
    //I found this signed 32 bit mix hash function online. I chose it
    //because it has good distribution, speed, and uniformity. It can be found at
    //https://gist.github.com/badboy/6267743
    keyint = ~keyint + (keyint << 15);
    keyint = keyint ^ (keyint >> 12);
    keyint = keyint + (keyint << 2);
    keyint = keyint ^ (keyint >> 4);
    keyint = keyint * 2057;
    keyint = keyint ^ (keyint >> 16);
    
    //return our hashed value moduloed with the hash table's capacity
    //so that the value stays in range of our hash table.
    return keyint % ht->capacity;
}

/*
 * This function is called whenever the load factor of our hash table is above
 * 0.75, or 3/4 of the table is full. This resizes the array to twice the old amount
 * and then rehashes all the elements into the new array. Doing this allows us to maintain
 * an average runtime complexity of O(1) for lookup, insert, and remove in our hash table.
 *
 * Params:
 *   ht - the hash table into which to resize and rehash.  May not be NULL.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode.
 */
void ht_rehash(struct ht* ht, int (*convert) (void*)) {
    //create a new dynarray and resize it to twice the 
    //old amount
    struct dynarray* new_da = dynarray_create();
    _dynarray_resize(new_da, ht->capacity * 2);

    //change our hash table's capacity to twice it's old amount
    ht->capacity *= 2;

    //set all the values in the new array to NULL
    for(int i = 0; i < ht->capacity; i++) dynarray_set(new_da, i, NULL);

    //go through the old hash table's contents and rehash them into the 
    //new array
    for(int i = 0; i < ht->capacity / 2; i++) {
        //get node at index i
        struct ht_node* curr = dynarray_get(ht->da, i);

        //if the node isn't empty
        if(curr != NULL && curr != (void*)__TS__) {
            //calculate a new index
            int index = ht_hash_func(ht, curr->key, convert);

            //create a new node with the same values as our 
            //struct ht_node* newNode = malloc(sizeof(struct ht_node));
            //newNode->key = curr->key;
            //newNode->val = curr->val;

            struct ht_node* rehashNode = dynarray_get(new_da, index);

            while(rehashNode != NULL) {
                index = (index + 1) % ht->capacity;
                rehashNode = dynarray_get(new_da, index);
            }

            dynarray_set(new_da, index, (void*)curr);

            //free(curr);
            //curr = NULL;
        } 
    }

    dynarray_free(ht->da);
    ht->da = new_da;
}

/*
 * This function should insert a given element into a hash table with a
 * specified key.  Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){
    if(load_factor(ht) >= 0.75) ht_rehash(ht, convert);

    int index = ht_hash_func(ht, key, convert);

    struct ht_node* curr = dynarray_get(ht->da, index);

    while(curr != NULL && curr != (void*)__TS__) {
        if(convert(curr->key) == convert(key)) {
            curr->val = value; 
            return;
        }

        index = (index + 1) % ht->capacity;

        curr = dynarray_get(ht->da, index);
    }

    struct ht_node* newNode = malloc(sizeof(struct ht_node));
    newNode->key = key;
    newNode->val = value;

    dynarray_set(ht->da, index, (void*)newNode);
    ht->size++;

    return;
}


/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
    int index = ht_hash_func(ht, key, convert);

    struct ht_node* curr = dynarray_get(ht->da, index);

    int i = index;

    while(curr != NULL) {
        if(curr != (void*)__TS__ && convert(curr->key) == convert(key)) return curr->val;

        index = (index + 1) % ht->capacity;
	    if(i == index) break;

        curr = dynarray_get(ht->da, index);
    }

    return NULL;

}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){
    int index = ht_hash_func(ht, key, convert);
    struct ht_node* curr = dynarray_get(ht->da, index);

    int i = index;

    while(curr != NULL) {
        if(curr != (void*)__TS__ && convert(curr->key) == convert(key)) {
            dynarray_set(ht->da, index, (void*)__TS__);

            free(curr);
            curr = NULL;

            ht->size--;
            return;
        }

        index = (index + 1) % ht->capacity;
	    if(i == index) break;

        curr = dynarray_get(ht->da, index);
    }

    return;
} 