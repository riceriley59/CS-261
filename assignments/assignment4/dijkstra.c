/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Riley Rice
 * Email: riceri@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>

#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0
#define INF 9999

/*
 * This is the struct which gets pushed into the queue 
 * and holds the current and previous node.
 */
struct dijNode{
	int current;
	int prev;
}; 
 
/*
 * This function implements dijkstra's algorithm in order to find the shortest
 * path from the start node to all other nodes in the graph. The cost of each 
 * node is stored into the corresponding index in the cost array, and the path
 * can be found by repeatedly going to the prev node of the corresponding index
 * in the prev array until your at the start.
 *
 * Params:
 *   start - this is the start index or the node that you want to find all the shortest
 * 		paths to.
 * 	 adj - This is the adjacency matrix for the graph that you are trying to represent
 * 		it must be initialized.
 *   size - This is the amount of nodes in the graph.
 *   cost - This is the array which will hold the cost of each node to the start node,
 * 		you will find the node's cost with the corresponding index of the cost array
 * 	 prev - This is the node that holds all the previous nodes before it, this allows
 * 		you to retrace the shortest path for each node. 
 */
void dijkstra (int start, int adj[][10], int size, int cost[], int prev[]){
	//create new priority queue
	struct pq* pq = pq_create();

	//set all of the cost to infinity and prev to undefined
	for(int i = 0; i < size; i++){
		cost[i] = INF;
		prev[i] = -1;
	}

	//create the first node to push in which will push in the start node
	struct dijNode* first = malloc(sizeof(struct dijNode));
	first->current = start;
	first->prev = prev[start];

	//insert start node with priority of 0
	pq_insert(pq, (void*)first, 0);

	//while the priority queue isn't empty keep on popping and checking nodes
	while(!pq_isempty(pq)){
		//get the priority of next node
		int c = pq_first_priority(pq);

		//pop node out of queue
		struct dijNode* popped = pq_remove_first(pq);

		//if the cost of the node we just popped is lower than the current shortest
		//path for that node then update queue, otherwise continue
		if(c < cost[popped->current]){
			//update the cost and prev at the index
			cost[popped->current] = c;
			prev[popped->current] = popped->prev;

			//iterate through all the node's neighbors
			for(int i = 0; i < size; i++){
				//if there is a neighbor to another node then add it into queue
				if(adj[popped->current][i] > 0){
					//get the cost of that neighbor from the adjacency matrix
					int neighborCost = adj[popped->current][i];

					//create a new node with the current being the neighbor
					//and the previous being the popped node.
					struct dijNode* new = malloc(sizeof(struct dijNode));
					new->current = i;
					new->prev = popped->current;

					//insert into queue with a priority of the original cost of this
					//node plus the cost to get to the neighbor as this is a path,
					// and we need to add up.
					pq_insert(pq, (void*)new, (neighborCost + c));
				}
			}
		}

		//free all the nodes that come out of the queue so there are no memory leaks
		free(popped);
	}

	//free priority queue
	pq_free(pq);
}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, "%d %d", &n_nodes, &n_edges);

	//create an adjacency matrix the size of number of nodes
	int adjacency[n_nodes][n_nodes];

	//put 0s in every index
	for(int i = 0; i < n_nodes; i++){
		for(int j = 0; j < n_nodes; j++){
			adjacency[i][j] = 0;
		}
	}

	//read information from file and load that into adjacency
	//matrix in the corresponding indices
	for(int i = 0; i < n_edges; i++){
		int source, dest, weight;

		fscanf(file, "%d %d %d", &source, &dest, &weight);
		adjacency[source][dest] = weight;
	}

	//formatting
	printf("Adjacency Matrix: \n");
	printf("------------------------------------------------------\n");

	//print out adjacency matrix with aligned columns
	for(int i = 0; i < n_nodes; i++){
		printf("| ");
		for(int j = 0; j < n_nodes; j++){
			printf("%d ", adjacency[i][j]);

			if(adjacency[i][j] < 10){
				printf("   ");
			}else if(adjacency[i][j] < 100){
				printf("  ");
			}else{
				printf(" ");
			}
		}
		printf(" |\n");
	}

	printf("------------------------------------------------------\n\n\n");

	//formatting
	printf("Paths from PDX to other locations:\n");
	printf("-----------------------------------------------------\n");

	//create cost and prev arrays the size of the amount of nodes 
	//to pass to dijkstra function
	int cost[n_nodes], prev[n_nodes];
	
	//call dijkstra with corresponding parameters
	dijkstra(START_NODE, adjacency, n_nodes, cost, prev);

	//print out the results from the dijkstra function call in meaningful way
	//prints out start and destination, cost, and the path to get there.
	for(int i = 0; i < n_nodes; i++){
		if(i != START_NODE){
			printf("The path from %d to %d costs %d.", START_NODE, i, cost[i]);
			
			int k = i;

			printf("\tThe path is: ");

			while(k != START_NODE){
				printf("%d<-", k);

				k = prev[k];
			}

			printf("%d.", START_NODE);
			printf("\n");
		}
	}

	//close file stream
	fclose(file);
 
 	return 0;
}