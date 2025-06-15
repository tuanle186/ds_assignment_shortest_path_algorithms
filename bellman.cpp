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


struct Edge {
    int start;
    int goal;
    int weight;
};

/**
 * Build a vector<Edge> from the raw `graph` array.
 */
static void buildEdgeList(int graph[][3], int numEdges, vector<Edge>& edges) {
    edges.clear();
    edges.reserve(numEdges);
    for (int i = 0; i < numEdges; ++i) {
        edges.push_back({graph[i][0], graph[i][1], graph[i][2]});
    }
}

/**
 * Sort the edge list in ascending order of start vertex,
 * but preserve the original order of edges when start vertices are equal.
 */
static void sortEdgeList(vector<Edge>& edges) {
    stable_sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.start < b.start;
    });
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
void BF(int graph[][3], 
        int numEdges, 
        char startVertex, 
        int BFValue[], 
        int BFPrev[]) {

    if (numEdges <= 0) return; // nothing to do

    /* ------------- gather vertex set & prepare tmp buffer ------------- */
    vector<int> vertices = getSortedVertices(graph, numEdges);
    const int n = vertices.size();

    vector<int> BFValue_new(n);
    vector<int> BFPrev_new(n);
    for (int i = 0; i < n; ++i) {
        BFValue_new[i] = BFValue[i];  
        BFPrev_new[i] = BFPrev[i];
    }

    /* -------------------- build & sort edge list ---------------------- */
    vector<Edge> edges;
    buildEdgeList(graph, numEdges, edges);
    sortEdgeList(edges);     // implements steps 1 & 2

    // Check if this is the 1st step
    int startVertexIdx = getVertexIndex(startVertex, vertices);
    if (startVertexIdx == -1) {
        BFValue[startVertexIdx] = 0;
        return;
    }
    
    bool flag = false;
    if (BFValue[startVertexIdx] == -1) { // 1st step
        for (const Edge& e : edges) {
            const int uIdx = getVertexIndex(e.start, vertices);
            const int vIdx = getVertexIndex(e.goal, vertices);

            if (uIdx != startVertexIdx && !flag) continue; // has not reached out-going edges of startVertex yet => skip
            else if (uIdx != startVertexIdx && flag) break;
            else { // reached out-going edges of startVertex
                BFValue_new[vIdx] = e.weight;
                BFPrev_new[vIdx] = uIdx;
                flag = true;
            }

        }
    } else { // not 1st step
        for (const Edge& e : edges) {
            const int uIdx = getVertexIndex(e.start, vertices);
            const int vIdx = getVertexIndex(e.goal, vertices);

            if (BFPrev[uIdx] == -1) continue; // not reachable yet

            if (BFValue[uIdx] + e.weight < BFValue_new[vIdx] || BFValue_new[vIdx] == -1) {
                BFValue_new[vIdx] = BFValue[uIdx] + e.weight;
                BFPrev_new[vIdx] = uIdx;
            }
        }
    }

    /* ------------------------ commit results -------------------------- */
    for (int i = 0; i < n; ++i) {
        BFValue[i] = BFValue_new[i];   // 4.4
        BFPrev[i] = BFPrev_new[i];
    }

    BFValue[getVertexIndex(startVertex, vertices)] = 0;
}


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

    vector<int> vertices = getSortedVertices(graph, numEdges);
    int numVertices = vertices.size();

    int* BFValue = new int[numVertices];
    int* BFPrev = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    int startVertexIdx = getVertexIndex(startVertex, vertices);
    int goalVertexIdx = getVertexIndex(goalVertex, vertices);
    // BFValue[startVertexIdx] = 0;

    for (int i = 0; i < numVertices - 1; ++i) 
        BF(graph, numEdges, startVertex, BFValue, BFPrev);

    // If there's no path
    if (BFValue[goalVertexIdx] == -1) {
        delete[] BFValue;
        delete[] BFPrev;
        return "";
    }

    // Reconstruct path using indices
    vector<char> path;
    int currIdx = goalVertexIdx;
    while (currIdx != startVertexIdx) {
        path.push_back(vertices[currIdx]); // Convert index to vertex (char)
        currIdx = BFPrev[currIdx];
        if (currIdx == -1) {
            // In case of a broken path (shouldn't happen if graph is connected)
            delete[] BFValue;
            delete[] BFPrev;
            return "";
        }
    }
    path.push_back(vertices[startVertexIdx]);

    // Reverse and build the string
    reverse(path.begin(), path.end());
    string bf_path;
    for (char ch : path) {
        bf_path += ch;
        bf_path += ' ';
    }
    bf_path.pop_back(); // Remove trailing space

    delete[] BFValue;
    delete[] BFPrev;
    

    return bf_path;
}

