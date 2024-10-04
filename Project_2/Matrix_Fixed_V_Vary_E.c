#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define V 10000 //! Fixed number of vertices. Can change this number accordingly for part a

//! NOTE: If it generates 2147483647, this means it's an infinity number & this means the vertex is unreachable. 
//! This is because I have set it to INT_MAX, which is supposedly meant to represent infinity.
//! If it outputs INT_MAX, means the vertex is still at infinity and not updated  
//! And yes this is Undirected Graph

//* Find the vertex with the minimum distance in the priority queue
int minimumDistance(int distance[], bool ShortestPathTreeSet[], int priorityQueue[], int size, unsigned long long *Comparisons) {
    int minDistance = INT_MAX, min_index = -1;

    //* Find the vertex with minimum distance and save it
    for (int i = 0; i < size; i++) {
        (*Comparisons)++;
        int v = priorityQueue[i]; //* Checks the vertice in the priority queue and assign it to INT
        
        //* If vertice not in ShortestPathTree &
        //* If distance of vertice is smaller or equals to infinity  
        if (!ShortestPathTreeSet[v] && distance[v] <= minDistance) { 
                minDistance = distance[v];
                min_index = i;
        }
    
    }

    //* Remove minimum vertex from the priority queue
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

//* Dijkstra's algorithm, big V has been defined.
void dijkstra(int graph[V][V], int source, unsigned long long *Comparisons) {
    *Comparisons = 0;
    int distance[V];   //* Distance array, distance[v] holds the shortest distance from source to i
    bool ShortestPathTreeSet[V];  //* True if the vertex is included in the shortest path tree

    //* Priority queue array to hold all vertices, part a
    int queueSize = V;
    int priorityQueue[V];
    
    //* Initialize distances to infinity and ShortestPathTreeSet[] to false as there are currently no vertices in ShortestPathTreeSet
    for (int i = 0; i < V; i++) {
        distance[i] = INT_MAX;
        ShortestPathTreeSet[i] = false;
        priorityQueue[i] = i;  //* Initially, all vertices are in the priority queue
    }

    //* Distance of starting vertex to itself is always 0
    distance[source] = 0;

    //* Find the shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        //* Select a vertex with a minimum distance value from the priority queue
        int u = minimumDistance(distance, ShortestPathTreeSet, priorityQueue, queueSize, Comparisons);

        //* Empty priority queue, meaning no more vertices to include
        if (u == -1) {
            break;
        }  

        queueSize--;  //* Decrease size of the priority queue after taking out each minimum distance vertex
        ShortestPathTreeSet[u] = true; //* Selected vertex u is now in the shortest path tree, no longer inside priority queue

        //* Update distance[] of the adjacent vertices of the selected vertex
        //* Loops until it has gone through all vertices
        for (int v = 0; v < V; v++) { 
            //* Update distance[] if it's not yet in ShortestPathTreeSet & there's an edge from u to v & 
            //* the total weight of the path from starting vertex to v through u is smaller than distance[v]
            if (!ShortestPathTreeSet[v] && graph[u][v] && distance[u] != INT_MAX && distance[u] + graph[u][v] < distance[v]) {
                (*Comparisons)++;
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }    // Debug: Output the selected vertex and updated distances
    //     printf("Selected vertex: %d\n", u);
    //     printf("Updated distances: ");
    //     for (int i = 0; i < V; i++) {
    //         printf("%d ", distance[i]);
    //     }
    //     printf("\n");
    // }

    // Print the final distance array
    //printSolution(distance);
}

//* Generate a random graph with random edges and weights
void generateRandomGraph(int graph[V][V], int maxEdges) {
    srand(time(0));  //* Seed for randomness

    //* Initialize all edges to 0 (no edge)
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    //* Randomly generate edges and assign weights
    int edgeCount = 0;
    while (edgeCount < maxEdges) {
        int u = rand() % V;  //* Random vertex u
        int v = rand() % V;  //* Random vertex v

        //* Ensure no self-loops and no duplicate edges
        if (u != v && graph[u][v] == 0) {
            int weight = rand() % 100 + 1;  //* Random weight between 1 and 100. No 0 because it's for starting vertex only. No negative numbers either.
            graph[u][v] = weight;
            graph[v][u] = weight;  //! For undirected graph
            edgeCount++;
        }
    }
}

int main() {
    srand(time(0)); 

    int (*graph)[V] = malloc(V * V * sizeof(int));  //* Creates Segmentation Error if I did not include this
    if (graph == NULL) {
        printf("Memory allocation failed!\n");
        return 1; 
    }

    FILE *file = fopen("Matrix_Fixed_V_Vary_E.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "Number_Of_Edges,Comparisons,CPU Time (ms)\n");

    for (int numberOfEdges = 1000; numberOfEdges <= 100000; numberOfEdges += 1000) {      
        unsigned long long Comparisons = 0;  // Initialize comparisons count
        clock_t start, end;
        double cpu_time_used;

        generateRandomGraph(graph, numberOfEdges);

        start = clock();

        // Debugging. Print the generated graph with vertex labels.
/*
        printf("\nGenerated graph with %d edges:\n", numberOfEdges);
        printf("    ");  // Initial space for the top-left corner
        for (int j = 0; j < V; j++) {
            printf("%3d ", j);  // Print column labels
        }
        printf("\n");

        for (int i = 0; i < V; i++) {
            printf("%3d |", i);  // Print row label
            for (int j = 0; j < V; j++) {
                printf("%3d ", graph[i][j]);  // Print the adjacency matrix values
            }
            printf("\n");
        } 
        */

        //* Call Dijkstra's algorithm with source vertex 0
        dijkstra(graph, 0, &Comparisons);

        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

        fprintf(file, "%d,%llu,%.3f\n", numberOfEdges, Comparisons, cpu_time_used);
        printf("Processing for %d edges is done.\n", numberOfEdges);
    }

    fclose(file);

    printf("\nCompleted all edges\n");

    free(graph); 

    return 0;
}
