#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int n;
vector<vector<int>> adj;
vector<int> sz, dep, ord;

void dfs_size(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u]) if (v != p) { dfs_size(v, u); sz[u] += sz[v]; }
}

int find_centroid(int u, int p) {
    for (int v : adj[u])
        if (v != p && sz[v] * 2 > n) return find_centroid(v, u);
    return u;
}

void dfs_order(int u, int p) {          // preorder: each subtree is contiguous
    ord.push_back(u);
    for (int v : adj[u]) if (v != p) { dep[v] = dep[u] + 1; dfs_order(v, u); }
}

int main() {
    scanf("%d", &n);
    adj.resize(n + 1);
    sz.assign(n + 1, 0);
    dep.assign(n + 1, 0);
    for (int i = 1; i < n; i++) {
        int a, b; scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    dfs_size(1, 0);
    int c = find_centroid(1, 0);

    dfs_order(c, 0);                    // dep[c] = 0

    ll sum = 0;
    for (int v = 1; v <= n; v++) sum += dep[v];

    vector<int> p(n + 1);
    int k = n / 2;
    for (int i = 0; i < n; i++) p[ord[i]] = ord[(i + k) % n];

    printf("%lld\n", 2 * sum);
    for (int i = 1; i <= n; i++) printf("%d ", p[i]);
    printf("\n");
}
