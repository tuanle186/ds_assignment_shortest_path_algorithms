#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include "bellman.h"
using namespace std;
#define MAX 1000 // maximum number of edges

void printEdge(int[]); // Function to print an edge
void printEdgeList(int edgeList[MAX][3], int numEdges);
void readFromFile(string fileName, int edgeList[MAX][3], int& numEdges, int& numVertices);

void testCaseBF1(int edgeList[MAX][3], int numEdges, int numVertices);
void testCaseBFPath1(int edgeList[MAX][3], int numEdges);
void testCaseBFPath2(int edgeList[MAX][3], int numEdges);


int main() {
    string input_file_name = "./edgeLists/EdgeList3.txt";

    int edgeList[MAX][3], numEdges, numVertices;
    readFromFile(input_file_name, edgeList, numEdges, numVertices);

    // printEdgeList(edgeList, numEdges);
    cout << "Number of edges: " << numEdges << endl;
    cout << "Number of vertices: " << numVertices << endl;

    cout << "\ntestcaseBF1: ============================\n";
    testCaseBF1(edgeList, numEdges, numVertices);

    cout << "\ntestcaseBFPath1: ============================\n";
    testCaseBFPath1(edgeList, numEdges);

    cout << "\ntestcaseBFPath2: ============================\n";
    testCaseBFPath2(edgeList, numEdges);
    return 0;
}


// -------------------------------------------------------
// ------------------- BF() testcases---------------------
// -------------------------------------------------------

void testCaseBF1(int edgeList[MAX][3], int numEdges, int numVertices) {
    // Initialize BFValue and BFPrev with all -1
    int* BFValue = new int[numVertices];
    int* BFPrev = new int[numVertices];
    for (int i = 0; i < numVertices; i++) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }
    
    for (int i = 0; i < numVertices - 1; i++) BF(edgeList, numEdges, /*startVertex=*/81, BFValue, BFPrev);

    cout << "BFValue = ";
    for (int i = 0; i < numVertices; i++) cout << BFValue[i] << ", ";
    cout << "\nBFPrev = ";
    for (int i = 0; i < numVertices; i++) cout << BFPrev[i] << ", ";
}


// -------------------------------------------------------
// ---------------- BF_Path() testcases-------------------
// -------------------------------------------------------

void testCaseBFPath1(int edgeList[MAX][3], int numEdges) {
    string bf_path = BF_Path(edgeList, numEdges, 81, 118);
    cout << "bf_path = " << bf_path << endl << "bf_path_dec_ver = ";
    for (int i = 0; i < bf_path.length(); i++) {
        if (bf_path[i] != ' ') {
            if (i != bf_path.length() - 1) cout << int(bf_path[i]) << " -> ";
            else cout << int(bf_path[i]);
        }
    }
}

void testCaseBFPath2(int edgeList[MAX][3], int numEdges) {
    string bf_path = BF_Path(edgeList, numEdges, 81, 81);
    cout << "bf_path = " << bf_path << endl << "bf_path_dec_ver = ";
    for (int i = 0; i < bf_path.length(); i++) {
        if (bf_path[i] != ' ') {
            if (i != bf_path.length() - 1) cout << int(bf_path[i]) << " -> ";
            else cout << int(bf_path[i]);
        }
    }
}


// -------------------------------------------------------
// ------------------------ Utils ------------------------ 
// -------------------------------------------------------

void readFromFile(string fileName, int edgeList[MAX][3], int& numEdges, int& numVertices) {
    numEdges = 0;
    set<int> vertices;
    int src_vertex, des_vertex, weight;
    ifstream fin(fileName);
    while (fin >> src_vertex >> des_vertex >> weight) {
        edgeList[numEdges][0] = src_vertex;
        edgeList[numEdges][1] = des_vertex;
        edgeList[numEdges][2] = weight;
        vertices.insert(src_vertex);
        vertices.insert(des_vertex);
        numEdges++;
    }
    fin.close();
    numVertices = vertices.size();
}

void printEdge(int edge[]){
    cout << (edge[0]) << ", " << (edge[1]) << ", " << edge[2] << endl;
}

void printEdgeList(int edgeList[MAX][3], int numEdges) {
    for (int i = 0; i < numEdges; i++) {
        printEdge(edgeList[i]);
    }
}
