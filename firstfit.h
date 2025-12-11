//
// Created by alyss on 24/11/2025.
//

#ifndef COMP6651_FIRSTFIT_H
#define COMP6651_FIRSTFIT_H
#include <stdint.h>

typedef struct Color_HashEntry
{
    int isColor, isIdentifier;
    struct Color_HashEntry *isNext;
}Color_HashEntry;

uint64_t fowlerNollVo_1A_Hash(const int isKey);
int *firstFitAlgorithm(Graph *Graph);
int *firstFitAlgorithm_Hash(Graph *Graph);
static void heapSortByDegree(int *ordered, const Graph *graph);
static int attemptBinSearch(const Graph *graph,const int *ordered,int limit,int *colourOut,int *isMark,int genStart);
int *firstFitAlgorithm_BinSearch(Graph *graph);

#endif //COMP6651_FIRSTFIT_H