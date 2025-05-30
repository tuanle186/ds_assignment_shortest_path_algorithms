#include <iostream>
using namespace std;

const int INFINITY = 2147483647;

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
        sorted_unique_vertices[num_unique] = all_vertices[0];
        num_unique++;
        for (int i = 1; i < vertices_count; i++) {
            if (all_vertices[i] != all_vertices[i - 1]) {
                sorted_unique_vertices[num_unique] = all_vertices[i];
                num_unique++;
            }
        }
    }

    for (int i = 0; i < num_unique; i++) {
        cout << char(sorted_unique_vertices[i]) << endl;
    }
}