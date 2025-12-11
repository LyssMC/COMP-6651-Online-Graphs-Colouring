//
// Created by alyss on 24/11/2025.
//

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "firstfit.h"
#include "graph.h"


uint64_t fowlerNollVo_1A_Hash(const int key) {
    uint64_t hash = 0xcbf29ce484222325ULL;
    uint8_t byte = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        byte = (key >> (8 * i)) & 0xFF;
        hash ^= byte;
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

int *firstFitAlgorithm(Graph *graph)
{
    if (graph == NULL || graph->Order <= 0) return NULL;
    int index = 0, arbitrary = 0, iteration = 0, vertex = -1, chroma = 0;
    bool *isAdjacentColor = NULL;

    // Output array: colour assignment for each vertex (0 = uncoloured)
    int *colour = calloc(graph->Order, sizeof(int));
    if (colour == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        free(colour);
        return NULL;
    }

    // Online sequence (σ): random permutation of all vertices
    int *isRandomizedVertex = calloc(graph->Order, sizeof(int));
    if (isRandomizedVertex == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        free(isRandomizedVertex);
        return NULL;
    }

    // ---------------------------------------------------------------
    // STEP 1: Compute max degree and build σ = [0,1,...,n-1]
    // ---------------------------------------------------------------
    for (iteration = 0; iteration < graph->Order; iteration++)
    {
        if (*(graph->Degree + iteration) > chroma)
        {
            chroma = *(graph->Degree + iteration);
        }
        *(isRandomizedVertex + iteration) = iteration;
    }

    // ---------------------------------------------------------------
    // STEP 2: Randomly shuffle σ (Fisher–Yates)
    // ---------------------------------------------------------------
    for (iteration = graph->Order - 1; iteration > 0; iteration--)
    {
        index = randomGenerator_Unbiased(iteration + 1);
        arbitrary = *(isRandomizedVertex + iteration);
        *(isRandomizedVertex + iteration) = *(isRandomizedVertex + index);
        *(isRandomizedVertex + index) = arbitrary;
    }

    // Allocate boolean array to mark colours used by neighbours
    isAdjacentColor = calloc(++chroma, sizeof(bool));
    if (isAdjacentColor == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        free(isAdjacentColor);
        return NULL;
    }

    // ---------------------------------------------------------------
    // STEP 3: FirstFit colouring in the online order σ
    // ---------------------------------------------------------------
    iteration = 0;
    while (iteration < graph->Order)
    {
        vertex = *(isRandomizedVertex + iteration);
        // Mark colours used by already-coloured neighbours
        index = 0;
        while (index < *(graph->Degree + vertex))
        {
            arbitrary = *(*(graph->AdjacencyList + vertex) + index);
            if (*(colour + arbitrary) > 0)
            {
                isAdjacentColor[colour[arbitrary]] = true;
            }
            index++;
        }

        // Assign smallest colour not used by neighbours
        for (chroma = 1; chroma < *(graph->Degree + vertex) + 1; chroma++)
        {
            if (!*(isAdjacentColor + chroma))
            {
                break;
            }
        }
        *(colour + vertex) = chroma;
        // Reset flags for next vertex
        memset(isAdjacentColor, 0, sizeof(bool) * (*(graph->Degree + vertex) + 1));
        iteration++;
    }
    free(isAdjacentColor);
    isAdjacentColor = NULL;
    free(isRandomizedVertex);
    isRandomizedVertex = NULL;
    return colour;
}

int *firstFitAlgorithm_Hash(Graph *isGraph)
{
    if (isGraph == NULL || isGraph->Order <= 0) return NULL;
    int isIndex = 0, isArbitrary = 0, isIteration = 0, isVertex = -1, isMaximal = 0, isChroma = 0;
    bool hasEntry = false;
    int *isColour = calloc(isGraph->Order, sizeof(int));
    if (isColour == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return NULL;
    }
    int *isRandomizedVertex = calloc(isGraph->Order, sizeof(int));
    if (isRandomizedVertex == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return NULL;
    }
    for (isIteration = 0; isIteration < isGraph->Order; isIteration++)
    {
        *(isRandomizedVertex + isIteration) = isIteration;
        if (*(isGraph->Degree + isIteration) > isMaximal)
        {
            isMaximal = *(isGraph->Degree + isIteration);
        }
    }
    for (isIteration = isGraph->Order - 1; isIteration > 0; isIteration--)
    {
        isIndex = randomGenerator_Unbiased(isIteration + 1);
        isArbitrary = *(isRandomizedVertex + isIteration);
        *(isRandomizedVertex + isIteration) = *(isRandomizedVertex + isIndex);
        *(isRandomizedVertex + isIndex) = isArbitrary;
    }
    Color_HashEntry **isAdjacentColour = calloc(isMaximal, sizeof(Color_HashEntry *));
    if (isAdjacentColour == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return NULL;
    }
    while (isIteration < isGraph->Order)
    {
        isVertex = *(isRandomizedVertex + isIteration);
        isIndex = 0;
        while (isIndex < *(isGraph->Degree + isVertex))
        {
            if (isColour[isGraph->AdjacencyList[isVertex][isIndex]] > 0)
            {
                isArbitrary = fowlerNollVo_1A_Hash(isColour[isGraph->AdjacencyList[isVertex][isIndex]]) % isMaximal;
                if (*(isAdjacentColour + isArbitrary) == NULL)
                {
                    Color_HashEntry *isEntry = calloc(1, sizeof(Color_HashEntry));
                    if (isEntry == NULL)
                    {
                        fprintf(stderr, "Memory Allocation Failed.\n");
                        return NULL;
                    }
                    isEntry->isColor = isColour[isGraph->AdjacencyList[isVertex][isIndex]];
                    isEntry->isIdentifier = isVertex;
                    isEntry->isNext = NULL;
                    *(isAdjacentColour + isArbitrary) = isEntry;
                }
                else
                {
                    Color_HashEntry *isPlacement = *(isAdjacentColour + isArbitrary);
                    while (isPlacement)
                    {
                        if (isPlacement->isIdentifier != isVertex)
                        {
                            isPlacement->isColor = isColour[isGraph->AdjacencyList[isVertex][isIndex]];
                            isPlacement->isIdentifier = isVertex;
                            hasEntry = true;
                            break;
                        }
                        isPlacement = isPlacement->isNext;
                    }
                    if (!hasEntry)
                    {
                        Color_HashEntry *isEntry = calloc(1, sizeof(Color_HashEntry));
                        if (isEntry == NULL)
                        {
                            fprintf(stderr, "Memory Allocation Failed.\n");
                            return NULL;
                        }
                        isEntry->isColor = isColour[isGraph->AdjacencyList[isVertex][isIndex]];
                        isEntry->isIdentifier = isVertex;
                        isEntry->isNext = *(isAdjacentColour + isArbitrary);
                        *(isAdjacentColour + isArbitrary) = isEntry;
                    }
                }
                hasEntry = false;
            }
            isIndex++;
        }
        isChroma = 1;
        while (true)
        {
            isArbitrary = fowlerNollVo_1A_Hash(isChroma) % isMaximal;
            Color_HashEntry *isPlacement = *(isAdjacentColour + isArbitrary);
            hasEntry = false;
            while (isPlacement)
            {
                if (isPlacement->isIdentifier == isVertex && isPlacement->isColor == isChroma)
                {
                    hasEntry = true;
                    break;
                }
                isPlacement = isPlacement->isNext;
            }
            if (!hasEntry)
            {
                *(isColour + isVertex) = isChroma;
                break;
            }
            isChroma++;
        }
        isIteration++;
    }
    for (isIteration = 0; isIteration < isMaximal; isIteration++)
    {
        free(*(isAdjacentColour + isIteration));
        *(isAdjacentColour + isIteration) = NULL;
    }
    free(isAdjacentColour);
    isAdjacentColour = NULL;
    free(isRandomizedVertex);
    isRandomizedVertex = NULL;
    return isColour;
}

/* -------------------------------------------------------------
 * Heap-sort vertices by decreasing degree
 * ------------------------------------------------------------- */
static void heapSortByDegree(int *ordered, const Graph *graph){
    int n = graph->Order;
    int i, v, left, right, swapIndex;

    /* Build max heap */
    for (i = n/2 - 1; i >= 0; i--) {
        v = i;
        for (;;) {
            swapIndex = v;
            left  = 2*v + 1;
            right = 2*v + 2;

            if (left < n &&
                graph->Degree[ ordered[left] ] >
                graph->Degree[ ordered[swapIndex] ])
            {
                swapIndex = left;
            }
            if (right < n &&
                graph->Degree[ ordered[right] ] >
                graph->Degree[ ordered[swapIndex] ])
            {
                swapIndex = right;
            }

            if (swapIndex == v) break;

            {
                int tmp = ordered[v];
                ordered[v] = ordered[swapIndex];
                ordered[swapIndex] = tmp;
            }
            v = swapIndex;
        }
    }

    /* Heapsort extraction */
    for (i = n - 1; i > 0; i--) {
        int tmp = ordered[0];
        ordered[0] = ordered[i];
        ordered[i] = tmp;

        v = 0;
        for (;;) {
            swapIndex = v;
            left  = 2*v + 1;
            right = 2*v + 2;

            if (left < i &&
                graph->Degree[ ordered[left] ] >
                graph->Degree[ ordered[swapIndex] ])
            {
                swapIndex = left;
            }
            if (right < i &&
                graph->Degree[ ordered[right] ] >
                graph->Degree[ ordered[swapIndex] ])
            {
                swapIndex = right;
            }

            if (swapIndex == v) break;

            tmp = ordered[v];
            ordered[v] = ordered[swapIndex];
            ordered[swapIndex] = tmp;
            v = swapIndex;
        }
    }
}

static int attemptBinSearch(const Graph *graph,const int *ordered,int limit,int *colourOut,int *isMark,int genStart){
    int n = graph->Order;
    int pos, vertex, deg, i, c, nei;
    int gen = genStart;

    memset(colourOut, 0, n * sizeof(int));

    for (pos = 0; pos < n; pos++) {

        vertex = ordered[pos];
        deg    = graph->Degree[vertex];

        /* Mark adjacent colours */
        for (i = 0; i < deg; i++) {
            nei = graph->AdjacencyList[vertex][i];
            c = colourOut[nei];
            if (c > 0 && c <= limit)
                isMark[c] = gen;
        }

        /* select smallest free colour */
        for (c = 1; c <= limit; c++) {
            if (isMark[c] != gen)
                break;
        }
        if (c > limit)
            return 0; /* fail */

        colourOut[vertex] = c;

        /* increment gen for next vertex marking pass */
        gen++;
        if (gen == 0) {
            memset(isMark, 0, (limit+1) * sizeof(int));
            gen = 1;
        }
    }

    return 1;
}


/* -------------------------------------------------------------
 * binary-search first fit
 * ------------------------------------------------------------- */
int *firstFitAlgorithm_BinSearch(Graph *graph){
    if (graph == NULL || graph->Order <= 0)
        return NULL;

    {
        int n = graph->Order;
        int *colour = calloc(n, sizeof(int));
        int *ordered = calloc(n, sizeof(int));
        int *isMark, *tempColours;
        int lower, upper, mid;
        int gen = 1;
        int i;
        int maxDegree = 0;

        if (colour == NULL || ordered == NULL) {
            free(colour);
            free(ordered);
            return NULL;
        }

        for (int i = 0; i < n; i++) {
            ordered[i] = i;
            if (graph->Degree[i] > maxDegree)
                maxDegree = graph->Degree[i];
        }
        heapSortByDegree(ordered, graph);

        lower = 1;
        upper = maxDegree + 1;

        isMark = calloc(upper + 1, sizeof(int));
        tempColours = calloc(n, sizeof(int));
        if (isMark == NULL || tempColours == NULL) {
            free(colour);
            free(ordered);
            free(isMark);
            free(tempColours);
            return NULL;
        }

        while (lower <= upper) {
            mid = (lower + upper) / 2;

            if (attemptBinSearch(graph, ordered, mid, tempColours, isMark, gen)) {
                memcpy(colour, tempColours, n * sizeof(int));
                upper = mid - 1;
            } else {
                lower = mid + 1;
            }

            gen++;
            if (gen == 0) {
                memset(isMark, 0, (upper+1) * sizeof(int));
                gen = 1;
            }
        }

        free(tempColours);
        free(isMark);
        free(ordered);

        return colour;
    }
}