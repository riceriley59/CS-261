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
    ht->size = 0;
    ht->capacity = 2;

    for (int i = 0; i < ht->capacity; i++) {
        dynarray_set(ht->da, i, NULL);  // init all elements as NULL
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
    for (int i = 0; i < ht->capacity; i++) {
        struct ht_node* temp = dynarray_get(ht->da, i);

        if (temp != NULL && temp != (void*)__TS__ ) {
            free(temp);
            temp = NULL;
        }
    }

    dynarray_free(ht->da); // free dynamic array
    free(ht); // free ht struct
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
    return ht->size;
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
    
    keyint = ~keyint + (keyint << 15); // key = (key << 15) - key - 1;
    keyint = keyint ^ (keyint >> 12);
    keyint = keyint + (keyint << 2);
    keyint = keyint ^ (keyint >> 4);
    keyint = keyint * 2057; // key = (key + (key << 3)) + (key << 11);
    keyint = keyint ^ (keyint >> 16);
    
    return keyint % ht->capacity;
}

void ht_rehash(struct ht* ht, int (*convert) (void*)) {
    
    struct dynarray* new_da = dynarray_create();
    _dynarray_resize(new_da, ht->capacity * 2);

    ht->capacity *= 2;

    for (int i = 0; i < ht->capacity; i++) {
        dynarray_set(new_da, i, NULL);
    }

    for (int i = 0; i < ht->capacity / 2; i++) {
        struct ht_node* curr = dynarray_get(ht->da, i);

        if (curr != NULL && curr != (void*) __TS__ ) {
            int index = ht_hash_func(ht, curr->key, convert);

            struct ht_node* new_node = malloc(sizeof(struct ht_node));
            new_node->key = curr->key;
            new_node->val = curr->val;

            struct ht_node* rehashNode = dynarray_get(new_da, index);

            while (rehashNode != NULL) {
                index = (index + 1) % ht->capacity;
                rehashNode = dynarray_get(new_da, index);
            }

            dynarray_set(new_da, index, (void*)new_node);

            free(curr);
            curr = NULL;
        } 
    }

    dynarray_free(ht->da);
    ht->da = new_da;
}

float get_load_factor(struct ht* ht){
    return ht->size / ht->capacity;
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
    if (get_load_factor(ht) >= 0.75) {
        ht_rehash(ht, convert);
    }

    int index = ht_hash_func(ht, key, convert);

    struct ht_node* curr = dynarray_get(ht->da, index);

    while (curr != NULL && curr != (void*) __TS__ ) {
        if (convert(curr->key) == convert(key)) {
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

    while (curr != NULL) {
        if (curr != (void*)__TS__ && convert(curr->key) == convert(key)) {
            return curr->val;
        }

        index = (index + 1) % ht->capacity;

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

    while (curr != NULL) {
        if (curr != (void*)__TS__ && convert(curr->key) == convert(key)) {
            void* old_data = curr;
            dynarray_set(ht->da, index, (void*)__TS__);

            free(old_data);
            old_data = NULL;

            ht->size--;
            return;
        }

        index = (index + 1) % ht->capacity;

        curr = dynarray_get(ht->da, index);
    }

    return;
} 