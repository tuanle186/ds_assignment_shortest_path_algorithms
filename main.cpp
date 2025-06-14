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

// -------------------------------------------------------
// ------------------- BF() testcases---------------------
// -------------------------------------------------------

void testCaseBF1(int edgeList[MAX][3], int numEdges, int numVertices) {
    int* BFValue = new int[numVertices];
    int* BFPrev = new int[numVertices];
    fill(BFValue, BFValue + numVertices, -1);
    fill(BFPrev, BFPrev + numVertices, -1);

    for (int i = 0; i < numVertices - 1; ++i)
        BF(edgeList, numEdges, '?', BFValue, BFPrev);

    cout << "BFValue = ";
    for (int i = 0; i < numVertices; ++i) cout << BFValue[i] << ", ";

    cout << "\nBFPrev = ";
    for (int i = 0; i < numVertices; ++i) cout << BFPrev[i] << ", ";
    cout << endl;

    delete[] BFValue;
    delete[] BFPrev;
}

void testCaseBF2() {
    int edgeList[70][3];
    int numEdges = 70;
    int Esize = 10;

    ifstream fin("./edgeLists/input10A.txt");
    for (int i = 0; i < numEdges; ++i)
        fin >> edgeList[i][0] >> edgeList[i][1] >> edgeList[i][2];
    fin.close();

    char namelist[] = "?b";

    for (int i = 0; i < 2; ++i) {
        char start_vertex = namelist[i];
        int BFValue[Esize], BFPrev[Esize];
        fill(BFValue, BFValue + Esize, -1);
        fill(BFPrev, BFPrev + Esize, -1);

        cout << "##### Start Vertex: " << start_vertex << " #####\n";

        for (int j = 0; j < 2; ++j) {
            cout << "Step: " << j + 1 << endl;
            BF(edgeList, numEdges, start_vertex, BFValue, BFPrev);

            for (int v : BFValue) cout << v << " ";
            cout << "\n";

            for (int p : BFPrev) cout << p << " ";
            cout << "\n";
        }
    }
}

// -------------------------------------------------------
// ---------------- BF_Path() testcases-------------------
// -------------------------------------------------------

void testCaseBFPath1(int edgeList[MAX][3], int numEdges) {
    string bf_path = BF_Path(edgeList, numEdges, 81, 118);
    cout << "bf_path = " << bf_path << "\nbf_path_dec_ver = ";
    for (size_t i = 0; i < bf_path.length(); ++i)
        if (bf_path[i] != ' ')
            cout << int(bf_path[i]) << (i != bf_path.length() - 1 ? " -> " : "");
    cout << endl;
}

void testCaseBFPath2(int edgeList[MAX][3], int numEdges) {
    string bf_path = BF_Path(edgeList, numEdges, 81, 81);
    cout << "bf_path = " << bf_path << "\nbf_path_dec_ver = ";
    for (size_t i = 0; i < bf_path.length(); ++i)
        if (bf_path[i] != ' ')
            cout << int(bf_path[i]) << (i != bf_path.length() - 1 ? " -> " : "");
    cout << endl;
}

void testCaseBFPath3() {
    int edgeList[70][3];
    int numEdges = 70;

    ifstream fin("./edgeLists/input10A.txt");
    for (int i = 0; i < numEdges; ++i)
        fin >> edgeList[i][0] >> edgeList[i][1] >> edgeList[i][2];
    fin.close();

    char startlist[] = "?b";
    char endlist[] = "5]";

    for (int i = 0; i < 2; ++i) {
        char start_vertex = startlist[i];
        char end_vertex = endlist[i];

        string out = BF_Path(edgeList, numEdges, start_vertex, end_vertex);
        cout << "### From " << start_vertex << " to " << end_vertex << " ###\n" << out << endl;
    }
}

// -------------------------------------------------------
// ------------- Traveling Salesman testcases ------------
// -------------------------------------------------------

void testCaseTraveling1(int graph[][3], int numEdges) {
    Traveling(graph, numEdges, 63);
}

