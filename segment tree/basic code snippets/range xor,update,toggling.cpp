
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Node {
    ll xr = 0;

    Node() {}

    Node(ll xr) : xr(xr) {}
};

Node merge(Node a, Node b) {
    return Node(a.xr ^ b.xr);
}

struct SegTree {
    ll n;
    vector<Node> tree;
    vector<ll> lazy;

    SegTree(ll n) : n(n), tree(4 * n), lazy(4 * n, 0) {}

    void pull(ll node) {
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    void apply(ll node, ll l, ll r, ll x) {
        if ((r - l + 1) & 1)
            tree[node].xr ^= x;

        lazy[node] ^= x;
    }

    void push(ll node, ll l, ll r) {
        if (lazy[node] == 0 || l == r) return;

        ll mid = (l + r) / 2;

        apply(node * 2, l, mid, lazy[node]);
        apply(node * 2 + 1, mid + 1, r, lazy[node]);

        lazy[node] = 0;
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

    void update(ll node, ll l, ll r, ll ql, ll qr, ll x) {
        if (r < ql || qr < l) return;

        if (ql <= l && r <= qr) {
            apply(node, l, r, x);
            return;
        }

        push(node, l, r);

        ll mid = (l + r) / 2;

        update(node * 2, l, mid, ql, qr, x);
        update(node * 2 + 1, mid + 1, r, ql, qr, x);

        pull(node);
    }

    void update(ll l, ll r, ll x) {
        if (l > r) return;
        update(1, 0, n - 1, l, r, x);
    }

    void toggle(ll l, ll r) {
        update(l, r, 1);
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

    ll query(ll l, ll r) {
        if (l > r) return 0;
        return query(1, 0, n - 1, l, r).xr;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, q;
    cin >> n >> q;

    vector<ll> a(n);

    for (ll i = 0; i < n; i++)
        cin >> a[i];

    SegTree seg(n);
    seg.build(a);

    while (q--) {
        ll type;
        cin >> type;

        if (type == 1) {
            ll l, r, x;
            cin >> l >> r >> x;
            seg.update(l, r, x);
        } else if (type == 2) {
            ll l, r;
            cin >> l >> r;
            cout << seg.query(l, r) << '\n';
        } else if (type == 3) {
            ll l, r;
            cin >> l >> r;
            seg.toggle(l, r);
        }
    }

    return 0;
}
