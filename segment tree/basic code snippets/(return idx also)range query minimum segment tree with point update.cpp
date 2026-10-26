struct Node {
    ll mn = LLONG_MAX;
    ll idx = -1;

    Node() {}

    Node(ll val, ll ind) {
        mn = val;
        idx = ind;
    }
};

class SegmentTree {
private:
    ll n;
    vector<Node> seg;

    Node merge(Node a, Node b) {
        if (a.mn <= b.mn)
            return a;
        return b;
    }

    void build(ll node, ll l, ll r, vector<ll>& ara) {
        if (l == r) {
            seg[node] = Node(ara[l], l);
            return;
        }

        ll mid = l + (r - l) / 2;

        build(2 * node + 1, l, mid, ara);
        build(2 * node + 2, mid + 1, r, ara);

        seg[node] = merge(seg[2 * node + 1],
                          seg[2 * node + 2]);
    }

    Node query(ll start, ll end, ll node, ll l, ll r) {
        if (r < start || l > end)
            return Node();

        if (start <= l && r <= end)
            return seg[node];

        ll mid = l + (r - l) / 2;

        return merge(
            query(start, end, 2 * node + 1, l, mid),
            query(start, end, 2 * node + 2, mid + 1, r)
        );
    }

    void update(ll node, ll l, ll r, ll idx, ll val) {
        if (l == r) {
            seg[node] = Node(val, l);
            return;
        }

        ll mid = l + (r - l) / 2;

        if (idx <= mid)
            update(2 * node + 1, l, mid, idx, val);
        else
            update(2 * node + 2, mid + 1, r, idx, val);

        seg[node] = merge(seg[2 * node + 1],
                          seg[2 * node + 2]);
    }

public:
    SegmentTree(vector<ll>& ara) {
        n = ara.size();
        seg.assign(4 * n, Node());
        build(0, 0, n - 1, ara);
    }

    SegmentTree(ll size) {
        n = size;
        seg.assign(4 * n, Node(0, -1));
        // Use this constructor only when initializing
        // a zero-filled tree; see note below.
    }

    Node query(ll l, ll r) {
        return query(l, r, 0, 0, n - 1);
    }

    void update(ll idx, ll val) {
        update(0, 0, n - 1, idx, val);
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    vector<ll> ara(n);
    for (ll i = 0; i < n; i++) {
        cin >> ara[i];
    }

    SegmentTree st(ara);

    ll q;
    cin >> q;

    while (q--) {
        ll type;
        cin >> type;

        if (type == 1) {
            // Point update: index, new value
            ll idx, val;
            cin >> idx >> val;

            st.update(idx, val);
        }
        else if (type == 2) {
            // Range query: l, r (inclusive)
            ll l, r;
            cin >> l >> r;

            Node ans = st.query(l, r);

            cout << ans.mn << " " << ans.idx << '\n';
        }
    }

    return 0;
}
