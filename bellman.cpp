#include "bellman.h"

/**
 * Find the index of a target vertex in the set of sorted vertices
 *
 * @param targetVertex The vertex whose index to be found
 * @param setOfSortedVertices The set of all sorted vertices
 * @return The index of targetVertex or -1 if not found
 */
int getVertexIndex(const int targetVertex, const set<int>& setOfSortedVertices) {
    if (setOfSortedVertices.empty()) return -1;
    int targetVertexIdx = 0;
    for (auto vertex : setOfSortedVertices) {
        // Vertex is unique
        if (vertex == targetVertex) return targetVertexIdx;
        targetVertexIdx++;
    }
    return -1;
}

/**
 * Calculate the sorted set of vertices from an input graph
 *
 * @param graph The input graph
 * @param numEdges The number of edges in the graph
 * @return The set of sorted vertices
 */
set<int> getSetOfSortedVertices(int graph[][3], int numEdges) {
    // Extract unique values, set sorts the elements in ascending order by default
    set<int> setOfSortedVertices;
    for (int i = 0; i < numEdges; i++) {
        setOfSortedVertices.insert(graph[i][0]);
        setOfSortedVertices.insert(graph[i][1]);
    }
    return setOfSortedVertices;
}


/**
 * Calculate 1 step of Bellman-Ford on an input graph
 *
 * @param graph The input graph
 * @param numEdges The number of edges in the graph
 * @param startVertex The start vertex of the Bellman-Ford algorithm
 * @param BFValue Current Bellman-Ford Value array (to be updated)
 * @param BFPrev Current Bellman-Ford Previous array (to be updated)
 */
void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]) {
    if (numEdges <= 0) return; // No edges to process
    set<int> setOfSortedVertices = getSetOfSortedVertices(graph, numEdges);

    // Make sure the distance from the source vertex to itself is always 0
    int startVertexIdx = getVertexIndex(startVertex, setOfSortedVertices);
    BFValue[startVertexIdx] = 0;

    // Start relaxing the edges
    for (int i = 0; i < numEdges; i++) {
        int u = graph[i][0];
        int v = graph[i][1];
        int weight = graph[i][2];

        int u_idx = getVertexIndex(u, setOfSortedVertices);
        int v_idx = getVertexIndex(v, setOfSortedVertices);

        if (BFValue[u_idx] != -1) {
            if ((BFValue[u_idx] + weight < BFValue[v_idx]) || (BFValue[v_idx] == -1)) {
                BFValue[v_idx] = BFValue[u_idx] + weight;
                BFPrev[v_idx] = u;
            }
        }
    }
}


#if DEBUGGING
void printBFValue(int BFValue[], int n) {
    cout << "BFValue: ";
    for (int i = 0; i < n; i++) cout << BFValue[i] << ", ";
    cout << endl;
}

void printBFPrev(int BFPrev[], int n) {
    cout << "BFPrev: ";
    for (int i = 0; i < n; i++) cout << BFPrev[i] << ", ";
    cout << endl;
}
#endif


/**
 * Return a string containing the BellmanFord path from input start and goal vertices 
 *
 * @param graph The input graph
 * @param numEdges The number of edges in the graph
 * @param startVertex The start vertex of the path
 * @param goalVertex The goal vertex of the path
 * @return A string containing the shortest path from Start vertex to Goal vertex
 */
string BF_Path(int graph[][3], int numEdges, char startVertex, char goalVertex) {
    if (numEdges <= 0) return ""; // No edges to process
    else if (startVertex == goalVertex) return string(1, startVertex);

    set<int> setOfSortedVertices = getSetOfSortedVertices(graph, numEdges);
    int numVertices = setOfSortedVertices.size();

    // Initialize and fill BFValue and BFPrev with all -1s
    int* BFValue = new int[numVertices];
    int* BFPrev = new int[numVertices];
    for (int i = 0; i < numVertices; i++) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    int startVertexIdx = getVertexIndex(startVertex, setOfSortedVertices);
    BFValue[startVertexIdx] = 0;

    // The input weights of the testcases will all be positive => no need to check for negative cycles
    for (int i = 0; i < numVertices - 1; i++) 
        BF(graph, numEdges, startVertex, BFValue, BFPrev);

    /*
    printBFValue(BFValue, numVertices);
    printBFPrev(BFPrev, numVertices);
    */

    // Start constructing BF path
    // The testcases will always have valid path => no need to check for no path case
    string bf_path = "";
    char currVertex = goalVertex;
    while (currVertex != startVertex) {
        bf_path += static_cast<char>(currVertex);
        bf_path += ' ';
        currVertex = BFPrev[getVertexIndex(currVertex, setOfSortedVertices)];
    }
    bf_path += static_cast<char>(startVertex);
    reverse(bf_path.begin(), bf_path.end());

    // Clean up dynamically allocated memory
    delete[] BFValue;
    delete[] BFPrev;

    return bf_path;
}