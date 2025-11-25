//
// Created by alyss on 24/11/2025.
//

#include "graph.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void add_edge(Graph *g, int v, int u, int *monitor) {
    if (g->AdjacencyMatrix[v][u] == 0) {
        g->AdjacencyMatrix[v][u] = g->AdjacencyMatrix[u][v] = 1;

        if (g->Degree[v] == monitor[v]) {
            monitor[v] = (int)ceil(monitor[v] * GOLDEN_RATIO);
            int *tmp = realloc(g->AdjacencyList[v], sizeof(int) * monitor[v]);
            if (!tmp) { fprintf(stderr,"Memory Allocation Failed.\n"); exit(1); }
            g->AdjacencyList[v] = tmp;
        }
        if (g->Degree[u] == monitor[u]) {
            monitor[u] = (int)ceil(monitor[u] * GOLDEN_RATIO);
            int *tmp = realloc(g->AdjacencyList[u], sizeof(int) * monitor[u]);
            if (!tmp) { fprintf(stderr,"Memory Allocation Failed.\n"); exit(1); }
            g->AdjacencyList[u] = tmp;
        }

        g->AdjacencyList[v][g->Degree[v]++] = u;
        g->AdjacencyList[u][g->Degree[u]++] = v;
    }
}


void free_graph(Graph *g) {
    if (!g) return;
    if (g->Degree) free(g->Degree);
    if (g->VertexSet) free(g->VertexSet);
    if (g->AdjacencyList) {
        for (int i = 0; i < g->Order; i++) {
            free(g->AdjacencyList[i]);
        }
        free(g->AdjacencyList);
    }
    if (g->AdjacencyMatrix) {
        for (int i = 0; i < g->Order; i++) {
            free(g->AdjacencyMatrix[i]);
        }
        free(g->AdjacencyMatrix);
    }
    if (g->DisjointSet) {
        for (int i = 0; i < g->graphSet; i++) {
            if (g->DisjointSet[i]) {
                free(g->DisjointSet[i]->Vertex);
                free(g->DisjointSet[i]);
            }
        }
        free(g->DisjointSet);
    }
    free(g);
}

void displayAdjacencyList(Graph *graph)
{
    fprintf(stdout, "\nAdjacency List\n");
    for (int isArbitrary = 0; isArbitrary < graph->Order; isArbitrary++)
    {
        fprintf(stdout, "Vertex [%d] = {", isArbitrary);
        for (int isEntry = 0; isEntry < graph->Degree[isArbitrary]; isEntry++)
        {
            fprintf(stdout, isEntry == graph->Degree[isArbitrary] - 1 ? "%d}\n" : "%d, ", graph->AdjacencyList[isArbitrary][isEntry]);
        }
    }
}

void displaySet(Graph *graph)
{
    fprintf(stdout, "\nSets\n");
    for (int isArbitrary = 0; isArbitrary < graph->graphSet; isArbitrary++)
    {
        fprintf(stdout, "Set [%d] = [", isArbitrary);
        for (int isEntry = 0; isEntry < graph->DisjointSet[isArbitrary]->Cardinality; isEntry++)
        {
            fprintf(stdout, isEntry == graph->DisjointSet[isArbitrary]->Cardinality - 1 ? "%d]\n" : "%d, ", graph->DisjointSet[isArbitrary]->Vertex[isEntry]);
        }
    }
}
