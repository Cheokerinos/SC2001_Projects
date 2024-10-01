#ifndef Graph_H
#define Graph_H

#include <stdio.h>
#include <stdlib.h>

#define output_sequence "%d %d %d\n"

typedef struct ListNode {
    int vertex;
    int weight;
    struct ListNode *next;
} ListNode;

typedef struct GraphAdjList {
    ListNode **adjLists;
} GraphAdjList;

typedef struct GraphAdjMatrix {
    int **adjMatrix;
} GraphAdjMatrix;

typedef enum GraphType {
    ADJ_LIST,
    ADJ_MATRIX
} GraphType;

typedef struct Graph {
    int numOfVertices;
    GraphType type;

    union { //store either ADJ_LIST or ADJ_MATRIX
        GraphAdjList *adjList;
        GraphAdjMatrix *adjMatrix;
    } graph;
} Graph;

Graph *create_graph(GraphType type, int numVertices) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->type = type;

    if (type == ADJ_LIST){
        graph->graph.adjList = (GraphAdjList *) malloc(sizeof(GraphAdjList));
        graph->numOfVertices = numVertices;
        graph->graph.adjList->adjLists = (ListNode **) malloc(numVertices * sizeof(ListNode*));
        for(int i = 0; i<numVertices; i++){
            graph->graph.adjList->adjLists[i] = NULL;
        }
    } else if( type == ADJ_MATRIX){
        graph->graph.adjMatrix = (GraphAdjMatrix *) malloc(sizeof(GraphAdjMatrix));
        graph->numOfVertices = numVertices;
        graph->graph.adjMatrix->adjMatrix = (int **) malloc(numVertices * sizeof(int *)); // array of int pointers for each vertice (rows)
        for(int i=0; i<numVertices; i++){ // allocated memory for each row baded on number of vertices (columns)
            graph->graph.adjMatrix->adjMatrix[i] = (int *) malloc(numVertices * sizeof(int));
            for(int j=0; j<numVertices; j++){ //initialising
                graph->graph.adjMatrix->adjMatrix[i][j] = INT_MAX;
            }
        }
    }
    return graph;
}

void add_edge_list(GraphAdjList *graph, int src, int dest, int weight) {
    ListNode *newNode = (ListNode*) malloc(sizeof(ListNode));
    newNode->vertex = dest;
    newNode->next = graph->adjLists[src];
    newNode->weight = weight;
    graph->adjLists[src] = newNode;
}

void add_edge_matrix(GraphAdjMatrix *graph, int src, int dest, int weight) {
    if (weight < graph->adjMatrix[src][dest]) {
        graph->adjMatrix[src][dest] = weight;
    }
}

void add_edge(Graph *graph, int src, int dest, int weight) {
    if (graph->type == ADJ_LIST) {
        add_edge_list(graph->graph.adjList, src, dest, weight);
    } else if (graph->type == ADJ_MATRIX) {
        add_edge_matrix(graph->graph.adjMatrix, src, dest, weight);
    }
}

void delete_graph(Graph *graph) {
    if (graph->type == ADJ_LIST) {
        for (int i = 0; i < graph->numVertices; i++) {
            ListNode *node = graph->graph.adjList->adjLists[i];
            while (node != NULL) {
                ListNode *temp = node;
                node = node->next;
                free(temp);
            }
        }
        free(graph->graph.adjList->adjLists);
        free(graph->graph.adjList);
    } else if (graph->type == ADJ_MATRIX) {
        for (int i = 0; i < graph->numVertices; i++) {
            free(graph->graph.adjMatrix->adjMatrix[i]);
        }
        free(graph->graph.adjMatrix->adjMatrix);
        free(graph->graph.adjMatrix);
    }
    free(graph);
}

void print_graph(Graph *graph) {
    if (graph->type == ADJ_LIST) {
//        printf("Adjacency List:\n");
//        printf("%d\n", graph->numVertices);
        for (int i = 0; i < graph->numVertices; ++i) {
            ListNode *node = graph->graph.adjList->adjLists[i];
            while (node != NULL) {
                printf(output_sequence, i, node->vertex, node->weight);
                node = node->next;
            }
        }
    } else if (graph->type == ADJ_MATRIX) {
        for (int i = 0; i < graph->numVertices; i++) {
            for (int j = 0; j < graph->numVertices; j++) {
                if (graph->graph.adjMatrix->adjMatrix[i][j] != INT_MAX) {
                    printf(output_sequence, i, j, graph->graph.adjMatrix->adjMatrix[i][j]);
                }
            }
        }
    }
}

Graph *adj_graph_to_matrix(Graph *graph) {
    if (graph->type == ADJ_MATRIX) {
        return graph;
    }

    Graph *newGraph = create_graph(ADJ_MATRIX, graph->numVertices);

    for (int i = 0; i < graph->numVertices; i++) {
        ListNode *node = graph->graph.adjList->adjLists[i];
        while (node != NULL) {
            add_edge(newGraph, i, node->vertex, node->weight);
            node = node->next;
        }
    }

    return newGraph;
}

Graph *adj_graph_to_list(Graph *graph) {
    if (graph->type == ADJ_LIST) {
        return graph;
    }

    Graph *newGraph = create_graph(ADJ_LIST, graph->numVertices);

    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->numVertices; j++) {
            if (graph->graph.adjMatrix->adjMatrix[i][j] != INT_MAX) {
                add_edge(newGraph, i, j, graph->graph.adjMatrix->adjMatrix[i][j]);
            }
        }
    }

    return newGraph;
}

#endif 