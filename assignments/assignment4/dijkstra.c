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
#define INF 999

struct dijNode{
	int current;
	int prev;
};

void dijkstra(int start, int adj[][10]){
	int cost[10], prev[10];
	struct pq* pq = pq_create();

	for(int i = 0; i < 10; i++){
		cost[i] = INF;
		prev[i] = -1;
	}

	struct dijNode* first = malloc(sizeof(struct dijNode));
	first->current = start;
	first->prev = prev[start];

	pq_insert(pq, (void*)first, 0);

	while(!pq_isempty(pq)){
		int c = pq_first_priority(pq);

		struct dijNode* popped = pq_remove_first(pq);

		if(c < cost[popped->current]){
			cost[popped->current] = c;
			prev[popped->current] = popped->prev;

			for(int i = 0; i < 10; i++){
				if(adj[popped->current][i] > 0){
					int neighborCost = adj[popped->current][i];

					struct dijNode* new = malloc(sizeof(struct dijNode));
					new->current = i;
					new->prev = popped->current;

					pq_insert(pq, (void*)new, (neighborCost + c));
				}
			}
		}

		free(popped);
	}

	for(int i = 0; i < 10; i++){
		printf("The cost for each node: %d\n", cost[i]);
	}

	pq_free(pq);
}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, "%d %d", &n_nodes, &n_edges);

	int adjacency[10][10];

	for(int i = 0; i < n_nodes; i++){
		for(int j = 0; j < n_nodes; j++){
			adjacency[i][j] = 0;
		}
	}

	for(int i = 0; i < n_edges; i++){
		int source, dest, weight;

		fscanf(file, "%d %d %d", &source, &dest, &weight);
		adjacency[source][dest] = weight;
	}

	printf("Adjacency Matrix: \n");

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			printf("%d ", adjacency[i][j]);
		}
		printf("\n");
	}

	printf("\n\n");

	dijkstra(START_NODE, adjacency);

	fclose(file);
 
 	return 0;
}