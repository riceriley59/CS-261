/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Riley Rice
 * Email: riceri@oregonstate.edu
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
	int i = 0;

	while(!pq_isempty(pq)){
		free(dynarray_get(pq->da, i));
		i++;
	}

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
	return !dynarray_size(pq->da);
}

void percolate_up(struct dynarray* da, int i){
	int parent = (i - 1) / 2;

	while(parent >= 0 && (((struct pq_node*)dynarray_get(da, parent))->priority > ((struct pq_node*)dynarray_get(da, i))->priority)){
		void* parentN = dynarray_get(da, parent);
		void* iN = dynarray_get(da, i);

		dynarray_set(da, i, parentN);
		dynarray_set(da, parent, iN);

		i = parent;
		parent = (i - 1) / 2;
	}
}

struct pq_node* createNode(int priority, void* val){
	struct pq_node* newnode = malloc(sizeof(struct pq_node));
	newnode->data = val;
	newnode->priority = priority;

	return newnode;
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
	dynarray_insert(pq->da, createNode(priority, value));
	percolate_up(pq->da, (dynarray_size(pq->da) - 1));
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
	return ((struct pq_node*)dynarray_get(pq->da, 0))->data;
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
	return ((struct pq_node*)dynarray_get(pq->da, 0))->priority;
}

int min_child(struct dynarray* da, int i){
	if(((i * 2) + 2) > dynarray_size(da) - 1){
		return -1;
	}

	if(((struct pq_node*)dynarray_get(da, (i * 2) + 1))->priority > ((struct pq_node*)dynarray_get(da, (i * 2) + 2))->priority){
		return (i * 2) + 2;
	}else{
		return (i * 2) + 1;
	}
}

void percolate_down(struct dynarray* da, int i){
	while(1){
		int min = min_child(da, i);

		if(min == -1) break;

		struct pq_node* minN = dynarray_get(da, min);
		struct pq_node* iN = dynarray_get(da, i);

		if(minN->priority < iN->priority){
			dynarray_set(da, min, iN);
			dynarray_set(da, i, minN);

			i = min;
		}else break;
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
	void* first = pq_first(pq);
	free(dynarray_get(pq->da, 0));

	dynarray_set(pq->da, 0, dynarray_get(pq->da, dynarray_size(pq->da) - 1));
	dynarray_remove(pq->da, dynarray_size(pq->da) - 1);

	percolate_down(pq->da, 0);

	return first;
}