#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Edge {
    ll to, rev;
    ll cap, flow;
};

struct Dinic {
    ll n;
    vector<vector<Edge>> adj;
    vector<ll> level, it;

    // Allocates n + 1 so all indices from 0 to n (inclusive) are valid
    Dinic(ll n) : n(n), adj(n + 1), level(n + 1), it(n + 1) {}

    void add_edge(ll u, ll v, ll cap) {
        Edge a = {v, (ll)adj[v].size(), cap, 0};
        Edge b = {u, (ll)adj[u].size(), 0, 0};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool bfs(ll s, ll t) {
        fill(level.begin(), level.end(), -1);
        queue<ll> q;
        q.push(s);
        level[s] = 0;
        while (!q.empty()) {
            ll u = q.front(); q.pop();
            for (auto &e : adj[u]) {
                if (e.cap - e.flow > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(ll u, ll t, ll f) {
        if (u == t) return f;
        for (ll &i = it[u]; i < (ll)adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (e.cap - e.flow > 0 && level[e.to] == level[u] + 1) {
                ll pushed = dfs(e.to, t, min(f, e.cap - e.flow));
                if (pushed > 0) {
                    e.flow += pushed;
                    adj[e.to][e.rev].flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    ll maxflow(ll s, ll t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (ll pushed = dfs(s, t, 1e18))
                flow += pushed;
        }
        return flow;
    }

    void print_all_flows(bool only_positive = false) {
        for (ll u = 0; u <= n; u++) {
            for (auto &e : adj[u]) {
                if (e.cap > 0) {
                    if (only_positive && e.flow <= 0) continue;
                    cout << u << " " << e.to << " " << e.flow << "/" << e.cap << "\n";
                }
            }
        }
    }

    void print_min_cut(ll s) {
        vector<ll> reach(n + 1, 0);
        reach[s] = 1;
        queue<ll> q;
        q.push(s);

        while (!q.empty()) {
            ll i = q.front();
            q.pop();

            for (auto &it : adj[i]) {
                if (it.cap - it.flow > 0 && !reach[it.to]) {
                    reach[it.to] = 1;
                    q.push(it.to);
                }
            }
        }

        for (ll u = 0; u <= n; u++) {
            if (reach[u]) {
                for (auto &e : adj[u]) {
                    if (e.cap > 0 && !reach[e.to]) {
                        cout << u << " " << e.to << "\n";
                    }
                }
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    if (!(cin >> n >> m)) return 0;

    Dinic dinic(n); // Supports nodes 0..n seamlessly!

    for (ll i = 0; i < m; i++) {
        ll a, b;
        cin >> a >> b;
        // Example if converting 1-based input to 0-based (0 to n-1):
        // a--; b--;
        dinic.add_edge(a, b, 1);
        dinic.add_edge(b, a, 1);
    }

    // Works if you use 1..n (or 0..n-1 if you decrement input)
    cout << dinic.maxflow(1, n) << "\n";
    dinic.print_min_cut(1);

    return 0;
}
