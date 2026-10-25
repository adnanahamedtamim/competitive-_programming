// given a dag, need to select maximum number  of  subset of vertices such that no two are reachble from one another in the set of selected vertices. 


#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge {
    int to;
    int cap;
    int flow;
    int rev;
};

struct Dinic {
    int n;
    vector<vector<Edge>> adj;
    vector<int> level, ptr;

    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    void add_edge(int from, int to, int cap) {
        adj[from].push_back({to, cap, 0, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& edge : adj[v]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    int dfs(int v, int t, int pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
            auto& edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
            int tr_pushed = dfs(tr, t, min(pushed, edge.cap - edge.flow));
            if (tr_pushed == 0) continue;
            edge.flow += tr_pushed;
            adj[tr][edge.rev].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    int maxflow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // STEP 1: Compute Transitive Closure using bitset (Fast Floyd-Warshall)
    // reach[i][j] is 1 if there is a path from i to j.
    vector<bitset<505>> reach(n + 1);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        reach[u][v] = 1;
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            if (reach[i][k]) {
                reach[i] |= reach[k];
            }
        }
    }

    // STEP 2: Build Bipartite Flow Network
    // Nodes 1 to N are the "Left" layer (u_out)
    // Nodes N+1 to 2N are the "Right" layer (u_in)
    int S = 0;
    int T = 2 * n + 1;
    Dinic dt(T + 1);

    for (int i = 1; i <= n; i++) {
        // Connect Super Source to Left Layer
        dt.add_edge(S, i, 1);
        
        // Connect Right Layer to Super Sink
        dt.add_edge(i + n, T, 1);
    }

    // Add edges for all reachable pairs
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (reach[i][j]) {
                dt.add_edge(i, j + n, 1);
            }
        }
    }

    // STEP 3: Apply Dilworth's Theorem
    int max_matching = dt.maxflow(S, T);
    int max_antichain = n - max_matching;

    cout << max_antichain << "\n";

    return 0;
}
