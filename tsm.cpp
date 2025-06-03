#include "tsm.h"



/**
 * Calculate the shortest way to go over all the vertices in the graph and go back to the starting vertex.
 * @param graph
 * @param numEdges
 * @param startVertex
 * @return Print the shortest path over all the vertices and return to the starting vertex.
 */
void Traveling(int graph[][3], int numEdges, char startVertex) {
    if (numEdges <= 0) return;
    
    set<int> setOfSortedVertices = getSetOfSortedVertices(graph, numEdges);
    int numVertices = setOfSortedVertices.size();

    for (auto vertice : setOfSortedVertices) {
        cout << vertice << ", ";
    }

    cout << "\nnumEdges = " << numEdges;
    cout << "\nnumVertices = " << numVertices;

    
}



/*
int main() {
    // Example 1: A simple triangle A-B-C
    // A=65, B=66, C=67 (ASCII values)
    int graph1[][3] = {
        {65, 66, 10}, // A-B, weight 10
        {66, 67, 20}, // B-C, weight 20
        {67, 65, 30}  // C-A, weight 30
    };
    int numEdges1 = 3;
    char startVertex1 = 'A';
    std::cout << "Test Case 1:" << std::endl;
    Traveling(graph1, numEdges1, startVertex1); // Expected: A -> B -> C -> A, Cost: 10+20+30 = 60 (or A->C->B->A if that's shorter - no, this is fixed)

    std::cout << "\nTest Case 2: (A->B->C->A: 10+5+12=27), (A->C->B->A: 8+5+7=20)" << std::endl;
    int graph2[][3] = {
        {'A', 'B', 7}, 
        {'B', 'C', 5}, 
        {'C', 'A', 8}
    };
    Traveling(graph2, 3, 'A'); // Expected: A -> C -> B -> A Cost 20


    std::cout << "\nTest Case 3: Four vertices" << std::endl;
    int graph3[][3] = {
        {'A', 'B', 10}, {'A', 'C', 15}, {'A', 'D', 20},
        {'B', 'C', 35}, {'B', 'D', 25},
        {'C', 'D', 30}
    };
    Traveling(graph3, 6, 'A'); 
    // Possible paths:
    // A-B-C-D-A = 10+35+30+20 = 95
    // A-B-D-C-A = 10+25+30+15 = 80
    // A-C-B-D-A = 15+35+25+20 = 95 (No, A-C then C-B then B-D then D-A) A(15)C(35)B(25)D(20)A = 95
    // A-C-D-B-A = 15+30+25+10 = 80
    // A-D-B-C-A = 20+25+35+15 = 95
    // A-D-C-B-A = 20+30+35+10 = 95
    // Expected path A->B->D->C->A (cost 80) or A->C->D->B->A (cost 80)

    std::cout << "\nTest Case 4: Single Vertex" << std::endl;
    int graph4[][3] = {}; // No edges
    Traveling(graph4, 0, 'Z'); // Expected: Z -> Z, Cost: 0

    std::cout << "\nTest Case 5: Disconnected start vertex" << std::endl;
    int graph5[][3] = {
        {'X', 'Y', 5} 
    };
    Traveling(graph5, 1, 'A'); // Expected: No Hamiltonian cycle found

    std::cout << "\nTest Case 6: No path possible" << std::endl;
     int graph6[][3] = {
        {'A', 'B', 1},
        // C is isolated from A and B after A is chosen as start
    };
    Traveling(graph6, 1, 'A'); // A, B, C. Start A. Other {B,C}. Path A->B, B->C?, C->A?
                                // If C is not in edges, it becomes a vertex through startVertexChar if start='C'
                                // If start='A', vertices A, B. Path A->B->A cost 1+1=2. Oh, adjMatrix will have B-A=INF.
                                // This needs more thought on disconnected.
                                // If vertices are A,B from edges, and start is C. Then unique_vertex_chars_set = {A,B,C}
                                // adjMatrix[C_int][anything] = INF. So, no path.
                                // If start is 'A', vertices are A,B. Path A->B->A (adj[B][A] is INF, as only A-B given)
                                // This would print "No Hamiltonian cycle found" for A-B edge, start A. Correct.

    return 0;
}
*/