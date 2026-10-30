#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 4e18; // Safe infinity for y = m*x + c evaluation

// Persistent Li Chao Tree (Sparse / Dynamic Coordinates)
// Supports both MINIMUM (is_max = false) and MAXIMUM (is_max = true) queries.
// Time per insert / query: O(log(X_MAX - X_MIN))
// Space per insert:        O(log(X_MAX - X_MIN)) nodes
struct PersistentLiChao {
    struct Line {
        ll m, c;
        ll eval(ll x) const { return m * x + c; }
    };

    struct Node {
        Line line;
        int l = 0, r = 0; // 0 represents the null node
    };

    ll X_MIN, X_MAX;
    bool is_max;
    vector<Node> tr;

    PersistentLiChao(ll x_min, ll x_max, bool maximum = false, int reserve_nodes = 0)
        : X_MIN(x_min), X_MAX(x_max), is_max(maximum) {
        if (reserve_nodes > 0) tr.reserve(reserve_nodes);
        // Node 0 is the sentinel null node with a dummy line (0*x + INF)
        tr.push_back({{0, INF}, 0, 0});
    }

    // Clone an existing node (or create a fresh one if prev_root == 0)
    int clone(int u) {
        tr.push_back(tr[u]);
        return (int)tr.size() - 1;
    }

    int insert_impl(int prev, ll lo, ll hi, Line nw) {
        int cur = clone(prev);
        ll mid = lo + (hi - lo) / 2;

        bool left_better = nw.eval(lo) < tr[cur].line.eval(lo);
        bool mid_better  = nw.eval(mid) < tr[cur].line.eval(mid);

        if (mid_better) {
            swap(tr[cur].line, nw);
        }
        // Base case: single point interval, or the losing line is the dummy INF line
        if (lo == hi || nw.c == INF) {
            return cur;
        }

        if (left_better != mid_better) {
            int nxt = insert_impl(tr[cur].l, lo, mid, nw);
            tr[cur].l = nxt;
        } else {
            int nxt = insert_impl(tr[cur].r, mid + 1, hi, nw);
            tr[cur].r = nxt;
        }
        return cur;
    }

    ll query_impl(int u, ll lo, ll hi, ll x) const {
        if (u == 0) return INF;
        ll res = tr[u].line.eval(x);
        if (lo == hi) return res;
        ll mid = lo + (hi - lo) / 2;
        if (x <= mid) {
            return min(res, query_impl(tr[u].l, lo, mid, x));
        } else {
            return min(res, query_impl(tr[u].r, mid + 1, hi, x));
        }
    }

    // Public API 1: Insert line y = m*x + c into `prev_root`
    // Returns the NEW version root (leaves `prev_root` 100% untouched!)
    int insert(int prev_root, ll m, ll c) {
        if (is_max) { m = -m; c = -c; } // Internally always maintains minimum
        return insert_impl(prev_root, X_MIN, X_MAX, {m, c});
    }

    // Public API 2: Query best y = m*x + c at coordinate `x` in version `root`
    // Returns +INF (for min) or -INF (for max) if `root` has no lines
    ll query(int root, ll x) const {
        ll ans = query_impl(root, X_MIN, X_MAX, x);
        if (ans >= INF / 2) return is_max ? -INF : INF;
        return is_max ? -ans : ans;
    }
};

int main() {
    // Coordinate range [-10^9, 10^9], false = MINIMUM queries
    // Pre-reserve ~200,000 * 32 nodes if N = 2e5
    PersistentLiChao lct(-1e9, 1e9, false);

    vector<int> root(10, 0); // root[0] = 0 is the empty tree

    // Version 1: Insert y = 2x + 3 into empty tree (root[0])
    root[1] = lct.insert(root[0], 2, 3);

    // Version 2: Insert y = -x + 10 on top of Version 1
    root[2] = lct.insert(root[1], -1, 10);

    // Version 3: Branch directly off Version 1 (ignoring Version 2!) and insert y = 0x + 5
    root[3] = lct.insert(root[1], 0, 5);

    cout << lct.query(root[1], 4) << "\n"; // Only {2x+3}        -> 2(4)+3 = 11
    cout << lct.query(root[2], 4) << "\n"; // {2x+3, -x+10}      -> min(11, 6) = 6
    cout << lct.query(root[3], 4) << "\n"; // {2x+3, 5}          -> min(11, 5) = 5
}
