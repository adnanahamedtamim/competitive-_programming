
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 998244353;

struct Func {
    ll a = 1, b = 0;

    Func() {}

    Func(ll a, ll b) {
        this->a = a;
        this->b = b;
    }
};

Func compose(Func f, Func g) {
    return Func(
        g.a * f.a % MOD,
        (g.a * f.b + g.b) % MOD
    );
}

struct Node {
    Func fwd, rev;

    Node() {}

    Node(Func f) {
        fwd = rev = f;
    }
};

Node merge(Node L, Node R) {
    Node res;
    res.fwd = compose(L.fwd, R.fwd);
    res.rev = compose(R.rev, L.rev);
    return res;
}

struct SegTree {
    ll n;
    vector<Node> tree;

    SegTree(ll n) : n(n), tree(4 * n) {}

    void pull(ll node) {
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    void build(ll node, ll l, ll r, vector<Func> &a) {
        if (l == r) {
            tree[node] = Node(a[l]);
            return;
        }

        ll mid = (l + r) / 2;

        build(node * 2, l, mid, a);
        build(node * 2 + 1, mid + 1, r, a);

        pull(node);
    }

    void build(vector<Func> &a) {
        build(1, 0, n - 1, a);
    }

    void update(ll node, ll l, ll r, ll idx, Func f) {
        if (l == r) {
            tree[node] = Node(f);
            return;
        }

        ll mid = (l + r) / 2;

        if (idx <= mid)
            update(node * 2, l, mid, idx, f);
        else
            update(node * 2 + 1, mid + 1, r, idx, f);

        pull(node);
    }

    void update(ll idx, Func f) {
        update(1, 0, n - 1, idx, f);
    }

    Node query(ll node, ll l, ll r, ll ql, ll qr) {
        if (ql <= l && r <= qr)
            return tree[node];

        ll mid = (l + r) / 2;

        if (qr <= mid)
            return query(node * 2, l, mid, ql, qr);

        if (ql > mid)
            return query(node * 2 + 1, mid + 1, r, ql, qr);

        return merge(
            query(node * 2, l, mid, ql, qr),
            query(node * 2 + 1, mid + 1, r, ql, qr)
        );
    }

    Node query(ll l, ll r) {
        return query(1, 0, n - 1, l, r);
    }
};

vector<vector<ll>> adj;
vector<ll> parent_, depth_, heavy, head, pos, sze;
ll curPos = 0;

void dfs(ll u, ll p) {
    parent_[u] = p;
    sze[u] = 1;
    ll best = 0;

    for (ll v : adj[u]) {
        if (v == p) continue;

        depth_[v] = depth_[u] + 1;
        dfs(v, u);

        sze[u] += sze[v];

        if (sze[v] > best) {
            best = sze[v];
            heavy[u] = v;
        }
    }
}

void dfs_hld(ll u, ll h) {
    head[u] = h;
    pos[u] = curPos++;

    if (heavy[u] != -1)
        dfs_hld(heavy[u], h);

    for (ll v : adj[u]) {
        if (v == parent_[u] || v == heavy[u])
            continue;

        dfs_hld(v, v);
    }
}

void updatePathNode(ll u, Func f, SegTree &seg) {
    seg.update(pos[u], f);
}

Func queryPath(ll u, ll v, ll x, SegTree &seg) {
    Func left, right;

    while (head[u] != head[v]) {
        if (depth_[head[u]] > depth_[head[v]]) {
            Node cur = seg.query(pos[head[u]], pos[u]);

            left = compose(left, cur.rev);
            u = parent_[head[u]];
        } else {
            Node cur = seg.query(pos[head[v]], pos[v]);

            right = compose(cur.fwd, right);
            v = parent_[head[v]];
        }
    }

    if (depth_[u] >= depth_[v]) {
        Node cur = seg.query(pos[v], pos[u]);
        left = compose(left, cur.rev);
    } else {
        Node cur = seg.query(pos[u], pos[v]);
        right = compose(cur.fwd, right);
    }

    Func total = compose(left, right);

    return Func(
        total.a,
        (total.a * x + total.b) % MOD
    );
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;

    vector<Func> initial(n);

    for (ll i = 0; i < n; i++) {
        ll a, b;
        cin >> a >> b;
        initial[i] = Func(a, b);
    }

    adj.assign(n, {});

    for (ll i = 0; i < n - 1; i++) {
        ll u, v;
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    parent_.assign(n, -1);
    depth_.assign(n, 0);
    heavy.assign(n, -1);
    head.assign(n, -1);
    pos.assign(n, 0);
    sze.assign(n, 0);

    dfs(0, -1);
    dfs_hld(0, 0);

    vector<Func> arr(n);

    for (ll u = 0; u < n; u++)
        arr[pos[u]] = initial[u];

    SegTree seg(n);
    seg.build(arr);

    while (q--) {
        ll type;
        cin >> type;

        if (type == 0) {
            ll p, c, d;
            cin >> p >> c >> d;

            updatePathNode(p, Func(c, d), seg);
        } else {
            ll u, v, x;
            cin >> u >> v >> x;

            Func ans = queryPath(u, v, x, seg);
            cout << ans.b << '\n';
        }
    }

    return 0;
}
