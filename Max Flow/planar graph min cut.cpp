// given a 2d grid with n<=1000,m<=1000, each cell has a cost to build baricade on them . you need to block all the path from 1,1 to n,m 
// , in each move the enemy can move one step right or one step down, initially enemy is in 1,1 cell

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

// Function to map 2D face coordinates to a 1D node ID
// Faces are 1-indexed: i from 1 to N-1, j from 1 to M-1
inline int get_face_id(int i, int j, int M) {
    return (i - 1) * (M - 1) + j;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    int total_faces = (N - 1) * (M - 1);
    int S = 0;                 // Super Source (Top and Right boundaries)
    int T = total_faces + 1;   // Super Sink (Bottom and Left boundaries)

    // Dual Graph Adjacency List: {neighbor, weight}
    vector<vector<pair<int, ll>>> adj(T + 1);

    auto add_edge = [&](int u, int v, ll w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };

    // 1. Read Horizontal Edges (separates vertical faces)
    // Row i, Col j to Col j+1
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M - 1; j++) {
            ll cost;
            cin >> cost;
            if (i == 1) {
                // Top boundary road: separates Super Source and Top Face
                add_edge(S, get_face_id(1, j, M), cost);
            } else if (i == N) {
                // Bottom boundary road: separates Bottom Face and Super Sink
                add_edge(get_face_id(N - 1, j, M), T, cost);
            } else {
                // Internal horizontal road: separates Face above and Face below
                add_edge(get_face_id(i - 1, j, M), get_face_id(i, j, M), cost);
            }
        }
    }

    // 2. Read Vertical Edges (separates horizontal faces)
    // Row i to Row i+1, Col j
    for (int i = 1; i <= N - 1; i++) {
        for (int j = 1; j <= M; j++) {
            ll cost;
            cin >> cost;
            if (j == 1) {
                // Left boundary road: separates Super Sink and Left Face
                add_edge(T, get_face_id(i, 1, M), cost);
            } else if (j == M) {
                // Right boundary road: separates Right Face and Super Source
                add_edge(get_face_id(i, M - 1, M), S, cost);
            } else {
                // Internal vertical road: separates Face left and Face right
                add_edge(get_face_id(i, j - 1, M), get_face_id(i, j, M), cost);
            }
        }
    }

    // === Dijkstra's Algorithm on Dual Graph ===
    vector<ll> dist(T + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[S] = 0;
    pq.push({0, S});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == T) break; // Early exit since we reached the Super Sink

        for (auto& edge : adj[u]) {
            int v = edge.first;
            ll w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    cout << dist[T] << "\n";

    return 0;
}
