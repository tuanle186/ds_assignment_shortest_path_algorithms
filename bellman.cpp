#include "bellman.h"

void bubbleSort(int arr[], int n) {
    if (arr == nullptr || n <= 1) return;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


int getVertexIndex(int vertex, const int sorted_unique_vertices[], int num_unique) {
    if (sorted_unique_vertices == nullptr) {
        return -1;
    }

    // Binary Search
    int low = 0;
    int high = num_unique - 1;

    while (low <= high) {
        int mid = low + (high - low)/2;

        // Check if vertex is present at mid
        if (sorted_unique_vertices[mid] == vertex) return mid;

        // If vertex is greater than mid, ignore the left half
        if (sorted_unique_vertices[mid] < vertex) low = mid + 1;

        // If vertex is less than mid, ignore the right half
        else high = mid - 1;
    }

    return -1; // Vertex is not found
}


void getSortedUniqueVertices(int graph[][3], int numEdges, int& numUniqueVertices, int*& sortedUniqueVertices) {
    if (numEdges <= 0) {
        return;
    }

    // 1. Collect all vertices from the graph, 
    // maximum number of vertices is 2 * numEdges
    int* all_vertices_not_filtered = new int[2 * numEdges];
    if (all_vertices_not_filtered == nullptr) {
        // Failed to allocate memory
        return; 
    }

    int vertices_count_not_filtered = 0;
    for (int i = 0; i < numEdges; i++) {
        all_vertices_not_filtered[vertices_count_not_filtered++] = graph[i][0];
        all_vertices_not_filtered[vertices_count_not_filtered++] = graph[i][1];
    }

    // 2. Sort all collected vertices
    bubbleSort(all_vertices_not_filtered, vertices_count_not_filtered);
    
    // 3. Extract unique sorted labels
    // The array of unique vertices can't be larger than all_vertices
    sortedUniqueVertices = new int[vertices_count_not_filtered];
    if (sortedUniqueVertices == nullptr) {
        delete[] all_vertices_not_filtered; // Clean up previously allocated memory
        return; // failed to allocate memory
    }

    if (vertices_count_not_filtered > 0) {
        sortedUniqueVertices[numUniqueVertices++] = all_vertices_not_filtered[0];
        for (int i = 1; i < vertices_count_not_filtered; i++) {
            if (all_vertices_not_filtered[i] != all_vertices_not_filtered[i - 1]) {
                sortedUniqueVertices[numUniqueVertices++] = all_vertices_not_filtered[i];
            }
        }
    }

    delete[] all_vertices_not_filtered; // Clean up previously allocated memory
}


void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]) {
    if (numEdges <= 0) {
        return; // No edges to process
    }

    int* sortedUniqueVertices;
    int numUniqueVertices = 0;
    getSortedUniqueVertices(graph, numEdges, numUniqueVertices, sortedUniqueVertices);
    
    // Make sure the label of the startVertex is 0
    int startVertexIndex = getVertexIndex(startVertex, sortedUniqueVertices, numUniqueVertices);
    BFValue[startVertexIndex] = 0;

    // Start relaxing the edges
    for (int i = 0; i < numEdges; i++) {
        int u = graph[i][0];
        int v = graph[i][1];
        int weight = graph[i][2];

        int u_idx = getVertexIndex(u, sortedUniqueVertices, numUniqueVertices);
        int v_idx = getVertexIndex(v, sortedUniqueVertices, numUniqueVertices);

        if (BFValue[u_idx] != -1) {
            if ((BFValue[u_idx] + weight < BFValue[v_idx]) || (BFValue[v_idx] == -1)) {
                BFValue[v_idx] = BFValue[u_idx] + weight;
                BFPrev[v_idx] = u;
            }
        }
    }

    // Clean up dynamically allocated memory
    delete[] sortedUniqueVertices;
}


// string BF_Path(int graph[][3], int numEdges, char startVertex, int goalVertex) {
//     if (numEdges <= 0) {
//         return; // No edges to process
//     }

//     // Initialize BFValue and BFPrev with all -1s
//     int BFValue[numVertices];
//     int BFPrev[numVertices];
//     for (int i = 0; i < numVertices; i++) {
//         BFValue[i] = -1;
//         BFPrev[i] = -1;
//     }
// }