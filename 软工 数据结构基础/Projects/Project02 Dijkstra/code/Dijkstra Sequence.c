#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Vertex int
#define MAX 1111
#define INF 100000000

int Nv, Ne;	 			// record size of graph 
Vertex graph[MAX][MAX]; // store weight of edges
Vertex Seq[MAX]; 		// store given path

int Check( Vertex start ); // check dijkstra sequence

/*---- Part 1: Main Function ----*/
/* Get input and Output results  */
int main() {
	/*
	------ Part 1.1: Build up Graph -------
	 */	
	// initialize and read in size of graph
	scanf("%d %d", &Nv, &Ne);
	int i, j;
	for ( i = 0; i <= Nv; i++ ) {
		for ( j = 0; j <= Nv; j++ ) {
				graph[i][j] = INF;
		}
	}
	// connect vertex and build up graph
	for ( i = 0; i < Ne; i++ ) {
		Vertex v1, v2;
		int weight;
		scanf("%d %d %d", &v1, &v2, &weight);
		// store weight in
		// Note: it's an undirected graph
		graph[v1][v2] = weight;
		graph[v2][v1] = weight;
	}

	/*
	------ Part 1.2: Dijkstra Sequence Check -------
	 */
	// read in number of lines
	int k;
	scanf("%d", &k);
	// check for each queues
	for ( i = 0; i < k; i++ ) {
		int j;
		// read in given sequence
		for ( j = 0; j < Nv; j++ ) {
			scanf("%d", &Seq[j]);
		}
		// check if it's a dijkstra sequence and print result
		if ( Check(Seq[0]) ) 
			printf("Yes\n");
		else 
			printf ("No\n");
	}

	return 0;	
}


/*---- Part 2: Check Function ----*/
/* Check whether is Dijkstra Path */
int Check(Vertex start) {
	// initialization
	Vertex s = start;
	int i, j, dist[MAX], flag[MAX];
	for( i = 0; i <= Nv; i++ ) {
		dist[i] = INF;
		flag[i] = 0;	// unvisited
	}
	dist[s] = 0;
	// visit vertex in given sequence
	for ( i = 0; i < Nv; i++ ) {
		int min = INF;		// get current min distance
		int cv = -1; 		// get current vertex
		// if there's shorter path, then update min distance
		for ( j = 1; j <= Nv; j++ ) {
			if ( !flag[j] && dist[j] < min ) {
				cv = j;
				min = dist[j];
			}
		}
		// record and compare
			// unconnected 
			// current vertex's distance not equal to current min distance 
		if ( cv == -1 || dist[cv] != dist[Seq[i]] )
			return 0;
		flag[cv] = 1; // mark as visited
		// update distance
		for ( j = 1; j <= Nv; j++ ) {
			if ( !flag[j] && graph[cv][j] != INF ) 		// unvisited and connected
				if( dist[cv] + graph[cv][j] < dist[j] ) // shorter than previous distance
					dist[j] = dist[cv] + graph[cv][j];  // update
		}
	}

	return 1;
}
