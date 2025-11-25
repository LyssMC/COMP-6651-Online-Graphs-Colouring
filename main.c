#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"
#include "generator.h"
#include "firstfit.h"
#include "cbip.h"

#define GOLDEN_RATIO 1.618

void displayColour(int *isColour, int isOrder);
void isSimulationResult();

void displayColour(int *isColour, int isOrder)
{
    for (int isIndex = 0; isIndex < isOrder; isIndex++)
    {
        fprintf(stdout, isIndex == isOrder - 1 ? "[%d => %d]\n" : "[%d => %d], ", isIndex, *(isColour + isIndex));
    }
    return;
}

void isSimulationResult()
{
    int isGenerate = 100, isInstance = 0, isIteration = 0, isOrder[6] = {50, 100, 200, 400, 800, 1600}, isSet[3] = {2, 3, 4}, *isColour = NULL;
    double isRatio = DBL_MIN, isMean = 0, isStandardDeviation = 0;
    double *isCompetitiveRatio = calloc(isGenerate, sizeof(double));
    if (isCompetitiveRatio == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return;
    }
    fprintf(stdout, "FirstFit Algorithm\n");
    for (int isIndex = 0; isIndex < sizeof(isSet) / sizeof(isSet[0]); isIndex++)
    {
        for (int isArbitrary = 0; isArbitrary < sizeof(isOrder) / sizeof(isOrder[0]); isArbitrary++)
        {
            for (; isInstance < isGenerate; isInstance++)
            {
                Graph *isGraph = calloc(1, sizeof(Graph));
                if (isGraph == NULL)
                {
                    fprintf(stderr, "Memory Allocation Failed\n");
                    return;
                }
                generate_online_kcolourable(isGraph, isOrder[isArbitrary], isSet[isIndex], 0.3);
                isColour = firstFitAlgorithm(isGraph);
                for (isIteration = 0; isIteration < isGraph->Order; isIteration++)
                {
                    (*(isColour + isIteration) > isRatio) && (isRatio = *(isColour + isIteration));
                }
                isRatio /= isGraph->graphSet;
                isCompetitiveRatio[isInstance] = isRatio;
                isMean += isRatio;
                free(isColour);
                isColour = NULL;
                isRatio = DBL_MIN;
                free(isGraph->Degree);
                isGraph->Degree = NULL;
                free(isGraph->VertexSet);
                isGraph->VertexSet = NULL;
                for (isIteration = 0; isIteration < isGraph->Order; isIteration++)
                {
                    free(*(isGraph->AdjacencyList + isIteration));
                    *(isGraph->AdjacencyList + isIteration) = NULL;
                    free(*(isGraph->AdjacencyMatrix + isIteration));
                    *(isGraph->AdjacencyMatrix + isIteration) = NULL;
                }
                free(isGraph->AdjacencyList);
                isGraph->AdjacencyList = NULL;
                free(isGraph->AdjacencyMatrix);
                isGraph->AdjacencyMatrix = NULL;
                for (isIteration = 0; isIteration < isGraph->graphSet; isIteration++)
                {
                    free(isGraph->DisjointSet[isIteration]->Vertex);
                    isGraph->DisjointSet[isIteration]->Vertex = NULL;
                    free(isGraph->DisjointSet[isIteration]);
                    isGraph->DisjointSet[isIteration] = NULL;
                }
                free(isGraph->DisjointSet);
                isGraph->DisjointSet = NULL;
                free(isGraph);
                isGraph = NULL;
            }
            isMean /= isGenerate;
            for (isInstance = 0; isInstance < isGenerate; isInstance++)
            {
                isStandardDeviation += pow(isCompetitiveRatio[isInstance] - isMean, 2);
            }
            isStandardDeviation /= (isGenerate - 1);
            isStandardDeviation = sqrt(isStandardDeviation);
            isInstance = 0;
            memset(isCompetitiveRatio, 0, sizeof(double) * isGenerate);
            fprintf(stdout, "Order = %d, Set = %d, Generate = %d, Competitive Ratio Mean = %.2lf, Competitive Ratio Standard Deviation = %.2lf\n", isOrder[isArbitrary], isSet[isIndex], isGenerate, isMean, isStandardDeviation);
            isMean = 0;
            isStandardDeviation = 0;
        }
    }
    fprintf(stdout, "\nColoring Based Interval Partition Algorithm\n");
    for (int isArbitrary = 0; isArbitrary < sizeof(isOrder) / sizeof(isOrder[0]); isArbitrary++)
    {
        for (; isInstance < isGenerate; isInstance++)
        {
            Graph *isGraph = calloc(1, sizeof(Graph));
            if (isGraph == NULL)
            {
                fprintf(stderr, "Memory Allocation Failed\n");
                return;
            }
            generate_online_kcolourable(isGraph, isOrder[isArbitrary], isSet[0], 0.3);
            isColour = coloringBasedIntervalPartitionAlgorithm(isGraph);
            for (isIteration = 0; isIteration < isGraph->Order; isIteration++)
            {
                (*(isColour + isIteration) > isRatio) && (isRatio = *(isColour + isIteration));
            }
            isRatio /= isGraph->graphSet;
            isCompetitiveRatio[isInstance] = isRatio;
            isMean += isRatio;
            free(isColour);
            isColour = NULL;
            isRatio = DBL_MIN;
            free(isGraph->Degree);
            isGraph->Degree = NULL;
            free(isGraph->VertexSet);
            isGraph->VertexSet = NULL;
            for (isIteration = 0; isIteration < isGraph->Order; isIteration++)
            {
                free(*(isGraph->AdjacencyList + isIteration));
                *(isGraph->AdjacencyList + isIteration) = NULL;
                free(*(isGraph->AdjacencyMatrix + isIteration));
                *(isGraph->AdjacencyMatrix + isIteration) = NULL;
            }
            free(isGraph->AdjacencyList);
            isGraph->AdjacencyList = NULL;
            free(isGraph->AdjacencyMatrix);
            isGraph->AdjacencyMatrix = NULL;
            for (isIteration = 0; isIteration < isGraph->graphSet; isIteration++)
            {
                free(isGraph->DisjointSet[isIteration]->Vertex);
                isGraph->DisjointSet[isIteration]->Vertex = NULL;
                free(isGraph->DisjointSet[isIteration]);
                isGraph->DisjointSet[isIteration] = NULL;
            }
            free(isGraph->DisjointSet);
            isGraph->DisjointSet = NULL;
            free(isGraph);
            isGraph = NULL;
        }
        isMean /= isGenerate;
        for (isInstance = 0; isInstance < isGenerate; isInstance++)
        {
            isStandardDeviation += pow(isCompetitiveRatio[isInstance] - isMean, 2);
        }
        isStandardDeviation /= (isGenerate - 1);
        isStandardDeviation = sqrt(isStandardDeviation);
        isInstance = 0;
        memset(isCompetitiveRatio, 0, sizeof(double) * isGenerate);
        fprintf(stdout, "Order = %d, Set = %d, Generate = %d, Competitive Ratio Mean = %.2lf, Competitive Ratio Standard Deviation = %.2lf\n", isOrder[isArbitrary], isSet[0], isGenerate, isMean, isStandardDeviation);
        isMean = 0;
        isStandardDeviation = 0;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // isSimulationResult();
    Graph *isGraph = calloc(1, sizeof(Graph));
    if (isGraph == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed\n");
        return 1;
    }
    generate_online_kcolourable(isGraph, 2000, 4, 0.3);
    int isFileDescriptor = open("Online_K_Colourable_Graph.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int isArbitrary = (int)(isGraph->Order * ceil((double)(isGraph->Order - 1) / 2)), isEntry = 0;
    size_t isCapacity = -1, isEdge = 0;
    ssize_t isWrite = 0;
    double isCompetitiveRatio = DBL_MIN;
    isWrite = snprintf(NULL, 0, "%d %d\n", isGraph->Order, isArbitrary);
    if (isWrite < 0)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    char *isString = calloc(++isWrite, sizeof(unsigned char));
    if (isString == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed.\n");
        return 1;
    }
    isCapacity = isWrite;
    isWrite = snprintf(NULL, 0, "%d %d", isGraph->Order, isArbitrary);
    if (isWrite < 0)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    isWrite = snprintf(isString, isCapacity, "%*s\n", isWrite, "");
    if (isWrite < 0)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    if (write(isFileDescriptor, isString, isWrite) == -1)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    for (; isEntry < isGraph->Order; isEntry++)
    {
        isEdge += *(isGraph->Degree + isEntry);
        for (isArbitrary = 0; isArbitrary < *(isGraph->Degree + isEntry); isArbitrary++)
        {
            isWrite = snprintf(isString, isCapacity, "%d %d\n", isEntry, *(*(isGraph->AdjacencyList + isEntry) + isArbitrary));
            if (isWrite < 0)
            {
                fprintf(stderr, "%s.\n", strerror(errno));
            }
            if (write(isFileDescriptor, isString, isWrite) == -1)
            {
                fprintf(stderr, "%s.\n", strerror(errno));
                return 1;
            }
        }
    }
    if (lseek(isFileDescriptor, 0, SEEK_SET) == -1)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    isWrite = snprintf(isString, isCapacity, "%d %zu\n", isGraph->Order, isEdge);
    if (isWrite < 0)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    if (write(isFileDescriptor, isString, isWrite) == -1)
    {
        fprintf(stderr, "%s.\n", strerror(errno));
        return 1;
    }
    close(isFileDescriptor);
    // displayAdjacencyList(isGraph);
    // displaySet(isGraph);
    int *isColour = NULL;
    fprintf(stdout, "\nFirst Fit Algorithm");
    isColour = firstFitAlgorithm(isGraph);
    for (isArbitrary = 0; isArbitrary < isGraph->Order; isArbitrary++)
    {
        (*(isColour + isArbitrary) > isCompetitiveRatio) && (isCompetitiveRatio = *(isColour + isArbitrary));
    }
    fprintf(stdout, "\nChroma = %.0lf, ", isCompetitiveRatio);
    isCompetitiveRatio /= isGraph->graphSet;
    fprintf(stdout, "Competitive Ratio: %.2lf\n", isCompetitiveRatio);
    free(isColour);
    isColour = NULL;
    isCompetitiveRatio = DBL_MIN;
    fprintf(stdout, "\nFirst Fit Algorithm (Alternative)");
    isColour = firstFitAlgorithm_(isGraph);
    for (isArbitrary = 0; isArbitrary < isGraph->Order; isArbitrary++)
    {
        (*(isColour + isArbitrary) > isCompetitiveRatio) && (isCompetitiveRatio = *(isColour + isArbitrary));
    }
    fprintf(stdout, "\nChroma = %.0lf, ", isCompetitiveRatio);
    isCompetitiveRatio /= isGraph->graphSet;
    fprintf(stdout, "Competitive Ratio: %.2lf\n", isCompetitiveRatio);
    free(isColour);
    isColour = NULL;
    isCompetitiveRatio = DBL_MIN;
    fprintf(stdout, "\nColoring Based Interval Partition Algorithm");
    isColour = coloringBasedIntervalPartitionAlgorithm(isGraph);
    for (isArbitrary = 0; isArbitrary < isGraph->Order; isArbitrary++)
    {
        (*(isColour + isArbitrary) > isCompetitiveRatio) && (isCompetitiveRatio = *(isColour + isArbitrary));
    }
    fprintf(stdout, "\nChroma = %.0lf, ", isCompetitiveRatio);
    isCompetitiveRatio /= isGraph->graphSet;
    fprintf(stdout, "Competitive Ratio: %.2lf\n", isCompetitiveRatio);
    free(isColour);
    isColour = NULL;
    free(isGraph->Degree);
    isGraph->Degree = NULL;
    free(isGraph->VertexSet);
    isGraph->VertexSet = NULL;
    for (isArbitrary = 0; isArbitrary < isGraph->Order; isArbitrary++)
    {
        free(*(isGraph->AdjacencyList + isArbitrary));
        *(isGraph->AdjacencyList + isArbitrary) = NULL;
        free(*(isGraph->AdjacencyMatrix + isArbitrary));
        *(isGraph->AdjacencyMatrix + isArbitrary) = NULL;
    }
    free(isGraph->AdjacencyList);
    isGraph->AdjacencyList = NULL;
    free(isGraph->AdjacencyMatrix);
    isGraph->AdjacencyMatrix = NULL;
    for (isArbitrary = 0; isArbitrary < isGraph->graphSet; isArbitrary++)
    {
        free(isGraph->DisjointSet[isArbitrary]->Vertex);
        isGraph->DisjointSet[isArbitrary]->Vertex = NULL;
        free(isGraph->DisjointSet[isArbitrary]);
        isGraph->DisjointSet[isArbitrary] = NULL;
    }
    free(isGraph->DisjointSet);
    isGraph->DisjointSet = NULL;
    free(isGraph);
    isGraph = NULL;
    return 0;
}