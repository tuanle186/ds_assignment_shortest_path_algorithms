#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "bellman.h"
#include "tsm.h"
#include "set"
using namespace std;
#define MAX 1000 // maximum number of edges

void printEdge(int[]); // Function to print an edge
void printEdgeList(int edgeList[MAX][3], int numEdges);
void readFromFile(string fileName, int edgeList[MAX][3], int& numEdges, int& numVertices);

void testCaseBF1(int edgeList[MAX][3], int numEdges, int numVertices);
void testCaseBFPath1(int edgeList[MAX][3], int numEdges);
void testCaseBFPath2(int edgeList[MAX][3], int numEdges);
void testCaseTraveling1();
void testCaseTraveling2(int graph[][3], int numEdges);

/*
int main() {
    string input_file_name = "./edgeLists/input10A.txt";

    int edgeList[MAX][3], numEdges, numVertices;
    readFromFile(input_file_name, edgeList, numEdges, numVertices);

    // printEdgeList(edgeList, numEdges);
    cout << "Number of edges: " << numEdges << endl;
    cout << "Number of vertices: " << numVertices << endl;

    cout << "\ntestcaseBF1: ============================\n";
    testCaseBF1(edgeList, numEdges, numVertices);

    // cout << "\ntestcaseBFPath1: ============================\n";
    // testCaseBFPath1(edgeList, numEdges);

    // cout << "\ntestcaseBFPath2: ============================\n";
    // testCaseBFPath2(edgeList, numEdges);

    // cout << "\ntestcaseTraveling1: ============================\n";
    // testCaseTraveling1();

    // cout << "\ntestcaseTraveling2: ============================\n";
    // testCaseTraveling2(edgeList, numEdges);
    return 0;
}
    */

int main() {
    int edgeList[70][3];
    int numEdges=70;
    int Esize=10;
    ifstream fin("./edgeLists/input10A.txt");
    for (int i = 0; i < numEdges; i++){
        fin >> edgeList[i][0] >> edgeList[i][1] >> edgeList[i][2];
    }
    fin.close();

    char start_vertices;
    char namelist[] = "?b";

    for (int i = 0; i < 2; i++) {
        start_vertices = namelist[i];
        int BFValue[Esize];
        int BFPrev[Esize];
        for (int k = 0; k < Esize; k++){
            BFValue[k] = -1;
            BFPrev[k] = -1;
        }
        cout << "#####" <<endl;
        
        for (int j = 0; j < 2; j++){
            cout << "step:" << j + 1 <<endl;
            BF(edgeList, numEdges, start_vertices, BFValue, BFPrev);
            for (int k = 0; k < Esize; k++) {
                cout << BFValue[k] << " ";
            }
            cout << endl;
            for (int k = 0; k < Esize; k++) {
                cout << BFPrev[k] << " ";
            }
            cout << endl;
        }
    }
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
    
    for (int i = 0; i < numVertices - 1; i++) BF(edgeList, numEdges, /*startVertex=*/'?', BFValue, BFPrev);

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
// ---------------- BF_Path() testcases-------------------
// -------------------------------------------------------

void testCaseTraveling1() {
    // Example 1: A simple triangle A-B-C
    // A=65, B=66, C=67 (ASCII values)
    int graph1[][3] = {
        {65, 66, 10}, // A->B, weight 10
        {66, 67, 20}, // B->C, weight 20
        {67, 65, 30}  // C->A, weight 30
    };
    int numEdges1 = 3;
    char startVertex1 = 'A';
    Traveling(graph1, numEdges1, startVertex1); 
    // Expected: A -> B -> C -> A, Cost: 10+20+30 = 60 (or A->C->B->A if that's shorter - no, this is fixed)
}

void testCaseTraveling2(int graph[][3], int numEdges) {
    Traveling(graph, numEdges, 'M');
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