void testCaseTraveling2() {
    int graph1[][3] = {
        {65, 66, 10},
        {66, 67, 20},
        {67, 65, 30}
    };
    int numEdges1 = 3;
    Traveling(graph1, numEdges1, 'A');
}

void testCaseTraveling3(int graph[][3], int numEdges) {
    Traveling(graph, numEdges, 'M');
}

// -------------------------------------------------------
// ------------------------ Utils ------------------------ 
// -------------------------------------------------------

void readFromFile(const string& fileName, int edgeList[MAX][3], int& numEdges, int& numVertices) {
    numEdges = 0;
    set<int> vertices;
    ifstream fin(fileName);

    int src, dst, w;
    while (fin >> src >> dst >> w) {
        edgeList[numEdges][0] = src;
        edgeList[numEdges][1] = dst;
        edgeList[numEdges][2] = w;
        vertices.insert(src);
        vertices.insert(dst);
        ++numEdges;
    }
    fin.close();
    numVertices = vertices.size();
}

void printEdge(int edge[]) {
    cout << edge[0] << ", " << edge[1] << ", " << edge[2] << endl;
}

void printEdgeList(int edgeList[MAX][3], int numEdges) {
    for (int i = 0; i < numEdges; ++i)
        printEdge(edgeList[i]);
}

// -------------------------------------------------------
// ------------------- main function --------------------- 
// -------------------------------------------------------

int main() {
    const string input_file_1 = "./edgeLists/input10A.txt";
    int edgeList1[MAX][3], numEdges1, numVertices1;
    readFromFile(input_file_1, edgeList1, numEdges1, numVertices1);

    cout << "====================================================\n";
    cout << "Input File 1: " << input_file_1 << "\n";
    cout << "Number of Edges   : " << numEdges1 << "\n";
    cout << "Number of Vertices: " << numVertices1 << "\n";
    cout << "====================================================\n\n";

    cout << "[TEST] Bellman-Ford Basic\n";
    cout << "----------------------------------------------------\n";
    testCaseBF1(edgeList1, numEdges1, numVertices1);
    cout << "\n";

    cout << "[TEST] BF_Path From 81 to 118\n";
    cout << "----------------------------------------------------\n";
    testCaseBFPath1(edgeList1, numEdges1);
    cout << "\n";

    cout << "[TEST] BF_Path From 81 to 81\n";
    cout << "----------------------------------------------------\n";
    testCaseBFPath2(edgeList1, numEdges1);
    cout << "\n";

    cout << "[TEST] Bellman-Ford Step LMS\n";
    cout << "----------------------------------------------------\n";
    testCaseBF2();
    cout << "\n";

    cout << "[TEST] BF_Path Character Pair LMS\n";
    cout << "----------------------------------------------------\n";
    testCaseBFPath3();
    cout << "\n";

    cout << "[TEST] Traveling Salesman Example 1 (LMS)\n";
    cout << "----------------------------------------------------\n";
    testCaseTraveling1(edgeList1, numEdges1);
    cout << "\n";

    const string input_file_2 = "./edgeLists/EdgeList4.txt";
    int edgeList2[MAX][3], numEdges2, numVertices2;
    readFromFile(input_file_2, edgeList2, numEdges2, numVertices2);

    cout << "====================================================\n";
    cout << "Input File 2: " << input_file_2 << "\n";
    cout << "Number of Edges   : " << numEdges2 << "\n";
    cout << "Number of Vertices: " << numVertices2 << "\n";
    cout << "====================================================\n\n";

    cout << "[TEST] Traveling Salesman Example 2 (Simple)\n";
    cout << "----------------------------------------------------\n";
    testCaseTraveling2();
    cout << "\n";

    cout << "[TEST] Traveling Salesman Example 3 (22 vertices)\n";
    cout << "----------------------------------------------------\n";
    testCaseTraveling3(edgeList2, numEdges2);
    cout << "\n";

    cout << "================== END OF TESTS ====================\n";
    return 0;
}
