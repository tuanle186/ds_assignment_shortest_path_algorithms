#include "tsm.h"

// Threshold for choosing algorithm based on problem size
// E.g. if numVertices > VERTICES_COUNT_THRESHOLD => Use heuristic approach (suboptimal)
//      else => Use Help-Karp approach (exact dynamic programming)
#define VERTICES_COUNT_THRESHOLD 20

// -----------------------------------------------------------------------------
// Small helpers reused by both the exact and the heuristic paths
// -----------------------------------------------------------------------------

static int getVertexIndex(const int targetVertex, const vector<int>& sortedVertices) {
    for (int i = 0; i < static_cast<int>(sortedVertices.size()); ++i) {
        if (sortedVertices[i] == targetVertex) return i;
    }
    return -1;
}

static vector<int> getSortedVertices(int graph[][3], int numEdges) {
    vector<int> vertices;
    for (int i = 0; i < numEdges; ++i) {
        vertices.push_back(graph[i][0]);
        vertices.push_back(graph[i][1]);
    }
    std::sort(vertices.begin(), vertices.end());
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    return vertices;
}

static vector<vector<int>> initAndPopulateAdjMatrix(const int graph[][3],
                                                   const vector<int>& sortedVertices,
                                                   const int& numEdges,
                                                   const int& numVertices) {
    vector<vector<int>> adjMatrix(numVertices, vector<int>(numVertices, -1));
    for (int i = 0; i < numVertices; ++i) adjMatrix[i][i] = 0;
    for (int i = 0; i < numEdges; ++i) {
        int u = graph[i][0];
        int v = graph[i][1];
        int w = graph[i][2];
        int ui = getVertexIndex(u, sortedVertices);
        int vi = getVertexIndex(v, sortedVertices);
        adjMatrix[ui][vi] = w;
    }
    return adjMatrix;
}

// -----------------------------------------------------------------------------
// HEURISTIC SECTION – only used when |V| > 22
// -----------------------------------------------------------------------------
namespace {
constexpr long long INF = 1LL << 50; // large but safe within signed 64‑bit

static long long safeEdgeCost(const vector<vector<int>>& adj, int u, int v) {
    int w = adj[u][v];
    return (w == -1) ? INF : static_cast<long long>(w);
}

static long long tourCost(const vector<int>& tour, const vector<vector<int>>& adj) {
    long long c = 0;
    for (size_t i = 0; i + 1 < tour.size(); ++i) {
        long long step = safeEdgeCost(adj, tour[i], tour[i + 1]);
        if (step >= INF) return INF;
        c += step;
    }
    return c;
}

// Nearest–Neighbour constructive heuristic
static vector<int> nearestNeighbour(int startIdx,
                                    const vector<vector<int>>& adj) {
    int n = static_cast<int>(adj.size());
    vector<int> tour;
    tour.reserve(n + 1);
    tour.push_back(startIdx);

    vector<char> visited(n, 0);
    visited[startIdx] = 1;

    for (int step = 1; step < n; ++step) {
        int last = tour.back();
        int best = -1;
        int bestW = INT32_MAX;
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && adj[last][v] != -1 && adj[last][v] < bestW) {
                bestW = adj[last][v];
                best = v;
            }
        }
        // If no reachable unvisited vertex remains, give up (graph not strongly connected)
        if (best == -1) return {};
        tour.push_back(best);
        visited[best] = 1;
    }
    tour.push_back(startIdx); // close tour
    return tour;
}

// 2‑Opt local search for tour improvement
static void twoOptImprove(vector<int>& tour, const vector<vector<int>>& adj) {
    if (tour.empty()) return;
    int n = static_cast<int>(tour.size()) - 1; // last == first
    bool improved = true;
    int maxPasses = 50; // prevent pathological runtimes
    while (improved && maxPasses--) {
        improved = false;
        for (int i = 1; i < n - 1; ++i) {
            for (int k = i + 1; k < n; ++k) {
                int a = tour[i - 1], b = tour[i];
                int c = tour[k], d = tour[(k + 1) % n];
                long long delta = safeEdgeCost(adj, a, c) + safeEdgeCost(adj, b, d) -
                                  safeEdgeCost(adj, a, b) - safeEdgeCost(adj, c, d);
                if (delta < 0) {
                    std::reverse(tour.begin() + i, tour.begin() + k + 1);
                    improved = true;
                }
            }
        }
    }
}

