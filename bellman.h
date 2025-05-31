#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
using namespace std;

#define DEBUGGING 1

void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]);
string BF_Path(int graph[][3], int numEdges, char startVertex, char goalVertex);

#endif