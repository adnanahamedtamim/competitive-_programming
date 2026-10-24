#pragma GCC optimize("Ofast,unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Edge {
    int to, rev;
    ll cap, flow;
};

struct Dinic {
    int n;
    ll max_cap = 0;
    vector<vector<Edge>> adj;
    vector<int> level, it, q;

    // Time Complexity: O(N) to initialize vectors
    // Space Complexity: O(N + M) for adjacency list and helper arrays
    Dinic(int n) : n(n), adj(n + 1), level(n + 1), it(n + 1), q(n + 1) {}

    // Time Complexity: O(1) per edge addition
    void add_edge(int u, int v, ll cap, bool directed = true) {
        if (u == v) return; // Ignore self-loops
        max_cap = max(max_cap, cap);
        Edge a = {v, (int)adj[v].size(), cap, 0};
        Edge b = {u, (int)adj[v].size(), directed ? 0 : cap, 0}; // reversed edge capacity
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    // Time Complexity: O(V + E) per BFS traversal
    bool bfs(int s, int t, ll lim) {
        fill(level.begin(), level.end(), -1);
        int head = 0, tail = 0;
        q[tail++] = s;
        level[s] = 0;
        while (head < tail) {
            int u = q[head++];
            for (auto &e : adj[u]) {
                if (e.cap - e.flow >= lim && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    if (e.to == t) return true; // Early exit optimization
                    q[tail++] = e.to;
                }
            }
        }
        return false;
    }

    // Time Complexity: O(V + E) amortized per blocking flow DFS
    ll dfs(int u, int t, ll f, ll lim) {
        if (u == t || f == 0) return f;
        ll total_pushed = 0;
        for (int &i = it[u]; i < (int)adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (e.cap - e.flow >= lim && level[e.to] == level[u] + 1) {
                ll pushed = dfs(e.to, t, min(f - total_pushed, e.cap - e.flow), lim);
                if (pushed > 0) {
                    e.flow += pushed;
                    adj[e.to][e.rev].flow -= pushed;
                    total_pushed += pushed;
                    if (total_pushed == f) return total_pushed;
                } else {
                    level[e.to] = -1; // Dead-end pruning
                }
            }
        }
        return total_pushed;
    }

    // Time Complexity: 
    // - Unit Networks / Matching: O(E * sqrt(V))
    // - General Graphs with Scaling: O(V * E * log(MaxCap))
    ll maxflow(int s, int t, bool scaling = false) {
        if (max_cap == 0) return 0;
        ll flow = 0;
        ll start_lim = scaling ? (1LL << __lg(max_cap)) : 1;

        for (ll lim = start_lim; lim >= 1; lim >>= 1) {
            while (bfs(s, t, lim)) {
                fill(it.begin(), it.end(), 0);
                flow += dfs(s, t, 2e18, lim);
            }
        }
        return flow;
    }

    // Time Complexity: O(V + E)
    void print_all_flows(bool only_positive = false) {
        for (int u = 0; u <= n; u++) {
            for (auto &e : adj[u]) {
                if (e.cap > 0) {
                    if (only_positive && e.flow <= 0) continue;
                    cout << u << " " << e.to << " " << e.flow << "/" << e.cap << "\n";
                }
            }
        }
    }

    // Time Complexity: O(V + E)
    void print_min_cut(int s) {  // min koita edge remove korle s and t disconnected hoi jabe , For finding Vertex Cut, vertex gulake split kore
        // 2 ta kore vertex banabo , that's it
        vector<int> reach(n + 1, 0);
        int head = 0, tail = 0;
        q[tail++] = s;
        reach[s] = 1;

        while (head < tail) {
            int u = q[head++];
            for (auto &e : adj[u]) {
                if (e.cap - e.flow > 0 && !reach[e.to]) {
                    reach[e.to] = 1;
                    q[tail++] = e.to;
                }
            }
        }

        for (int u = 0; u <= n; u++) {
            if (reach[u]) {
                for (auto &e : adj[u]) {
                    if (e.cap > 0 && !reach[e.to]) {
                        cout << u << " " << e.to << "\n";
                    }
                }
            }
        }
    }

    // Time Complexity: O(K * V) where K is maxPaths and V is path length
    void print_paths(int s, int t, ll maxPaths) {
        for (ll i = 0; i < maxPaths; i++) {
            vector<int> path;
            int cur = s;
            path.push_back(cur);

            while (cur != t) {
                for (auto &e : adj[cur]) {
                    if (e.cap > 0 && e.flow > 0) {
                        e.flow--; // consume flow
                        cur = e.to;
                        path.push_back(cur);
                        break;
                    }
                }
            }

            cout << path.size() << "\n";
            for (int j = 0; j < (int)path.size(); j++) {
                cout << path[j] << (j == (int)path.size() - 1 ? "" : " ");
            }
            cout << "\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    int S = 0, T = n + m + 1;
    Dinic dinic(T);

    // Source -> Boys (1 to n)
    for (int i = 1; i <= n; i++) dinic.add_edge(S, i, 1);
    // Potential pairs (Boy -> Girl)
    for (int i = 0; i < k; i++) {
        int a, b;
        cin >> a >> b;
        dinic.add_edge(a, n + b, 1);
    }
    // Girls -> Sink (n+1 to n+m)
    for (int i = 1; i <= m; i++) dinic.add_edge(n + i, T, 1);

    cout << dinic.maxflow(S, T, false) << "\n";
    
    // Print matched pairs
    for (int u = 1; u <= n; u++) {
        for (auto &e : dinic.adj[u]) {
            if (e.cap > 0 && e.flow > 0) {
                cout << u << " " << (e.to - n) << "\n";
            }
        }
    }
    return 0;
}

/*
 ==============================================================================
 MASTER DINIC'S ALGORITHM TEMPLATE — QUICK SELECTION GUIDE
 ==============================================================================

 1. MAXFLOW FUNCTION CALL:
    - dinic.maxflow(s, t, false); // NORMAL DINIC (Default)
      * Time Complexity: O(E * sqrt(V)) for unit networks; O(V^2 * E) worst-case.
      * USE WHEN:
        - Unit networks (all capacities = 1), like Bipartite Matching (School Dance).
        - Grid-based max-flow / path reconstruction problems (Distinct Routes).
        - Min-Cut problems that involve INF (1e18) edge capacities (Police Chase).
          (Why? Capacity Scaling would waste 60 outer loops dividing INF by 2).

    - dinic.maxflow(s, t, true); // CAPACITY SCALING
      * Time Complexity: generally O(E*log(MaxCap)) ,  O(V * E * log(MaxCap)) worst-case.
      * USE WHEN:
        - Pure max-flow pipe networks with massive capacities up to 10^9 
          AND NO INF edges (e.g., SPOJ FASTFLOW, CSES Download Speed).
          (Why? Protects against adversarial anti-Dinic test cases that TLE normal Dinic).

 2. EDGE ADDITION:
    - dinic.add_edge(u, v, cap, true);  // One-way directed edge (default)
    - dinic.add_edge(u, v, cap, false); // Two-way undirected edge (e.g., FASTFLOW)
 ==============================================================================
*/
