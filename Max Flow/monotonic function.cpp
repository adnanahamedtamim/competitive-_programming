/*

You are commanding a defensive fortification modeled as a network of n junctions and m directed pipes. 
Enemy forces are trying to pass flow from junction 1 (the source) to junction n (the sink).

To hold off the enemy, you have a variable energy resource parameter x (0 <= x <= 10^9).
The capacity of each pipe e (from junction u to v) is directly proportional to this energy resource, defined by a linear function:

C_e(x) = A_e * x + B_e

You need to find the maximum possible value of x such that the maximum flow from junction 1 to junction n is at least equal to a required target flow F.
*/

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e14;

struct Edge {
    int to;
    ll cap;
    ll flow;
    int rev;
};

struct Dinic {
    int n;
    vector<vector<Edge>> adj;
    vector<int> level, ptr;

    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    void add_edge(int from, int to, ll cap) {
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

    ll dfs(int v, int t, ll pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
            auto& edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
            ll tr_pushed = dfs(tr, t, min(pushed, edge.cap - edge.flow));
            if (tr_pushed == 0) continue;
            edge.flow += tr_pushed;
            adj[tr][edge.rev].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    ll maxflow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

// Example edge data structure for parametric scaling: C_e(x) = A_e * x + B_e
struct ParamEdge {
    int u, v;
    ll a, b;
};

bool check(double mid, int n, int src, int sink, const vector<ParamEdge>& edges, ll target_flow) {
    // Total nodes = sink + 1
    Dinic dt(sink + 1);
    
    for (const auto& pe : edges) {
        // Calculate capacity for a given parameter x = mid
        // Ensure capacities stay within safe limits or non-negative
        ll cap = max(0.0, pe.a * mid + pe.b);
        dt.add_edge(pe.u, pe.v, cap);
    }
    
    return dt.maxflow(src, sink) >= target_flow;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    // n: number of vertices, m: number of edges
    if (!(cin >> n >> m)) return 0;

    int src = 1, sink = n;
    ll target_flow;
    cin >> target_flow;

    vector<ParamEdge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].a >> edges[i].b;
    }

    // Binary search on the real parameter x (e.g., range [0, 1e9])
    double low = 0, high = 1e9;
    double ans = 0;

    for (int iter = 0; iter < 60; iter++) { // ~60 iterations guarantees high precision for doubles
        double mid = low + (high - low) / 2.0;
        if (check(mid, n, src, sink, edges, target_flow)) {
            ans = mid;
            low = mid; // Try to push for a larger parameter if maximizing
        } else {
            high = mid;
        }
    }

    cout << fixed << setprecision(6) << ans << '\n';

    return 0;
}
