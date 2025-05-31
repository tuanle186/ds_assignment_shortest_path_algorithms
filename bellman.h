#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <string.h>
using namespace std;

void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]);
string BF_Path(int graph[][3], int numEdges, char startVertex, int goalVertex);

#endif