/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* da;
};

struct pq_node{
	int priority;
	void* data;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq));
	
	pq->da = dynarray_create();

	return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	dynarray_free(pq->da);
	pq->da = NULL;

	free(pq);
	pq = NULL;

	return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if(dynarray_size(pq->da) == 0) return 1;
	else return 0;
}

/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	struct pq_node* newnode = malloc(sizeof(struct pq_node));
	newnode->data = value;
	newnode->priority = priority;

	dynarray_insert(pq->da, (void*)newnode);
	int i = dynarray_size(pq->da) - 1;

	while(i > 0){
		struct pq_node* root = dynarray_get(pq->da, i); 
		struct pq_node* parent = dynarray_get(pq->da, (i - 1) / 2);

		if(parent->priority > root->priority){
			dynarray_set(pq->da, i, parent);
			dynarray_set(pq->da, (i - 1) / 2, root);
		}else break;
		
		i--;
	}

	return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	struct pq_node* first = dynarray_get(pq->da, 0);

	return first->data;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	struct pq_node* first = dynarray_get(pq->da, 0);

	return first->priority;
}

struct pq_node* min(struct pq_node* one, struct pq_node* two){
	if(one->priority < two->priority){
		if(one == NULL){
			return two;
		}
		return one;
	}else{
		if(two == NULL){
			return one;
		}
		return two;
	}
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	if(pq_isempty(pq)) return NULL;

	struct pq_node* first = dynarray_get(pq->da, 0);

	dynarray_set(pq->da, 0, dynarray_get(pq->da, (dynarray_size(pq->da) - 1)));
	dynarray_remove(pq->da, (dynarray_size(pq->da) - 1));

	if(!pq_isempty(pq)){
		int i = 0;

		while(i < (dynarray_size(pq->da) - 1)){
			if((i * 2) + 2 < (dynarray_size(pq->da) - 1)){
				struct pq_node* left = dynarray_get(pq->da, (i * 2) + 1);
				struct pq_node* right = dynarray_get(pq->da, (i * 2) + 2);

				struct pq_node* current = dynarray_get(pq->da, i);

				if(current < min(left, right)){
					if(left->priority < right->priority){
						dynarray_set(pq->da, i, left);
						dynarray_set(pq->da, (i * 2) + 1, current);

						i = (i * 2) + 1;
					}else{
						dynarray_set(pq->da, i, right);
						dynarray_set(pq->da, (i * 2) + 2, current);

						i = (i * 2) + 2;
					}
				}else break;
			}else break;
		}
	}
	
	return first->data;
}
