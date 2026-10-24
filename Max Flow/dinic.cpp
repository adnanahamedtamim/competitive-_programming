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
    vector<vector<Edge>> adj;
    vector<int> level, it, q;

    // Allocates n + 1 so all indices from 0 to n (inclusive) are valid
    Dinic(int n) : n(n), adj(n + 1), level(n + 1), it(n + 1), q(n + 1) {}

    void add_edge(int u, int v, ll cap, bool directed = true) {
        if (u == v) return; // Ignore self-loops
        Edge a = {v, (int)adj[v].size(), cap, 0};
        Edge b = {u, (int)adj[u].size(), directed ? 0 : cap, 0};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        int head = 0, tail = 0;
        q[tail++] = s;
        level[s] = 0;
        while (head < tail) {
            int u = q[head++];
            for (auto &e : adj[u]) {
                if (e.cap - e.flow > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    if (e.to == t) return true; // Early exit as soon as sink is reached
                    q[tail++] = e.to;
                }
            }
        }
        return false;
    }

    // Multi-push DFS + Dead-end pruning
    ll dfs(int u, int t, ll f) {
        if (u == t || f == 0) return f;
        ll total_pushed = 0;
        for (int &i = it[u]; i < (int)adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (e.cap - e.flow > 0 && level[e.to] == level[u] + 1) {
                ll pushed = dfs(e.to, t, min(f - total_pushed, e.cap - e.flow));
                if (pushed > 0) {
                    e.flow += pushed;
                    adj[e.to][e.rev].flow -= pushed;
                    total_pushed += pushed;
                    if (total_pushed == f) return total_pushed; // Node u's incoming flow is fully used
                } else {
                    level[e.to] = -1; // Prune dead-end node!
                }
            }
        }
        return total_pushed;
    }

    ll maxflow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            flow += dfs(s, t, 2e18); // Multi-push handles the entire blocking flow in 1 call!
        }
        return flow;
    }

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

    void print_min_cut() {
        // After maxflow(s, t) finishes, the final bfs(s, t) already ran without early exit!
        // So level[u] != -1 is ALREADY the exact reachability array from s.
        for (int u = 0; u <= n; u++) {
            if (level[u] != -1) {
                for (auto &e : adj[u]) {
                    if (e.cap > 0 && level[e.to] == -1) {
                        cout << u << " " << e.to << "\n";
                    }
                }
            }
        }
    }

    void print_paths(int s, int t, ll maxPaths) {
        for (ll i = 0; i < maxPaths; i++) {
            vector<int> path;
            int cur = s;
            path.push_back(cur);

            while (cur != t) {
                for (auto &e : adj[cur]) {
                    if (e.cap > 0 && e.flow > 0) {
                        e.flow--;
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

    int n, m;
    if (!(cin >> n >> m)) return 0;

    Dinic dinic(n); // Supports nodes 0..n seamlessly!

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        // For undirected graph (like Police Chase or FASTFLOW), just pass false!
        dinic.add_edge(a, b, 1, false);
    }

    cout << dinic.maxflow(1, n) << "\n";
    dinic.print_min_cut();

    return 0;
}
