
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<ll> parent_, depth_, heavy, head, pos, sze, nodeatpos;
vector<ll> val;
ll curPos = 0;

struct Fenwick {
    ll n;
    vector<ll> bit;

    Fenwick(ll n) : n(n), bit(n + 1, 0) {}

    void add(ll idx, ll delta) {
        idx++;
        while (idx <= n) {
            bit[idx] += delta;
            idx += idx & -idx;
        }
    }

    ll sum(ll idx) {
        idx++;
        ll res = 0;

        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }

        return res;
    }

    ll rangeSum(ll l, ll r) {
        if (l > r) return 0;
        return sum(r) - (l ? sum(l - 1) : 0);
    }
};

void dfs(ll u, vector<vector<ll>> &adj) {
    sze[u] = 1;

    for (ll v : adj[u]) {
        if (v == parent_[u]) continue;

        parent_[v] = u;
        depth_[v] = depth_[u] + 1;

        dfs(v, adj);

        sze[u] += sze[v];

        if (heavy[u] == -1 || sze[v] > sze[heavy[u]])
            heavy[u] = v;
    }
}

void dfs_hld(ll u, ll h, vector<vector<ll>> &adj) {
    head[u] = h;
    pos[u] = curPos;
    nodeatpos[curPos++] = u;

    if (heavy[u] != -1)
        dfs_hld(heavy[u], h, adj);

    for (ll v : adj[u]) {
        if (v == parent_[u] || v == heavy[u]) continue;
        dfs_hld(v, v, adj);
    }
}

ll queryPath(ll a, ll b, Fenwick &fw) {
    ll x = a, y = b;

    while (head[x] != head[y]) {
        if (depth_[head[x]] > depth_[head[y]])
            x = parent_[head[x]];
        else
            y = parent_[head[y]];
    }

    ll c = (depth_[x] < depth_[y] ? x : y);

    while (head[a] != head[c]) {
        ll l = pos[head[a]], r = pos[a];

        if (fw.rangeSum(l, r) > 0) {
            while (l < r) {
                ll mid = (l + r + 1) / 2;

                if (fw.rangeSum(mid, pos[a]) > 0)
                    l = mid;
                else
                    r = mid - 1;
            }

            return nodeatpos[l] + 1;
        }

        a = parent_[head[a]];
    }

    ll l = pos[c], r = pos[a];

    if (fw.rangeSum(l, r) > 0) {
        while (l < r) {
            ll mid = (l + r + 1) / 2;

            if (fw.rangeSum(mid, pos[a]) > 0)
                l = mid;
            else
                r = mid - 1;
        }

        return nodeatpos[l] + 1;
    }

    vector<pair<ll, ll>> segs;

    while (head[b] != head[c]) {
        segs.push_back({pos[head[b]], pos[b]});
        b = parent_[head[b]];
    }

    if (pos[c] < pos[b])
        segs.push_back({pos[c] + 1, pos[b]});

    reverse(segs.begin(), segs.end());

    for (auto [l, r] : segs) {
        if (fw.rangeSum(l, r) == 0) continue;

        while (l < r) {
            ll mid = (l + r) / 2;

            if (fw.rangeSum(l, mid) > 0)
                r = mid;
            else
                l = mid + 1;
        }

        return nodeatpos[l] + 1;
    }

    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;

    val.assign(n, 0);

    vector<vector<ll>> adj(n);

    for (ll i = 0; i < n - 1; i++) {
        ll a, b;
        cin >> a >> b;
        --a;
        --b;

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    parent_.assign(n, -1);
    depth_.assign(n, 0);
    heavy.assign(n, -1);
    head.assign(n, -1);
    pos.assign(n, 0);
    sze.assign(n, 0);
    nodeatpos.assign(n, 0);

    dfs(0, adj);
    dfs_hld(0, 0, adj);

    Fenwick fw(n);

    while (q--) {
        ll type;
        cin >> type;

        if (type == 0) {
            ll s;
            cin >> s;
            --s;

            ll x = 1 - val[s];
            ll delta = x - val[s];
            val[s] = x;

            fw.add(pos[s], delta);
        } else {
            ll a, b;
            cin >> a >> b;
            --a;
            --b;

            cout << queryPath(a, b, fw) << '\n';
        }
    }

    return 0;
}
