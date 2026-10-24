#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e14; // Safe infinity to prevent long long overflow

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

    void add_edge(int from, int to, ll cap, bool directed = true) {
        adj[from].push_back({to, cap, 0, (int)adj[to].size()});
        adj[to].push_back({from, directed ? 0 : cap, 0, (int)adj[from].size() - 1});
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m, r, c;
    cin >> n >> m >> r >> c;

    vector<vector<ll>> ara(n, vector<ll>(m));
    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            cin >> ara[i][j];
        }
    }

    r--;
    c--;

    // Node splitting:
    // u_in  = i * m + j
    // u_out = n * m + i * m + j
    ll SRC = 2 * n * m;
    ll SINK = 2 * n * m + 1;

    Dinic dt(SINK + 1);

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            ll u_in = i * m + j;
            ll u_out = n * m + i * m + j;

            // Cell cost capacity edge (Base cell cannot be cut, set capacity to INF)
            ll cost = ara[i][j];
            if (i == r && j == c) {
                cost = INF;
            }
            dt.add_edge(u_in, u_out, cost, true);

            // Boundary cells connect from Super Source (outside threat)
            if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                dt.add_edge(SRC, u_in, INF, true);
            }

            // Base cell connects to Super Sink
            if (i == r && j == c) {
                dt.add_edge(u_out, SINK, INF, true);
            }

            // Connect u_out to neighbors' u_in
            for (int k = 0; k < 4; k++) {
                ll nx = i + dx[k];
                ll ny = j + dy[k];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                    ll nxt_in = nx * m + ny;
                    dt.add_edge(u_out, nxt_in, INF, true);
                }
            }
        }
    }

    cout << dt.maxflow(SRC, SINK) << '\n';

    // Find reachable vertices from SRC in residual graph
    vector<int> vis(SINK + 1, 0);
    queue<int> q;
    q.push(SRC);
    vis[SRC] = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &e : dt.adj[u]) {
            if (e.cap - e.flow > 0 && !vis[e.to]) {
                vis[e.to] = 1;
                q.push(e.to);
            }
        }
    }

    vector<vector<char>> ans(n, vector<char>(m, '.'));

    // A cell is in the min-cut if u_in is reachable but u_out is not
    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            ll u_in = i * m + j;
            ll u_out = n * m + i * m + j;

            if (vis[u_in] && !vis[u_out]) {
                ans[i][j] = 'X';
            }
        }
    }

    // Output the result grid
    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            if (ara[i][j] == 0 && (i != r || j != c)) {
                cout << 'X';
            } else {
                cout << ans[i][j];
            }
        }
        cout << '\n';
    }

    return 0;
}
