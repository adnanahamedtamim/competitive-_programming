#include <bits/stdc++.h>
using namespace std;

using ll = long long;

#define pb push_back

vector<bool> removed;
vector<vector<int>> adj;
vector<int> sub;
vector<int> bt;        // bt[u]  = 1 << letter(u)
vector<int> mk;        // mk[u]  = XOR of letters on path centroid -> u (both ends)
vector<int> par_;      // parent of u when rooted at the current centroid
vector<ll> ans, f;
vector<ll> cnt;        // cnt[mask] = number of vertices currently inserted with this mask

/*------------------------------------------------------------*
 | Compute subtree sizes ignoring removed centroid nodes
 *------------------------------------------------------------*/
void dfs_size(int u, int p) {
    sub[u] = 1;
    for (int v : adj[u]) {
        if (v == p || removed[v]) continue;
        dfs_size(v, u);
        sub[u] += sub[v];
    }
}

/*------------------------------------------------------------*
 | Find centroid of current component
 *------------------------------------------------------------*/
int find_centroid(int u, int p, int totalSize) {
    for (int v : adj[u]) {
        if (v == p || removed[v]) continue;
        if (sub[v] > totalSize / 2)
            return find_centroid(v, u, totalSize);
    }
    return u;
}

/*------------------------------------------------------------*
 | Collect masks (and preorder list) of one child subtree
 *------------------------------------------------------------*/
void collect(int u, int p, int mask, vector<int>& L) {
    mk[u] = mask ^ bt[u];
    par_[u] = p;
    L.pb(u);                                   // preorder: parent before child
    for (int v : adj[u]) {
        if (v == p || removed[v]) continue;
        collect(v, u, mk[u], L);
    }
}

bool good(int m) { return __builtin_popcount(m) <= 1; }

// number of inserted vertices u such that path u--v (through centroid) is a palindrome
ll query(int v, int cbit) {
    int need = mk[v] ^ cbit;
    ll s = cnt[need];                                         // popcount 0
    for (int k = 0; k < 20; k++) s += cnt[need ^ (1 << k)];   // popcount 1
    return s;
}

void addList(const vector<int>& L, int d) {
    for (int v : L) cnt[mk[v]] += d;
}

/*------------------------------------------------------------*
 | Centroid decomposition
 *------------------------------------------------------------*/
void build(int entry) {
    dfs_size(entry, -1);

    int c = find_centroid(entry, -1, sub[entry]);
    removed[c] = true;

    int cbit = bt[c];
    mk[c] = cbit;

    // collect every child subtree of c
    vector<vector<int>> lists;
    for (int ch : adj[c]) {
        if (removed[ch]) continue;
        lists.pb({});
        collect(ch, c, cbit, lists.back());
    }

    ll Q = 0;      // good paths u--v with u, v in different child subtrees
    ll sumF = 0;   // sum of f[v] over all v != c

    // LEFT -> RIGHT sweep: c is inside cnt, so c--v paths are counted here
    cnt[mk[c]]++;
    for (auto& L : lists) {
        for (int v : L) {
            ll s = query(v, cbit);
            f[v] = s;
            sumF += s;
            Q += s - (good(mk[v]) ? 1 : 0);   // remove c as partner -> pure cross count
        }
        addList(L, +1);
    }
    cnt[mk[c]]--;
    for (auto& L : lists) addList(L, -1);     // clear cnt

    // RIGHT -> LEFT sweep: c is NOT in cnt, so c--v is not counted twice
    for (int i = (int)lists.size() - 1; i >= 0; i--) {
        for (int v : lists[i]) {
            ll s = query(v, cbit);
            f[v] += s;
            sumF += s;
        }
        addList(lists[i], +1);
    }
    for (auto& L : lists) addList(L, -1);     // clear cnt

    // centroid: sumF = 2Q + P, paths through c = Q + P + 1
    ans[c] += sumF - Q + 1;

    // bottom-up: every ancestor owns the paths of its descendants
    for (auto& L : lists) {
        for (int i = (int)L.size() - 1; i >= 0; i--) {   // children before parents
            int v = L[i];
            ans[v] += f[v];                              // f[v] = full subtree sum now
            if (par_[v] != c) f[par_[v]] += f[v];
        }
    }

    // recurse on remaining components
    for (int v : adj[c]) {
        if (!removed[v]) build(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    adj.resize(n + 1);
    sub.assign(n + 1, 0);
    bt.assign(n + 1, 0);
    mk.assign(n + 1, 0);
    par_.assign(n + 1, 0);
    ans.assign(n + 1, 0);
    f.assign(n + 1, 0);
    removed.assign(n + 1, false);
    cnt.assign(1 << 20, 0);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }

    string s;
    cin >> s;
    for (int i = 1; i <= n; i++) bt[i] = 1 << (s[i - 1] - 'a');

    build(1);

    for (int i = 1; i <= n; i++) cout << ans[i] << ' ';
    cout << '\n';
    return 0;
}
