#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define MAX_V 100000  // Maximum number of vertices. For allocating memory

// Function to find the vertex with the minimum distance in the priority queue
int minimumDistance(int distance[], bool ShortestPathTreeSet[], int priorityQueue[], int size, unsigned long long *Comparisons) {
    int minDistance = INT_MAX, min_index = -1;

    // Find the vertex with minimum distance and save it
    for (int i = 0; i < size; i++) {
        (*Comparisons)++;
        int v = priorityQueue[i];

        // If vertex not in ShortestPathTree & distance of vertex is smaller or equals to infinity  
        if (!ShortestPathTreeSet[v] && distance[v] <= minDistance) { 
            minDistance = distance[v];
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

// Function to print the constructed distance array
void printSolution(int dist[], int V) {
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Dijkstra's algorithm
void dijkstra(int graph[MAX_V][MAX_V], int V, int source, unsigned long long *Comparisons) {
    *Comparisons = 0;
    int distance[V];   // Distance array
    bool ShortestPathTreeSet[V];  // True if the vertex is included in the shortest path tree

    // Priority queue array to hold all vertices
    int queueSize = V;
    int priorityQueue[V];

    // Initialize distances to infinity and ShortestPathTreeSet[] to false
    for (int i = 0; i < V; i++) {
        distance[i] = INT_MAX;
        ShortestPathTreeSet[i] = false;
        priorityQueue[i] = i;  // Initially, all vertices are in the priority queue
    }

    // Distance of starting vertex to itself is always 0
    distance[source] = 0;
   for (int count = 0; count < V - 1; count++) {
        // Select the vertex with minimum distance (ensure to count comparisons)
        int u = minimumDistance(distance, ShortestPathTreeSet, priorityQueue, queueSize, Comparisons);

        if (u == -1) {
            break;
        }

        ShortestPathTreeSet[u] = true;

        for (int v = 0; v < V; v++) {
            // Count comparison when checking if an edge exists
            if (graph[u][v] && !ShortestPathTreeSet[v]) {
                (*Comparisons)++;
                // Count comparison when checking for distance update
                if (distance[u] != INT_MAX && distance[u] + graph[u][v] < distance[v]) {
                    (*Comparisons)++; // Counting the second comparison
                    distance[v] = distance[u] + graph[u][v];
                }
            }
        }
    }
    
}

// Function to generate a random graph with a fixed number of edges
void generateRandomGraph(int graph[MAX_V][MAX_V], int V, int maxEdges) {
    srand(time(0));  // Seed for randomness

    // Initialize all edges to 0 (no edge)
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    // Randomly generate edges and assign weights
    int edgeCount = 0;
    while (edgeCount < maxEdges) {
        int u = rand() % V;  // Random vertex u
        int v = rand() % V;  // Random vertex v

        // Ensure no self-loops and no duplicate edges
        if (u != v && graph[u][v] == 0) {
            int weight = rand() % 100 + 1;  // Random weight between 1 and 100
            graph[u][v] = weight;
            graph[v][u] = weight;  // For undirected graph
            edgeCount++;
        }
    }
}

int main() {
    srand(time(0)); 

    // Maximum number of edges
    const int fixedEdges = 10000;  //! Can change this value to any other value. I put 10000 to make it fair
    int (*graph)[MAX_V] = malloc(MAX_V * MAX_V * sizeof(int));  

    if (graph == NULL) {
        printf("Memory allocation failed!\n");
        return 1; 
    }

    FILE *file = fopen("Matrix_Fixed_E_Vary_V.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "Number_Of_Vertices,Comparisons,CPU Time (ms)\n");

    for (int V = 100; V <= 5000; V += 100) {      
        unsigned long long Comparisons = 0;  // Initialize comparisons count
        clock_t start, end;
        double cpu_time_used;

        generateRandomGraph(graph, V, fixedEdges);  // Generate random graph with fixed edges

        start = clock();

        // Call Dijkstra's algorithm with source vertex 0
        dijkstra(graph, V, 0, &Comparisons);

        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

        fprintf(file, "%d,%llu,%.3f\n", V, Comparisons, cpu_time_used);
        printf("Processing for %d vertices is done.\n", V);
    }

    fclose(file);
    printf("\nCompleted all vertices\n");
    free(graph); 

    return 0;
}
