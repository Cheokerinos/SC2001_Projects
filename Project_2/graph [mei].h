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

typedef struct GraphAdjacentList {
    ListNode **adjacentList;
} GraphAdjacentList;

typedef struct GraphAdjacentMatrix {
    int **adjacentMatrix;
} GraphAdjacentMatrix;

typedef enum GraphType {
    ADJACENT_LIST,
    ADJACENT_MATRIX
} GraphType;

typedef struct Graph {
    int numberOfVertices;
    GraphType type;

    union { //store either ADJACENT_LIST or ADJACENT_MATRIX
        GraphAdjacentList *adjacentList;
        GraphAdjacentMatrix *adjacentMatrix;
    } graph;
} Graph;

//* Part a

Graph *adj_graph_to_matrix(Graph *graph) {
    if (graph -> type == ADJACENT_MATRIX) {
        return graph;
    }

    Graph *newGraph = create_adjacentMatrix_graph(GraphType type, int numVertices);

    for (int i = 0; i < graph -> numVertices; i++) {
        ListNode *node = graph-> graph.adjacentMatrix -> adjacentMatrix[i];
        while (node != NULL) {
            add_edge(newGraph, i, node -> vertex, node -> weight);
            node = node -> next;
        }
    }

    return newGraph;
}

Graph *create_adjacentMatrix_graph(GraphType type, int numVertices) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph -> type = type;
    
    graph -> graph.adjacentMatrix = (GraphAdjacentMatrix *) malloc(sizeof(GraphAdjacentMatrix));
    graph -> numberOfVertices = numVertices;
    graph -> graph.adjacentMatrix -> adjacentMatrix = (int **) malloc(numVertices * sizeof(int *)); //* array of int pointers for each vertice (rows)
    for(int i = 0; i < numVertices; i++) { //* allocated memory for each row based on number of vertices (columns)
        graph -> graph.adjacentMatrix -> adjacentMatrix[i] = (int *) malloc(numVertices * sizeof(int));
        for(int j = 0; j < numVertices; j++){ //* initialising
            graph -> graph.adjacentMatrix -> adjacentMatrix[i][j] = INT_MAX;
        }
    }

    return graph;
}

void add_edge_matrix (GraphAdjMatrix *graph, int soruce, int destination, int weight) {
    if (weight < graph -> adjacentMatrix[soruce][destination]) {
        graph -> adjacentMatrix[soruce][destination] = weight;
    }
}

/*
void add_edge(Graph *graph, int src, int dest, int weight) {
    if (graph->type == ADJ_LIST) {
        add_edge_list(graph->graph.adjList, src, dest, weight);
    } else if (graph->type == ADJ_MATRIX) {
        add_edge_matrix(graph->graph.adjMatrix, src, dest, weight);
    }
}
*/

//* Part b

Graph *create_adjacentList_graph(GraphType type, int numVertices) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph -> type = type;  

    graph -> graph.adjacentList = (GraphAdjacentList *) malloc(sizeof(GraphAdjacentList));
    graph -> numberOfVertices = numVertices;
    graph -> graph.adjacentList -> adjacentList = (ListNode **) malloc(numVertices * sizeof(ListNode*));
    for(int i = 0; i < numVertices; i++){
        graph -> graph.adjacentList -> adjacentList[i] = NULL;
    }

    return graph;
}

//* Others
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