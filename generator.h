//
// Created by alyss on 24/11/2025.
//

#ifndef COMP6651_GENERATOR_H
#define COMP6651_GENERATOR_H

#include "graph.h"

long randomGenerator_Unbiased(int isMax);
void generate_online_kcolourable(Graph *,int n, int k, double p);

#endif //COMP6651_GENERATOR_H