//
// Created by alyss on 24/11/2025.
//

#include "generator.h"

#include <assert.h>

#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long randomGenerator_Unbiased(int max){
    unsigned long limit = (((unsigned long)RAND_MAX + 1) / max) * max, random = -1;
    do
    {
        random = rand();
    } while (random >= limit);
    return random % max;
}

void generate_online_kcolourable(Graph *Graph, int n, int k, double p) {
    // ==== INITIALIZE GRAPH STRUCTURE (arrays, lists, sets) ====
    Graph->Order = n;
    Graph->graphSet = k;
    Graph->Degree = calloc(n, sizeof(int));
    Graph->VertexSet = calloc(n, sizeof(int));
    Graph->AdjacencyList = calloc(n, sizeof(int *));
    Graph->AdjacencyMatrix = calloc(n, sizeof(int *));
    Graph->DisjointSet = calloc(k, sizeof(Set *));
    if (!Graph->Degree || !Graph->VertexSet || !Graph->AdjacencyList || !Graph->AdjacencyMatrix || !Graph->DisjointSet)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return;
    }
    // Used to store adjacency list capacities (for resizing)
    int *isMonitor = calloc(n, sizeof(int));
    if (isMonitor == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return;
    }

    // ======================================================================
    // === STEP 1: CREATE K DISJOINT SETS & ASSIGN VERTICES
    // ======================================================================
    for (int v = 0; v < n; v++) {
        Graph->AdjacencyList[v] = calloc(k-1, sizeof(int));
        Graph->AdjacencyMatrix[v] = calloc(n, sizeof(int));
        isMonitor[v] = k-1;

        if (v < k) {
            Graph->DisjointSet[v] = calloc(1, sizeof(Set));
            Graph->DisjointSet[v]->Vertex = calloc(1, sizeof(int));
            Graph->DisjointSet[v]->Vertex[0] = v;
            Graph->DisjointSet[v]->Cardinality = 1;
            Graph->DisjointSet[v]->Size = 1;
            Graph->VertexSet[v] = v;
        } else {
            int s = randomGenerator_Unbiased(k);
            if (Graph->DisjointSet[s]->Cardinality == Graph->DisjointSet[s]->Size)
                Graph->DisjointSet[s]->Size = (int)ceil(Graph->DisjointSet[s]->Size * GOLDEN_RATIO);
            int *tmp = realloc(Graph->DisjointSet[s]->Vertex, sizeof(int)*Graph->DisjointSet[s]->Size);
            if (!tmp) { fprintf(stderr,"Memory Allocation Failed.\n"); exit(1); }
            Graph->DisjointSet[s]->Vertex = tmp;
            Graph->DisjointSet[s]->Vertex[Graph->DisjointSet[s]->Cardinality++] = v;
            Graph->VertexSet[v] = s;
        }
    }

    // =======================================================================
    // === STEP 2: ADD MANDATORY EDGES BETWEEN EVERY PAIR OF DISTINCT SETS ===
    // =======================================================================
    for (int i = 0; i < k; i++){
        for (int idx = 0; idx < Graph->DisjointSet[i]->Cardinality; idx++){
            assert(Graph->DisjointSet[i] != NULL);
            int v = Graph->DisjointSet[i]->Vertex[idx];
            for (int j = 0; j < k; j++){
                if (i == j) continue;
                assert(Graph->DisjointSet[j] != NULL);
                int uIdx = randomGenerator_Unbiased(Graph->DisjointSet[j]->Cardinality);
                int u = Graph->DisjointSet[j]->Vertex[uIdx];
                add_edge(Graph, v, u, isMonitor);
            }
        }
    }
    // ===================================================================
    // === STEP 3: ADD RANDOM EDGES BETWEEN SETS WITH PROBABILITY p    ===
    // ===================================================================
    for (int i = 0; i < k; i++){
        for (int idx = 0; idx < Graph->DisjointSet[i]->Cardinality; idx++){
            assert(Graph->DisjointSet[i] != NULL);
            int v = Graph->DisjointSet[i]->Vertex[idx];
            for (int j = 0; j < k; j++){
                if (i == j) continue;
                for (int uIdx = 0; uIdx < Graph->DisjointSet[j]->Cardinality; uIdx++){
                    assert(Graph->DisjointSet[j] != NULL);
                    int u = Graph->DisjointSet[j]->Vertex[uIdx];
                    double r = (double)rand() / (RAND_MAX + 1.0);
                    if (r < p)
                        add_edge(Graph, v, u, isMonitor);
                }
            }
        }
    }
    free(isMonitor);
}
