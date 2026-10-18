#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int n, L;
ll W;
vector<vector<pair<int,ll>>> adj;   // (neighbor, weight)
vector<int> sub;
vector<bool> removed;
vector<int> bit;                     // Fenwick over len (index = len + 1)
ll ans = 0;

void bitAdd(int len, int val) {
    for (int i = len + 1; i <= n + 1; i += i & -i) bit[i] += val;
}
int bitSum(int len) {                // number of entries with length <= len
    int s = 0;
    for (int i = len + 1; i > 0; i -= i & -i) s += bit[i];
    return s;
}

void dfs_size(int u, int p) {
    sub[u] = 1;
    for (auto [v, w] : adj[u]) {
        if (v == p || removed[v]) continue;
        dfs_size(v, u);
        sub[u] += sub[v];
    }
}

int find_centroid(int u, int p, int tot) {
    for (auto [v, w] : adj[u]) {
        if (v == p || removed[v]) continue;
        if (sub[v] > tot / 2) return find_centroid(v, u, tot);
    }
    return u;
}

// collect (wt, len) of every vertex, pruning when a limit is exceeded
void collect(int u, int p, int len, ll wt, vector<pair<ll,int>>& v) {
    if (len > L || wt > W) return;
    v.push_back({wt, len});
    for (auto [x, w] : adj[u]) {
        if (x == p || removed[x]) continue;
        collect(x, u, len + 1, wt + w, v);
    }
}

// number of unordered pairs (a, b) in v with len sum <= L and wt sum <= W
ll calc(vector<pair<ll,int>>& v) {
    sort(v.begin(), v.end());
    int m = v.size(), j = 0;
    ll res = 0;
    for (int i = m - 1; i >= 0; i--) {
        while (j < m && v[j].first + v[i].first <= W) {
            bitAdd(v[j].second, 1);
            j++;
        }
        int lim = min(L - v[i].second, n);       // L - len >= 0 because len <= L
        res += bitSum(lim);
        if (2LL * v[i].second <= L && 2 * v[i].first <= W) res--;   // remove self pair
    }
    for (int k = 0; k < j; k++) bitAdd(v[k].second, -1);            // clean the BIT
    return res / 2;
}

void build(int entry) {
    dfs_size(entry, 0);
    int c = find_centroid(entry, 0, sub[entry]);
    removed[c] = true;

    vector<pair<ll,int>> all;
    all.push_back({0, 0});                       // c itself

    vector<vector<pair<ll,int>>> lists;
    for (auto [ch, w] : adj[c]) {
        if (removed[ch]) continue;
        lists.push_back({});
        collect(ch, c, 1, w, lists.back());
        for (auto& x : lists.back()) all.push_back(x);
    }

    ans += calc(all);
    for (auto& v : lists) ans -= calc(v);

    for (auto [ch, w] : adj[c])
        if (!removed[ch]) build(ch);
}

int main() {
    scanf("%d %d %lld", &n, &L, &W);
    adj.resize(n + 1);
    sub.assign(n + 1, 0);
    removed.assign(n + 1, false);
    bit.assign(n + 3, 0);
    for (int i = 2; i <= n; i++) {               // vertex i has parent p with edge weight w
        int p; ll w;
        scanf("%d %lld", &p, &w);
        adj[i].push_back({p, w});
        adj[p].push_back({i, w});
    }
    build(1);
    printf("%lld\n", ans);
}
