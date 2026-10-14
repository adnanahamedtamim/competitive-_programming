
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 50005;

int n, m, T;
int x[MAXN * 2], y[MAXN * 2];

struct DSU {
    vector<int> p, sz;

    DSU(int n) {
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int u) {
        if (p[u] == u) return u;
        return p[u] = find(p[u]);
    }

    void unite(int u, int v) {
        u = find(u);
        v = find(v);

        if (u == v) return;

        if (sz[u] < sz[v]) swap(u, v);

        p[v] = u;
        sz[u] += sz[v];
    }
};

using BS = bitset<MAXN>;
vector<BS> dp;

void solve1() {
    dp.resize(n);

    for (int i = 0; i < n; i++) {
        dp[i].set(i);
    }

    for (int i = 0; i < m; i++) {
        BS tmp = dp[x[i]] | dp[y[i]];
        dp[x[i]] = dp[y[i]] = tmp;
    }

    int goal = -1;

    for (int i = 0; i < n; i++) {
        if (dp[i].count() == n) {
            goal = i;
            break;
        }
    }

    if (goal == -1) {
        cout << -1 << '\n';
        return;
    }

    DSU dsu(n);
    string ans(m, '^');

    for (int i = m - 1; i >= 0; i--) {
        int gx = dsu.find(x[i]);
        int gy = dsu.find(y[i]);
        int gg = dsu.find(goal);

        if (gx == gg && gy != gg) {
            ans[i] = '^';
            dsu.unite(x[i], y[i]);
        } else if (gy == gg && gx != gg) {
            ans[i] = 'v';
            dsu.unite(x[i], y[i]);
        }
    }

    cout << ans << '\n';
}

void solve2() {
    if (n == 2) {
        cout << -1 << '\n';
        return;
    }

    vector<int> B(n), cnt(n, 1);
    iota(B.begin(), B.end(), 0);

    string ans(m, '^');

    for (int i = m - 1; i >= 0; i--) {
        int bx = B[x[i]];
        int by = B[y[i]];

        if (cnt[bx] > cnt[by]) {
            ans[i] = 'v';
            cnt[by]++;
            cnt[bx]--;
            B[x[i]] = by;
        } else {
            ans[i] = '^';
            cnt[bx]++;
            cnt[by]--;
            B[y[i]] = bx;
        }
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> T;

    for (int i = 0; i < m; i++) {
        cin >> x[i] >> y[i];
        --x[i];
        --y[i];
    }

    if (T == 1) solve1();
    else solve2();

    return 0;
}