static string pathToString(const vector<int>& tour,
                           const vector<int>& idxToVertexMap) {
    if (tour.empty()) return "";
    string out;
    for (size_t i = 0; i < tour.size(); ++i) {
        out += static_cast<char>(idxToVertexMap[tour[i]]);
        if (i + 1 < tour.size()) out += " ";
    }
    return out;
}

static string solveTSPHeuristic(const vector<vector<int>>& adjMatrix,
                                const vector<int>& idxToVertexMap,
                                int startIdx) {
    // 1. Build initial solution
    vector<int> tour = nearestNeighbour(startIdx, adjMatrix);
    if (tour.empty()) return ""; // disconnected

    // 2. Local improvement
    twoOptImprove(tour, adjMatrix);

    // 3. Convert to required output format
    return pathToString(tour, idxToVertexMap);
}
} // anonymous namespace

// -----------------------------------------------------------------------------
// Main Traveling Function
// -----------------------------------------------------------------------------
string Traveling(int graph[][3], int numEdges, char startVertex) {
    // 1. Pre‑processing common to both approaches
    vector<int> sortedVertices = getSortedVertices(graph, numEdges);
    int numVertices = static_cast<int>(sortedVertices.size());

    // Map indices → original vertex IDs (which are given as ints representing
    // ASCII codes in the original submission).
    vector<int> idxToVertexMap(sortedVertices.begin(), sortedVertices.end());

    // Early trivial case
    if (numVertices == 0) return "";

    // Build adjacency matrix once – used by either branch
    vector<vector<int>> adjMatrix =
        initAndPopulateAdjMatrix(graph, sortedVertices, numEdges, numVertices);

    int startIdx = getVertexIndex(startVertex, sortedVertices);
    if (startIdx == -1) return ""; // start vertex absent from graph

    // ------------------------------------------------------------------
    // Choose algorithm based on problem size
    // ------------------------------------------------------------------
    if (numVertices > VERTICES_COUNT_THRESHOLD) {
        // ---- Heuristic branch ----
        return solveTSPHeuristic(adjMatrix, idxToVertexMap, startIdx);
    }

    // ---- Exact Held‑Karp branch ----
    int numMasks = 1 << numVertices;
    vector<vector<long long>> dp(numMasks, vector<long long>(numVertices, -1LL));
    vector<vector<int>> parent(numMasks, vector<int>(numVertices, -1));

    dp[1 << startIdx][startIdx] = 0LL;

    for (int mask = 1; mask < numMasks; ++mask) {
        for (int u = 0; u < numVertices; ++u) {
            if (!(mask & (1 << u)) || dp[mask][u] == -1LL) continue;
            for (int v = 0; v < numVertices; ++v) {
                if (mask & (1 << v)) continue; // already in set
                if (adjMatrix[u][v] == -1) continue; // no edge
                int nextMask = mask | (1 << v);
                long long newCost = dp[mask][u] + adjMatrix[u][v];
                if (dp[nextMask][v] == -1LL || newCost < dp[nextMask][v]) {
                    dp[nextMask][v] = newCost;
                    parent[nextMask][v] = u;
                }
            }
        }
    }

    long long bestCost = -1LL;
    int last = -1;
    int finalMask = numMasks - 1;

    for (int u = 0; u < numVertices; ++u) {
        if (dp[finalMask][u] == -1LL || adjMatrix[u][startIdx] == -1) continue;
        long long cost = dp[finalMask][u] + adjMatrix[u][startIdx];
        if (bestCost == -1LL || cost < bestCost) {
            bestCost = cost;
            last = u;
        }
    }

    if (bestCost == -1LL) return ""; // no Hamiltonian cycle

    // Reconstruct exact path
    vector<int> pathIndices;
    int mask = finalMask;
    int cur = last;
    while (cur != -1) {
        pathIndices.push_back(cur);
        int prev = parent[mask][cur];
        mask ^= (1 << cur);
        cur = prev;
        if (cur == startIdx && mask == (1 << startIdx)) {
            pathIndices.push_back(cur);
            break;
        }
    }
    std::reverse(pathIndices.begin(), pathIndices.end());

    // Convert to required output string format
    string result;
    for (size_t i = 0; i < pathIndices.size(); ++i) {
        result += static_cast<char>(idxToVertexMap[pathIndices[i]]);
        if (i + 1 < pathIndices.size()) result += " ";
    }
    result += " ";
    result += startVertex;
    return result;
}
