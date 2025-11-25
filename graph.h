//
// Created by alyss on 24/11/2025.
//

#include <stdbool.h>
#ifndef COMP6651_GRAPH_H
#define COMP6651_GRAPH_H

#define GOLDEN_RATIO 1.618

typedef struct Set
{
    int Cardinality, Size, *Vertex;
} Set;

typedef struct Graph
{
    int Order, graphSet, *Degree, *VertexSet, **AdjacencyList, **AdjacencyMatrix;
    Set **DisjointSet;
    bool isCyclic;
} Graph;

void add_edge(Graph *g, int v, int u, int *monitor);
void free_graph(Graph* g);
void displayAdjacencyList(Graph *isGraph);
void displaySet(Graph *isGraph);

#endif //COMP6651_GRAPH_H