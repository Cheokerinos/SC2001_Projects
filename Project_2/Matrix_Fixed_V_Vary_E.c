#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define V 5  //! Fixed number of vertices. Can change this number accordingly for part a

//* Find the vertex with the minimum distance in the priority queue
int minimumDistance(int distance[], bool sptSet[], int priorityQueue[], int size) {
    int min = INT_MAX, min_index = -1;

    // Find the vertex with  minimum distance
    for (int i = 0; i < size; i++) {
        int v = priorityQueue[i];
        if (!sptSet[v] && distance[v] <= min) {
            min = distance[v];
            min_index = i;
        }
    }

    // Remove minimum vertex from the priority queue
    if (min_index != -1) {
        int minVertex = priorityQueue[min_index];
        priorityQueue[min_index] = priorityQueue[size - 1];  // Move the last vertex down the array to replace the vertex that is taken out
        return minVertex;
    }

    return -1;
}

//* Print the constructed distance array
void printSolution(int dist[]) {
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

//* Dijkstra's algorithm 
void dijkstra(int graph[V][V], int source) {
    int distance[V];   //* Distance array, dist[i] holds the shortest distance from source to i
    bool sptSet[V];  //* True if the vertex is included in the shortest path tree

    //* Priority queue array to hold all vertices
    int queueSize = V;
    int priorityQueue[V];
    
    //* Initialize distances to infinite and sptSet[] to false
    for (int i = 0; i < V; i++) {
        distance[i] = INT_MAX;
        sptSet[i] = false;
        priorityQueue[i] = i;  //* Initially, all vertices are in the priority queue
    }

    //* Distance of source vertex from itself is always 0
    distance[source] = 0;

    //* Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        //* Select vertex with minimum distance value from priority queue
        int u = minimumDistance(distance, sptSet, priorityQueue, queueSize);

        if (u == -1) {
            break;
        }  //* No more vertices in priority queue

        queueSize--;  //* Decrease size of the priority queue after taking out minimum distance vertex
        sptSet[u] = true; //* Vertex u is now in the shortest path tree

        //* Update distance[] of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            //* Update distance[v] if it's not in sptSet, there's an edge from u to v, and the total weight of the path from source to v through u is smaller than distance[v]
            if (!sptSet[v] && graph[u][v] && distance[u] != INT_MAX && distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }

    // Print the final distance array
    printSolution(distance);
}

//* Generate a random graph with random edges and weights
void generateRandomGraph(int graph[V][V], int maxEdges) {
    srand(time(0));  //* Seed for randomness

    // Initialize all edges to 0 (no edge)
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    // Randomly generate edges and assign weights
    int edgeCount = 0;
    while (edgeCount < maxEdges) {
        int u = rand() % V;  //* Random vertex u
        int v = rand() % V;  //* Random vertex v

        // Ensure no self-loops and no duplicate edges
        if (u != v && graph[u][v] == 0) {
            int weight = rand() % 100 + 1;  //* Random weight between 1 and 100
            graph[u][v] = weight;
            graph[v][u] = weight;  //* For undirected graph
            edgeCount++;
        }
    }
}

int main() {
    int graph[V][V];  //* Fixed number of vertices

    int maxEdges = 6;  //* Specify the random number of edges
    generateRandomGraph(graph, maxEdges);

    printf("Generated graph with %d edges:\n", maxEdges);
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%3d ", graph[i][j]);
        }
        printf("\n");
    }

    // Call Dijkstra's algorithm with source vertex 0
    dijkstra(graph, 0);

    return 0;
}
