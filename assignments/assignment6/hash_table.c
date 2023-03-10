/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"

#define __TS__ 9999


/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */
struct ht{
    struct dynarray* da;
};

struct ht_node{
    void* key;
    void* value;
};

/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* ht = malloc(sizeof(struct ht));

    ht->da = dynarray_create();

    for(int i = 0; i < dynarray_capacity(ht->da); i++){
        dynarray_set(ht->da, i, NULL);
    }

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
    for(int i = 0; i < dynarray_capacity(ht->da); i++){
        struct ht_node* curr = dynarray_get(ht->da, i);

        free(curr);
        curr = NULL;
    }

    dynarray_free(ht->da);
    ht->da = NULL;

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
    return !ht_size(ht);
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 */
int ht_size(struct ht* ht){
    int size = 0;

    for(int i = 0; i < dynarray_capacity(ht->da); i++){
        if(dynarray_get(ht->da, i) != NULL && dynarray_get(ht->da, i) != (void*)__TS__) size++;
    }

    return size;
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
    int keyint = convert(key);
    
    int c2 = 0x27d4eb2d; // a prime or an odd constant

    keyint = (keyint ^ 61) ^ (keyint >> 16);
    keyint = keyint + (keyint << 3);
    keyint = keyint ^ (keyint >> 4);
    keyint = keyint * c2;
    keyint = keyint ^ (keyint >> 15);
    
    return keyint % dynarray_capacity(ht->da);
}

int load_factor(struct ht* ht){
    return ht_size(ht) / dynarray_capacity(ht->da);
}

void rehash(struct ht* ht, int (*convert)(void*)){
    void** old_data = get_data(ht->da);
    int elements = dynarray_capacity(ht->da);
    
    _dynarray_resize(ht->da, dynarray_capacity(ht->da) * 2);

    for(int i = 0; i < dynarray_capacity(ht->da); i++){
        dynarray_set(ht->da, i, NULL);
    }

    for(int i = 0; i < elements; i++){
        struct ht_node* curr = old_data[i];

        if(curr == NULL || curr == (void*)__TS__) continue;

        int index = ht_hash_func(ht, curr->key, convert);

        dynarray_set(ht->da, index, (void*)curr);
    }

    free(old_data);
    old_data = NULL;
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
    int index = ht_hash_func(ht, key, convert);

    struct ht_node* newNode = malloc(sizeof(struct ht_node));
    newNode->key = key;
    newNode->value = value;

    while(dynarray_get(ht->da, index) != NULL && dynarray_get(ht->da, index) != (void*)__TS__){
        index = (index + 1)  % dynarray_capacity(ht->da);   
    }
    
    dynarray_set(ht->da, index, (void*)newNode);
    
    if(load_factor(ht) >= 0.75){
        rehash(ht, convert);
    }

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

    while(dynarray_get(ht->da, index) != NULL){
        if(((struct ht_node*)dynarray_get(ht->da, index))->key == key) return ((struct ht_node*)dynarray_get(ht->da, index))->value;

        index = (index + 1) % dynarray_capacity(ht->da);
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

    int i = index;

    while(((struct ht_node*)dynarray_get(ht->da, i))->key != key) {
        i = (i + 1) % dynarray_capacity(ht->da);

        if(i == index) return;
    }

    void* old_data = dynarray_get(ht->da, i);
    dynarray_set(ht->da, i, (void*)__TS__);

    free(old_data);
    old_data = NULL;

    return;
} 