
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<ll> parent_, depth_, heavy, head, pos, sze, nodeatpos;
ll curPos = 0;

struct Node {
    ll cnt = 0;

    Node() {}

    Node(ll cnt) : cnt(cnt) {}
};

Node merge(Node a, Node b) {
    return Node(a.cnt + b.cnt);
}

struct SegTree {
    ll n;
    vector<Node> tree;
    vector<bool> lazy;

    SegTree(ll n) : n(n), tree(4 * n), lazy(4 * n, false) {}

    void pull(ll node) {
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    void apply(ll node, ll l, ll r) {
        tree[node].cnt = (r - l + 1) - tree[node].cnt;
        lazy[node] = !lazy[node];
    }

    void push(ll node, ll l, ll r) {
        if (!lazy[node] || l == r) return;

        ll mid = (l + r) / 2;

        apply(node * 2, l, mid);
        apply(node * 2 + 1, mid + 1, r);

        lazy[node] = false;
    }

    void build(ll node, ll l, ll r, vector<ll> &a) {
        if (l == r) {
            tree[node] = Node(a[l]);
            return;
        }

        ll mid = (l + r) / 2;

        build(node * 2, l, mid, a);
        build(node * 2 + 1, mid + 1, r, a);

        pull(node);
    }

    void build(vector<ll> &a) {
        build(1, 0, n - 1, a);
    }

    void toggle(ll node, ll l, ll r, ll ql, ll qr) {
        if (r < ql || qr < l) return;

        if (ql <= l && r <= qr) {
            apply(node, l, r);
            return;
        }

        push(node, l, r);

        ll mid = (l + r) / 2;

        toggle(node * 2, l, mid, ql, qr);
        toggle(node * 2 + 1, mid + 1, r, ql, qr);

        pull(node);
    }

    void rangeToggle(ll l, ll r) {
        if (l > r) return;
        toggle(1, 0, n - 1, l, r);
    }

    Node query(ll node, ll l, ll r, ll ql, ll qr) {
        if (r < ql || qr < l) return Node();

        if (ql <= l && r <= qr)
            return tree[node];

        push(node, l, r);

        ll mid = (l + r) / 2;

        return merge(
            query(node * 2, l, mid, ql, qr),
            query(node * 2 + 1, mid + 1, r, ql, qr)
        );
    }

    Node query(ll l, ll r) {
        if (l > r) return Node();
        return query(1, 0, n - 1, l, r);
    }

    ll pointQuery(ll i) {
        return query(i, i).cnt;
    }

    ll first(ll node, ll l, ll r, ll ql, ll qr) {
        if (r < ql || qr < l || tree[node].cnt == 0)
            return -1;

        if (l == r)
            return l;

        push(node, l, r);

        ll mid = (l + r) / 2;

        ll res = first(node * 2, l, mid, ql, qr);

        if (res != -1)
            return res;

        return first(node * 2 + 1, mid + 1, r, ql, qr);
    }

    ll first(ll l, ll r) {
        if (l > r) return -1;
        return first(1, 0, n - 1, l, r);
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

ll lca(ll u, ll v) {
    while (head[u] != head[v]) {
        if (depth_[head[u]] < depth_[head[v]])
            swap(u, v);

        u = parent_[head[u]];
    }

    return depth_[u] < depth_[v] ? u : v;
}

ll queryPath(ll a, ll b, SegTree &seg) {
    ll c = lca(a, b);

    while (head[a] != head[c]) {
        ll res = seg.first(pos[head[a]], pos[a]);

        if (res != -1)
            return nodeatpos[res] + 1;

        a = parent_[head[a]];
    }

    ll res = seg.first(pos[c], pos[a]);

    if (res != -1)
        return nodeatpos[res] + 1;

    vector<pair<ll, ll>> segments;

    while (head[b] != head[c]) {
        segments.push_back({pos[head[b]], pos[b]});
        b = parent_[head[b]];
    }

    if (pos[c] < pos[b])
        segments.push_back({pos[c] + 1, pos[b]});

    reverse(segments.begin(), segments.end());

    for (auto [l, r] : segments) {
        ll p = seg.first(l, r);

        if (p != -1)
            return nodeatpos[p] + 1;
    }

    return -1;
}

void togglePath(ll a, ll b, SegTree &seg) {
    ll c = lca(a, b);

    while (head[a] != head[c]) {
        seg.rangeToggle(pos[head[a]], pos[a]);
        a = parent_[head[a]];
    }

    seg.rangeToggle(pos[c], pos[a]);

    while (head[b] != head[c]) {
        seg.rangeToggle(pos[head[b]], pos[b]);
        b = parent_[head[b]];
    }

    if (pos[c] < pos[b])
        seg.rangeToggle(pos[c] + 1, pos[b]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;

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

    vector<ll> initial(n, 0);
    SegTree seg(n);
    seg.build(initial);

    while (q--) {
        ll type;
        cin >> type;

        if (type == 0) {
            ll s;
            cin >> s;
            --s;

            seg.rangeToggle(pos[s], pos[s]);
        } else if (type == 1) {
            ll a, b;
            cin >> a >> b;
            --a;
            --b;

            cout << queryPath(a, b, seg) << '\n';
        } else if (type == 2) {
            ll a, b;
            cin >> a >> b;
            --a;
            --b;

            togglePath(a, b, seg);
        }
    }

    return 0;
}
