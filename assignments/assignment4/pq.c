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
	//need one dynamic array which we can use to represent a heap
	struct dynarray* da;
};

//this allows me to push a node into the dynamic array which will
//hold the data and priority.
struct pq_node{
	int priority;
	void* data;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	//create pq pointer
	struct pq* pq = malloc(sizeof(struct pq));
	
	//intialize it's dynamic array
	pq->da = dynarray_create();

	//return pq pointer
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
	//start at 0
	int i = 0;

	//while the array isn't empty free each node in the priority queue
	while(!pq_isempty(pq)){
		free(dynarray_get(pq->da, i));
		i++;
	}

	//free dynarray
	dynarray_free(pq->da);
	pq->da = NULL;

	//free priority queue
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
	//if dynarray size is 0 then return 1 otherwise return 0
	return !dynarray_size(pq->da);
}

/*
 * This function takes an index and percolates it up a binary min heap until 
 * the parent's priority is less than the inserted node. makeing the conditions 
 * for the heap true. This is really helpful in inserting
 *
 * Params:
 *   da - the dynarray that is used in the pq to represent binary min heap.  May not be NULL.
 * 	 i - the index of the node we want to percolate up in the tree. Usually the newly inserted 
 * 		node.
 */
void percolate_up(struct dynarray* da, int i){
	//get the index for the parent of the given index
	int parent = (i - 1) / 2;

	//while the parent isn't higher than the root and the priority of the parent is greater than the priority of the current index
	//the continue to swap the parent node and the current node thus percolating the node upwards.
	while(parent >= 0 && (((struct pq_node*)dynarray_get(da, parent))->priority > ((struct pq_node*)dynarray_get(da, i))->priority)){
		//get the node at the parent index and current index
		void* parentN = dynarray_get(da, parent);
		void* iN = dynarray_get(da, i);

		//swap the values of the nodes
		dynarray_set(da, i, parentN);
		dynarray_set(da, parent, iN);

		//update the current index to the parent and then find the parent of the 
		//new current index and set that to parent for the next iteration of the loop.
		i = parent;
		parent = (i - 1) / 2;
	}
}

/*
 * This is a helper function to create a new pq_node to insert into my
 * dynarray within my pq.
 *
 * Params:
 *   priority - this the priority of the new ndoe being created.
 * 	 val - this the value or data of the new node being inserted.
 * 
 * Return:
 * 	 This function should return a pointer to a newly created pq_node. 
 */
struct pq_node* createNode(int priority, void* val){
	//create new node and initialize values
	struct pq_node* newnode = malloc(sizeof(struct pq_node));
	newnode->data = val;
	newnode->priority = priority;

	//return newly created node
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
	//insert a new node at last index with the passed priority and value
	dynarray_insert(pq->da, createNode(priority, value));

	//percolate the newly inserted node up the tree until the conditions
	//of the tree are met again. Starting at last index becuase thats 
	//where the new node will be.
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
	//return the data from the first index in the array
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
 *   	with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	//return the priority value from the first index in the array
	return ((struct pq_node*)dynarray_get(pq->da, 0))->priority;
}

/*
 * This function gets the index of the child of the passed i index with the 
 * minimum priority.
 *
 * Params:
 *   da - the dynarray to be checked.  May not be NULL.
 * 	 i - the index that we want to find the minimum child for.
 * 
 * Returns:
 * 	 This function returns the index to the child with the minimum priority
 * 		or if the nodes are out of bounds it returns -1.
 */
int min_child(struct dynarray* da, int i){
	//if the right child is out of bounds of array then return -1
	if(((i * 2) + 2) > dynarray_size(da) - 1){
		return -1;
	}

	//check to see which node has lower priority
	if(((struct pq_node*)dynarray_get(da, (i * 2) + 1))->priority > ((struct pq_node*)dynarray_get(da, (i * 2) + 2))->priority){
		//return right childs index
		return (i * 2) + 2;
	}else{
		//return left childs index
		return (i * 2) + 1;
	}
}

/*
 * This function will take and index from a dynarray which represents a 
 * binary min heap in order to percolate the given index down the tree 
 * until the conditions of a binary heap are met again. This is usually 
 * helpful in removing from a binary heap.
 *
 * Params:
 *   da - the dynarray which represents a binary heap.  May not be NULL.
 * 	 i - the index that we want percolate down from.
 */
void percolate_down(struct dynarray* da, int i){
	//loop until we break
	while(1){
		//get the index for the minimum child of the current index
		int min = min_child(da, i);

		//if the index is out of bounds then break
		if(min == -1) break;

		//get the nodes at the current index and minimum child's index
		struct pq_node* minN = dynarray_get(da, min);
		struct pq_node* iN = dynarray_get(da, i);

		//if the priority of the minimum child is less than the root's priority 
		//then swap nodes and loop again otherwise break
		if(minN->priority < iN->priority){
			//swap nodes
			dynarray_set(da, min, iN);
			dynarray_set(da, i, minN);

			//set i to current min so that next iteration we can find the next
			//min child one depth down
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
	//get the data from the first element in the pq
	void* first = pq_first(pq);

	//free first node
	free(dynarray_get(pq->da, 0));

	//replace first node with data in last inserted node
	dynarray_set(pq->da, 0, dynarray_get(pq->da, dynarray_size(pq->da) - 1));

	//remove last node's data so we don't have duplicates
	dynarray_remove(pq->da, dynarray_size(pq->da) - 1);

	//percolate down starting at the root where we just updated the node 
	//until the conditions of the binary heap are met again.
	percolate_down(pq->da, 0);

	//return the first data
	return first;
}