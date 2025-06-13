#include "bellman.h"

/**
 * Find the index of a target vertex in the set of sorted vertices
 *
 * @param targetVertex The vertex whose index to be found
 * @param setOfSortedVertices The set of all sorted vertices
 * @return The index of targetVertex or -1 if not found
 */
int getVertexIndex(const int targetVertex, const vector<int>& sortedVertices) {
    for (int i = 0; i < static_cast<int>(sortedVertices.size()); ++i) {
        if (sortedVertices[i] == targetVertex) return i;
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
vector<int> getSortedVertices(int graph[][3], int numEdges) {
    vector<int> vertices;
    for (int i = 0; i < numEdges; ++i) {
        vertices.push_back(graph[i][0]);
        vertices.push_back(graph[i][1]);
    }
    sort(vertices.begin(), vertices.end());
    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());
    return vertices;
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
    if (numEdges <= 0) return;
    vector<int> sortedVertices = getSortedVertices(graph, numEdges);

    int startVertexIdx = getVertexIndex(startVertex, sortedVertices);
    BFValue[startVertexIdx] = 0;

    for (int i = 0; i < numEdges; ++i) {
        int u = graph[i][0];
        int v = graph[i][1];
        int weight = graph[i][2];

        int u_idx = getVertexIndex(u, sortedVertices);
        int v_idx = getVertexIndex(v, sortedVertices);

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
    for (int i = 0; i < n; ++i) cout << BFValue[i] << ", ";
    cout << endl;
}

void printBFPrev(int BFPrev[], int n) {
    cout << "BFPrev: ";
    for (int i = 0; i < n; ++i) cout << BFPrev[i] << ", ";
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
    if (numEdges <= 0) return "";
    if (startVertex == goalVertex) return string(1, startVertex);

    vector<int> sortedVertices = getSortedVertices(graph, numEdges);
    int numVertices = sortedVertices.size();

    int* BFValue = new int[numVertices];
    int* BFPrev = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    int startVertexIdx = getVertexIndex(startVertex, sortedVertices);
    BFValue[startVertexIdx] = 0;

    for (int i = 0; i < numVertices - 1; ++i) 
        BF(graph, numEdges, startVertex, BFValue, BFPrev);

    string bf_path = "";
    char currVertex = goalVertex;
    while (currVertex != startVertex) {
        bf_path += currVertex;
        bf_path += ' ';
        currVertex = BFPrev[getVertexIndex(currVertex, sortedVertices)];
    }
    bf_path += startVertex;
    reverse(bf_path.begin(), bf_path.end());

    delete[] BFValue;
    delete[] BFPrev;

    return bf_path;
}
