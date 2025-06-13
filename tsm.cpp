#include "tsm.h"

vector<vector<int>> initAndPopulateAdjMatrix(const int graph[][3], 
                                             const vector<int>& sortedVertices, 
                                             const int& numEdges, 
                                             const int& numVertices) 
{
    vector<vector<int>> adjMatrix(numVertices, vector<int>(numVertices, -1));
    // Cost from a vertex to itself is always 0 => All values on the diagonal are 0
    for (int i = 0; i < numVertices; i++) adjMatrix[i][i] = 0;
    // Populate the adjacency matrix
    for (int i = 0; i < numEdges; i++) {
        int u = graph[i][0];
        int v = graph[i][1];
        int weight = graph[i][2];
        int u_idx = getVertexIndex(u, sortedVertices);
        int v_idx = getVertexIndex(v, sortedVertices);
        adjMatrix[u_idx][v_idx] = weight;
    }
    return adjMatrix;
}


/**
 * Calculate the shortest way to go over all the vertices in the graph and go back to the starting vertex.
 * usnig the dynamic programming approach (Help-Karp)
 * @param graph
 * @param numEdges
 * @param startVertex
 * @return Print the shortest path over all the vertices and return to the starting vertex.
 */
void Traveling(int graph[][3], int numEdges, char startVertex) {    
    // 1. Initialization
    vector<int> sortedVertices = getSortedVertices(graph, numEdges);
    int numVertices = sortedVertices.size();

    vector<int> idxToVertexMap;
    for (char v_char : sortedVertices) idxToVertexMap.push_back(v_char);

    // Handle base cases
    if (numVertices <= 0) return; // There is no vertices in the graph

    // Initialize the adjacency matrix
    vector<vector<int>> adjMatrix = initAndPopulateAdjMatrix(graph, sortedVertices, numEdges, numVertices);

    // 2. Perform the TSP algorithm (Held-Karp)
    // shift left number 1 by numVertices positions e.g 1 << 3 = 8 ('0001' -> '1000')
    int numMasks = 1 << numVertices;

    vector<vector<long long>> dp(numMasks, vector<long long>(numVertices, -1LL));
    vector<vector<int>> parent(numMasks, vector<int>(numVertices, -1));

    int startVertexIdx = getVertexIndex(startVertex, sortedVertices);
    dp[1 << startVertexIdx][startVertexIdx] = 0LL;
    for (int mask = 1; mask < numMasks; mask++) {
        for (int i = 0; i < numVertices; i++) { // 'i' is the current end-vertex index
            if (mask & (1 << i)) { 
                if (dp[mask][i] != -1LL) { 
                    for (int j = 0; j < numVertices; j++) { // 'j' is the next vertex index
                        if (!(mask & (1 << j)) && adjMatrix[i][j] != -1) {
                            int next_mask = mask | (1 << j);
                            long long edge_weight = adjMatrix[i][j];
                            long long new_cost = dp[mask][i] + edge_weight;

                            if (dp[next_mask][j] == -1LL || new_cost < dp[next_mask][j]) {
                                dp[next_mask][j] = new_cost;
                                parent[next_mask][j] = i; 
                            }
                        }
                    }
                }
            }
        }
    }

    // Find minimum tour cost
    long long min_total_cost = -1LL; // Using -1LL as INF for long long
    int last_vertex_in_tour = -1; 

    int final_mask = numMasks - 1; 

    for (int i = 0; i < numVertices; ++i) {
        if (dp[final_mask][i] != -1LL && adjMatrix[i][startVertexIdx] != -1) {
            long long cost_to_return = adjMatrix[i][startVertexIdx];
            long long current_tour_cost = dp[final_mask][i] + cost_to_return;
            
            if (min_total_cost == -1LL || current_tour_cost < min_total_cost) {
                min_total_cost = current_tour_cost;
                last_vertex_in_tour = i;
            }
        }
    }

    // 3. Output and Path Reconstruction
    if (min_total_cost == -1LL || last_vertex_in_tour == -1) {
        // cout << "No Hamiltonian cycle found" << endl; // debug
    } else {
        vector<int> tour_nodes_intermediate_indices; 
        int current_vertex_recon_idx = last_vertex_in_tour;
        int current_mask_recon = final_mask;

        while (true) {
            if (current_vertex_recon_idx == startVertexIdx && current_mask_recon == (1 << startVertexIdx)) {
                break; 
            }
            if (current_vertex_recon_idx == -1 || 
                (parent[current_mask_recon][current_vertex_recon_idx] == -1 && 
                 !(current_vertex_recon_idx == startVertexIdx && current_mask_recon == (1 << startVertexIdx))) ) {
                // cout << "Error in path reconstruction: Invalid parent." << endl; // debug
                min_total_cost = -1LL; 
                break;
            }

            tour_nodes_intermediate_indices.push_back(current_vertex_recon_idx);
            
            int prev_vertex_recon_idx = parent[current_mask_recon][current_vertex_recon_idx];
            current_mask_recon ^= (1 << current_vertex_recon_idx); 
            current_vertex_recon_idx = prev_vertex_recon_idx;
            
            if (tour_nodes_intermediate_indices.size() > static_cast<size_t>(numVertices) + 1) { 
                // cout << "Error in path reconstruction: Path too long." << endl; // debug
                min_total_cost = -1LL; 
                break;
            }
        }
        
        if (min_total_cost == -1LL) { 
            // cout << "No Hamiltonian cycle found" << endl; // debug
        } else {
            cout << startVertex; // Print original start character
            reverse(tour_nodes_intermediate_indices.begin(), tour_nodes_intermediate_indices.end());
            
            for (int node_idx : tour_nodes_intermediate_indices) {
                cout << " " << static_cast<char>(idxToVertexMap[node_idx]);
            }
            // Print return to start vertex (original char)
            cout << " " << startVertex << endl;
            // cout << "Cost: " << min_total_cost << endl; // debug
        }
    }
}
