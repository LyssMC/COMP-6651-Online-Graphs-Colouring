//
// Created by alyss on 24/11/2025.
//

#include "cbip.h"

#include <stdio.h>
#include <stdlib.h>

#include "generator.h"


int *coloringBasedIntervalPartitionAlgorithm(Graph *isGraph)
{
    int isIndex = 0, isArbitrary = 0, isIteration = 0, isVertex = -1, isNavigate = 0, isChroma = 0, isProxy = 0;
    bool *isAdjacentColor = NULL;

    // c(V): colours of vertices (0 = uncoloured)
    int *isVertexColor = calloc(isGraph->Order, sizeof(int));
    if (isVertexColor == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return NULL;
    }

    // σ: randomized order of vertices
    int *isRandomizedVertex = calloc(isGraph->Order, sizeof(int));
    if (isRandomizedVertex == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        free(isVertexColor);
        return NULL;
    }

    // new: auxiliary arrays for CBIP (connected component + bipartition)
    int *isQueue = calloc(isGraph->Order, sizeof(int));             // NEW
    if(isQueue == NULL){fprintf(stderr, "Memory Allocation Failed.\n"); free(isVertexColor); free(isRandomizedVertex); return NULL;} // new
    int *isSide = calloc(isGraph->Order, sizeof(int));              // NEW
    if(isSide == NULL){fprintf(stderr, "Memory Allocation Failed.\n"); free(isVertexColor); free(isRandomizedVertex); free(isQueue); return NULL;} //new

    // fill σ with vertices 0..n-1
    for (isIteration = 0; isIteration < isGraph->Order; isIteration++)
    {
        isRandomizedVertex[isIteration] = isIteration;
    }

    // Fisher–Yates shuffle to get a random online order σ
    for (isIteration = isGraph->Order - 1; isIteration > 0; isIteration--)
    {
        isIndex = randomGenerator_Unbiased(isIteration + 1);
        isArbitrary = *(isRandomizedVertex + isIteration);
        *(isRandomizedVertex + isIteration) = *(isRandomizedVertex + isIndex);
        *(isRandomizedVertex + isIndex) = isArbitrary;
    }

    // CHANGED: reset isIteration to start the online process
    isIteration = 0;

    while (isIteration < isGraph->Order)
    {
        // v ← σ(i)
        isVertex = *(isRandomizedVertex + isIteration);

        // NEW: initialize sides to -1 (not in component)
        for(isIndex = 0; isIndex < isGraph->Order; isIndex++){
            isSide[isIndex] = -1;
        }
        // ===== NEW: Find connected component CC of v in current partial graph =====
        // current partial graph = vertices already coloured (isVertexColor[w] > 0) + v
        int isHead = 0, isTail = 0;          // NEW
        isQueue[isTail++] = isVertex;        // NEW
        isSide[isVertex] = 0;                // NEW: put v in A-side
        while (isHead < isTail) // NEW: BFS over partial graph
        {
            int u = isQueue[isHead++];
            for(isIndex = 0; isIndex < *(isGraph->Degree + u); isIndex++){
                int w = *(*(isGraph->AdjacencyList + u) + isIndex);

                // NEW: only follow edges to vertices already revealed in σ (coloured) or v itself
                if(w != isVertex && *(isVertexColor + w) == 0){
                    continue;
                }

                if(isSide[w] == -1){
                    isSide[w] = 1 - isSide[u];         // NEW: bipartition A/B
                    isQueue[isTail++] = w;
                }
            }
        }
        // ===== NEW: Mark colours used in B-side of CC =====
        // (B-side = all vertices with isSide[*] == 1)
        // CHANGED: size of isAdjacentColor is now based on isGraph->isOrder, not isArbitrary.
        isAdjacentColor = calloc(isGraph->Order + 1, sizeof(bool));
        if (isAdjacentColor == NULL)
        {
            fprintf(stderr, "Memory Allocation Failed.\n");
            free(isVertexColor); free(isRandomizedVertex); free(isQueue); free(isSide);
            return NULL;
        }
        for (isIndex = 0; isIndex < isGraph->Order; isIndex++)
        {
            if (isSide[isIndex] == 1){  // B-side
                int c = *(isVertexColor + isIndex);
                if(c > 0){
                    isAdjacentColor[c] = true;
                }
            }
        }
        // ===== choose smallest colour not used in B =====
        for (isChroma = 1; isChroma < isGraph->Order; isChroma++)
        {
            if (!*(isAdjacentColor + isChroma))
            {
                break;
            }
        }
        *(isVertexColor + isVertex) = isChroma;
        free(isAdjacentColor);
        isAdjacentColor = NULL;
        isIteration++;
    }
    free(isRandomizedVertex);
    free(isQueue);            isQueue = NULL;           // NEW
    free(isSide);             isSide = NULL;           // NEW
    isRandomizedVertex = NULL;
    return isVertexColor;
}
