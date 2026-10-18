
#include <bits/stdc++.h>
using namespace std;

#define pb push_back
typedef long long ll;
typedef vector<int> vi;

const int MX = 100005;
const int LOG = 20;
const int INF = 1e9;

int n, m;
vi adj[MX];

int dep[MX], up[MX][LOG];
int sz[MX], par[MX];
int best[MX];
bool blocked[MX];

void dfs(int u, int p) {
    up[u][0] = p;
    for (int j = 1; j < LOG; j++)
        up[u][j] = up[up[u][j - 1]][j - 1];

    for (int v : adj[u]) {
        if (v == p) continue;
        dep[v] = dep[u] + 1;
        dfs(v, u);
    }
}

int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);

    int d = dep[u] - dep[v];

    for (int j = 0; j < LOG; j++)
        if (d & (1 << j))
            u = up[u][j];

    if (u == v) return u;

    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }

    return up[u][0];
}

int dist(int u, int v) {
    int w = lca(u, v);
    return dep[u] + dep[v] - 2 * dep[w];
}

int calcSize(int u, int p) {
    sz[u] = 1;

    for (int v : adj[u]) {
        if (v == p || blocked[v]) continue;
        sz[u] += calcSize(v, u);
    }

    return sz[u];
}

int getCentroid(int u, int p, int total) {
    for (int v : adj[u]) {
        if (v == p || blocked[v]) continue;

        if (sz[v] > total / 2)
            return getCentroid(v, u, total);
    }

    return u;
}

void build(int u, int p) {
    int total = calcSize(u, 0);
    int c = getCentroid(u, 0, total);

    par[c] = p;
    blocked[c] = true;

    for (int v : adj[c]) {
        if (!blocked[v])
            build(v, c);
    }
}

void update(int x) {
    for (int c = x; c != 0; c = par[c])
        best[c] = min(best[c], dist(x, c));
}

int query(int x) {
    int ans = INF;

    for (int c = x; c != 0; c = par[c])
        ans = min(ans, best[c] + dist(x, c));

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }

    for (int i = 0; i <= n; i++)
        best[i] = INF;

    up[0][0] = 0;
    dep[1] = 0;
    dfs(1, 0);

    build(1, 0);

    update(1);

    while (m--) {
        int t, v;
        cin >> t >> v;

        if (t == 1)
            update(v);
        else
            cout << query(v) << '\n';
    }

    return 0;
}
