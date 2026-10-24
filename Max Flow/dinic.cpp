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

    Dinic(ll n) : n(n), adj(n), level(n), it(n) {}

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
            while (ll pushed = dfs(s, t, 1e18)) // Using a large ll constant
                flow += pushed;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    if (!(cin >> n >> m)) return 0;
    
    Dinic dinic(n + 1);

    for (ll i = 0; i < m; i++) {
        ll a, b;
        cin >> a >> b;
        dinic.add_edge(a, b, 1);
    }

    ll maxPaths = dinic.maxflow(1, n);
    cout << maxPaths << "\n";

    vector<vector<ll>> routes;
    
    for (ll i = 0; i < maxPaths; i++) {
        vector<ll> path;
        ll cur = 1;
        path.push_back(cur);
        
        while (cur != n) {
            bool found = false;
            for (auto &e : dinic.adj[cur]) {
                if (e.flow > 0) {
                    e.flow = 0; // Use flow and mark as consumed
                    cur = e.to;
                    path.push_back(cur);
                    found = true;
                    break;
                }
            }
            if (!found) break; 
        }
        routes.push_back(path);
    }

    for (auto &path : routes) {
        cout << path.size() << "\n";
        for (ll i = 0; i < (ll)path.size(); i++) {
            cout << path[i] << (i == (ll)path.size() - 1 ? "" : " ");
        }
        cout << "\n";
    }
    
    return 0;
}
