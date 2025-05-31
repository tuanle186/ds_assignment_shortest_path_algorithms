#include <iostream>
using namespace std;

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


void BF(int graph[][3], int numEdges, char startVertex, int BFValue[], int BFPrev[]) {
    if (numEdges <= 0) {
        return; // No edges to process
    }

    // 1. Collect all vertices from the graph, 
    // maximum number of vertices is 2 * numEdges
    int* all_vertices = new int[2 * numEdges];
    if (all_vertices == nullptr) {
        // Failed to allocate memory
        return; 
    }

    int vertices_count = 0;
    for (int i = 0; i < numEdges; i++) {
        all_vertices[vertices_count++] = graph[i][0];
        all_vertices[vertices_count++] = graph[i][1];
    }

    // 2. Sort all collected vertices
    bubbleSort(all_vertices, vertices_count);
    
    // 3. Extract unique sorted labels
    // The array of unique vertices can't be larger than all_vertices
    int* sorted_unique_vertices = new int[vertices_count];
    if (sorted_unique_vertices == nullptr) {
        delete[] all_vertices; // Clean up previously allocated memory
        return; // failed to allocate memory
    }

    int num_unique = 0;
    if (vertices_count > 0) {
        sorted_unique_vertices[num_unique++] = all_vertices[0];
        for (int i = 1; i < vertices_count; i++) {
            if (all_vertices[i] != all_vertices[i - 1]) {
                sorted_unique_vertices[num_unique++] = all_vertices[i];
            }
        }
    }

    cout << "numVertices: " << num_unique << endl;
    
    // 4. Perform the relaxation step
    // Make sure the label of the startVertex is 0
    int startVertexIndex = getVertexIndex(startVertex, sorted_unique_vertices, num_unique);
    BFValue[startVertexIndex] = 0;
    for (int i = 0; i < numEdges; i++) {
        int u = graph[i][0];
        int v = graph[i][1];
        int weight = graph[i][2];

        int u_idx = getVertexIndex(u, sorted_unique_vertices, num_unique);
        int v_idx = getVertexIndex(v, sorted_unique_vertices, num_unique);

        if (BFValue[u_idx] != -1) {
            if ((BFValue[u_idx] + weight < BFValue[v_idx]) || (BFValue[v_idx] == -1)) {
                BFValue[v_idx] = BFValue[u_idx] + weight;
                BFPrev[v_idx] = u;
            }
        }
    }

    // 5. Clean up dynamically allocated memory
    delete[] all_vertices;
    delete[] sorted_unique_vertices;
}