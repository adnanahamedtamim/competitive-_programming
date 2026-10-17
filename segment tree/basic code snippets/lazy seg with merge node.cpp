
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
