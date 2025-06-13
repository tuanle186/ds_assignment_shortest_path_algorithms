#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

#define DEBUGGING 1

void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]);
string BF_Path(int graph[][3], int numEdges, char startVertex, char goalVertex);
int getVertexIndex(const int targetVertex, const vector<int>& sortedVertices);
vector<int> getSortedVertices(int graph[][3], int numEdges);

#endif