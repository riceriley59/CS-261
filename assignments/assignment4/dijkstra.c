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

/* 	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			printf("%d  ", adjacency[i][j]);
		}
		printf("\n");
	} */

	

	fclose(file);

	 
  return 0;
}
