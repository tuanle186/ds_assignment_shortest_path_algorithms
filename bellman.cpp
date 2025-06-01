#include "bellman.h"


int getVertexIndex(const int targetVertex, const set<int> listOfSortedVertices) {
    if (listOfSortedVertices.empty()) return -1;
    int targetVertexIdx = 0;
    for (auto vertex : listOfSortedVertices) {
        // Vertex is unique
        if (vertex == targetVertex) return targetVertexIdx;
        targetVertexIdx++;
    }
    return -1;
}


set<int> getSetOfSortedVertices(int graph[][3], int numEdges) {
    // Extract unique values, set sorts the elements in ascending order by default
    set<int> setOfSortedVertices;
    for (int i = 0; i < numEdges; i++) {
        setOfSortedVertices.insert(graph[i][0]);
        setOfSortedVertices.insert(graph[i][1]);
    }
    return setOfSortedVertices;
}


/* Perform one step of Bellman-Ford
*/
void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]) {
    if (numEdges <= 0) return; // No edges to process
   
    set<int> setOfSortedVertices = getSetOfSortedVertices(graph, numEdges);
    
    // Make sure the label of the startVertex is 0
    BFValue[getVertexIndex(startVertex, setOfSortedVertices)] = 0;

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

    // The input weights of the testcases will all be positive => no need to check for negative cycles
    for (int i = 0; i < numVertices - 1; i++) 
        BF(graph, numEdges, startVertex, BFValue, BFPrev);

    // Start constructing BF path
    string bf_path = "";
    int currVertex = goalVertex;
    while (currVertex != startVertex) {
        bf_path += currVertex;
        bf_path += ' ';
        currVertex = BFPrev[getVertexIndex(currVertex, setOfSortedVertices)];
    }
    bf_path += startVertex;
    reverse(bf_path.begin(), bf_path.end());

    // Clean up dynamically allocated memory
    delete[] BFValue;
    delete[] BFPrev;

    return bf_path;
}