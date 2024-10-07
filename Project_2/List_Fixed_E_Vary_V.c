#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_V 100000  // Maximum number of vertices. For allocating memory

// Define the ListNode structure for Adjacency List
typedef struct ListNode {
    int vertex;
    int weight;
    struct ListNode* next;
} ListNode;

// Define the Graph structure with an Adjacency List
typedef struct GraphAdjList {
    ListNode** adjLists;  // Array of adjacency lists
} GraphAdjList;

// Define Min Heap Node structure
typedef struct MinHeapNode {
    int v;
    int dist;
} MinHeapNode;

// Define Min Heap structure
typedef struct MinHeap {
    int size;
    int capacity;
    int* pos;  // Position of vertices
    MinHeapNode** array;
} MinHeap;

// Function to create a new Min Heap Node
MinHeapNode* newMinHeapNode(int v, int dist) {
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Function to create a Min Heap
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// Swap two Min Heap nodes
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Heapify function for Min Heap
void minHeapify(MinHeap* minHeap, int idx, unsigned long long *Comparisons) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Compare left child
    if (left < minHeap->size) {
        (*Comparisons)++;  // Increment comparison count
        if (minHeap->array[left]->dist < minHeap->array[smallest]->dist) {
            smallest = left;
        }
    }

    // Compare right child
    if (right < minHeap->size) {
        (*Comparisons)++;  // Increment comparison count
        if (minHeap->array[right]->dist < minHeap->array[smallest]->dist) {
            smallest = right;
        }
    }

    // If the smallest isn't the root
    if (smallest != idx) {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];
        
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        // Recursively heapify the affected subtree
        minHeapify(minHeap, smallest, Comparisons);
    }
}

// Extract the node with the minimum distance from the Min Heap
MinHeapNode* extractMin(MinHeap* minHeap, unsigned long long *Comparisons) {
    // Base case: If the heap is empty, return NULL
    if (minHeap->size == 0)
        return NULL;

    // Store the root node (minimum element)
    MinHeapNode* root = minHeap->array[0];

    // Replace the root with the last node in the heap
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of the root and last node in the pos array
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size by 1
    (minHeap->size)--;

    // Restore the heap property by heapifying from the root
    minHeapify(minHeap, 0, Comparisons);

    // Return the original root (minimum element)
    return root;
}

// Decrease the distance value of a given vertex
void decreaseKey(MinHeap* minHeap, int v, int dist, unsigned long long *Comparisons) {
    // Get index of the node in the heap
    int i = minHeap->pos[v];

    if (i == -1) return; // Safety check: if the node is not valid, skip

    // Update the distance value
    minHeap->array[i]->dist = dist;

    // Move up the tree while necessary (restore min-heap property)
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        (*Comparisons)++;  // Count the comparison
        
        // Swap positions in the heap
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;

        // Swap the nodes in the heap array
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // Move up to the parent
        i = (i - 1) / 2;
    }
}

// Check if a vertex is in the Min Heap
int isInMinHeap(MinHeap* minHeap, int v) {
    return minHeap->pos[v] < minHeap->size;
}

// Function to add an edge to the adjacency list
void addEdge(GraphAdjList* graph, int src, int dest, int weight) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->vertex = dest;
    newNode->weight = weight;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // For undirected graph, add an edge from dest to src as well
    newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->vertex = src;
    newNode->weight = weight;
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Dijkstra's Algorithm using Min Heap and Adjacency List
void dijkstra(GraphAdjList* graph, int src, int V, unsigned long long *Comparisons) {
    int* dist = (int*)malloc(V * sizeof(int)); // Allocate memory for distances
    if (dist == NULL) {
        perror("Failed to allocate memory for distances");
        exit(EXIT_FAILURE);
    }
    *Comparisons = 0;
    MinHeap* minHeap = createMinHeap(V);

    for (int v = 0; v < V; v++) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src], Comparisons);
    minHeap->size = V;

    while (minHeap->size != 0) {
        MinHeapNode* minHeapNode = extractMin(minHeap, Comparisons);
        if (minHeapNode == NULL) {
            printf("minHeapNode is NULL \n");
            break; // Exit loop to avoid segmentation fault
        }
        int u = minHeapNode->v;

        ListNode* pCrawl = graph->adjLists[u];
        while (pCrawl != NULL) {
            int v = pCrawl->vertex;
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && dist[u] + pCrawl->weight < dist[v]) {
                (*Comparisons)++;
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v], Comparisons);
            }
            pCrawl = pCrawl->next;
        }
    }

    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; ++i) {
        printf("%d \t %d\n", i, dist[i]);
    }
}

// Function to generate a random graph with a fixed number of edges
void generateRandomGraph(GraphAdjList* graph, int V, int maxEdges) {
    srand(time(0));

    for (int i = 0; i < V; ++i) {
        graph->adjLists[i] = NULL;
    }

    int edgeCount = 0;
    while (edgeCount < maxEdges) {
        int u = rand() % V;
        int v = rand() % V;

        if (u != v) {
            int weight = rand() % 100 + 1;
            addEdge(graph, u, v, weight);
            edgeCount++;
        }
    }
    for (int i = 0; i < V; i++) {
    printf("Vertex %d:", i);
    ListNode* temp = graph->adjLists[i];
    while (temp) {
        printf(" -> %d (weight: %d)", temp->vertex, temp->weight);
        temp = temp->next;
    }
    printf("\n");
}
}

// Main function to generate the graph and run Dijkstra
int main() {
    const int fixedEdges = 10000;

    GraphAdjList* graph = (GraphAdjList*)malloc(sizeof(GraphAdjList));
    graph->adjLists = (ListNode**)malloc(MAX_V * sizeof(ListNode*));

    for (int i = 0; i < MAX_V; i++) {
        graph->adjLists[i] = NULL;
    }

    FILE* file = fopen("List_Fixed_E_Vary_V.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    

    fprintf(file, "Number_Of_Vertices,Comparisons,CPU Time (ms)\n");

    for (int V = 100; V <= 10000; V += 100) {
        clock_t start, end;
        double cpu_time_used;
        unsigned long long Comparisons = 0;


        generateRandomGraph(graph, V, fixedEdges);

        start = clock();
        dijkstra(graph, 0, V, &Comparisons);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        fprintf(file, "%d,%llu,%.3f\n", V, Comparisons, cpu_time_used);
        printf("Processing for %d vertices is done.\n", V);
    }

    fclose(file);
    free(graph->adjLists);
    free(graph);

    return 0;
}
