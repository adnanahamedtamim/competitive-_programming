#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define ll long long
const ll NEG_INF = -2e18;

struct Line {
    ll m, c;
    Line(ll _m = 0, ll _c = NEG_INF) : m(_m), c(_c) {}
    ll eval(ll x) const {
        if (c == NEG_INF) return NEG_INF;
        return m * x + c;
    }
};

// Dynamic Li Chao Tree (MAXIMUM) with segment insertion
class LiChaoTree {
private:
    struct Node {
        Line line;
        int left_child, right_child;
        Node() : line(0, NEG_INF), left_child(-1), right_child(-1) {}
    };

    vector<Node> tree;
    ll MIN_X, MAX_X;

    int get_left_child(int node) {
        if (tree[node].left_child == -1) {
            int new_idx = tree.size();
            tree.emplace_back();
            tree[node].left_child = new_idx;
        }
        return tree[node].left_child;
    }

    int get_right_child(int node) {
        if (tree[node].right_child == -1) {
            int new_idx = tree.size();
            tree.emplace_back();
            tree[node].right_child = new_idx;
        }
        return tree[node].right_child;
    }

    void add_line_internal(int node, ll l, ll r, Line nw) {
        ll mid = l + (r - l) / 2;

        bool left = nw.eval(l) > tree[node].line.eval(l);
        bool middle = nw.eval(mid) > tree[node].line.eval(mid);

        if (middle)
            swap(tree[node].line, nw);

        // FIX: the pushed-down line is the empty placeholder, so stop here.
        // Without this, empty lines keep creating child nodes until memory runs out.
        if (nw.c == NEG_INF) return;

        if (l == r) return;

        if (left != middle)
            add_line_internal(get_left_child(node), l, mid, nw);
        else
            add_line_internal(get_right_child(node), mid + 1, r, nw);
    }

    void add_segment_internal(int node, ll l, ll r, ll L, ll R, Line nw) {
        if (r < L || l > R)
            return;

        if (L <= l && r <= R) {
            add_line_internal(node, l, r, nw);
            return;
        }

        ll mid = l + (r - l) / 2;
        add_segment_internal(get_left_child(node), l, mid, L, R, nw);
        add_segment_internal(get_right_child(node), mid + 1, r, L, R, nw);
    }

    ll query_internal(int node, ll l, ll r, ll x) {
        if (node == -1)
            return NEG_INF;

        ll ans = tree[node].line.eval(x);
        if (l == r)
            return ans;

        ll mid = l + (r - l) / 2;
        if (x <= mid)
            return max(ans, query_internal(tree[node].left_child, l, mid, x));
        else
            return max(ans, query_internal(tree[node].right_child, mid + 1, r, x));
    }

public:
    LiChaoTree(ll minX, ll maxX) : MIN_X(minX), MAX_X(maxX) {
        tree.reserve(1 << 20);
        tree.emplace_back();
    }

    void add_line(ll m, ll c) {
        add_line_internal(0, MIN_X, MAX_X, Line(m, c));
    }

    void add_segment(ll L, ll R, ll m, ll c) {
        L = max(L, MIN_X);
        R = min(R, MAX_X);
        if (L > R) return;
        add_segment_internal(0, MIN_X, MAX_X, L, R, Line(m, c));
    }

    ll query(ll x) {
        if (x < MIN_X || x > MAX_X)
            return NEG_INF;
        return query_internal(0, MIN_X, MAX_X, x);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;
    LiChaoTree lct(0, m);

    while (n--) {
        ll x, y, x2, y2;
        cin >> x >> y >> x2 >> y2;

        // FIX: a vertical segment (x == x2) has no slope, so it can't be a line
        if (x == x2) continue;

        // FIX: make the left endpoint come first, so the segment is not dropped
        if (x > x2) {
            swap(x, x2);
            swap(y, y2);
        }

        // NOTE: integer division truncates. This is only correct if the slope
        // is always an integer. If the problem needs exact values, store the
        // slope as a fraction (num/den) instead.
        ll slope = (y2 - y) / (x2 - x);
        ll c = slope * (-x2) + y2;
        lct.add_segment(x, x2, slope, c);
    }

    for (ll i = 0; i <= m; i++) {
        ll ans = lct.query(i);
        cout << (ans == NEG_INF ? -1 : ans) << " ";
    }
    cout << '\n';

    return 0;
}
