
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;

struct Node {
    // ll sum = 0;
    // ll mx = LLONG_MIN;
    ll mn = LLONG_MAX;

    Node() {}

    Node(ll x) {
        // sum = x;
        // mx = x;
        mn = x;
    }
};

Node merge(Node a, Node b) {
    Node res;
    // res.sum = a.sum + b.sum;
    // res.mx = max(a.mx, b.mx);
    res.mn = min(a.mn, b.mn);
    return res;
}

class SegmentTree {
private:
    ll n = 0;
    vector<Node> tree;
    vll lazyAdd, lazyAssign;
    vector<bool> hasAssign;

    bool validRange(ll l, ll r) const {
        return !(l < 0 || r > n - 1 || l > r);
    }

    void pull(ll si) {
        tree[si] = merge(tree[si * 2 + 1], tree[si * 2 + 2]);
    }

    void constructSTUtil(vll &arr, ll ss, ll se, ll si) {
        if (ss == se) {
            tree[si] = Node(arr[ss]);
            return;
        }

        ll mid = (ss + se) / 2;

        constructSTUtil(arr, ss, mid, si * 2 + 1);
        constructSTUtil(arr, mid + 1, se, si * 2 + 2);

        pull(si);
    }

    void applyAssign(ll si, ll ss, ll se, ll x) {
        // tree[si].sum = (se - ss + 1) * x;
        // tree[si].mx = x;
        tree[si].mn = x;

        hasAssign[si] = true;
        lazyAssign[si] = x;
        lazyAdd[si] = 0;
    }

    void applyAdd(ll si, ll ss, ll se, ll diff) {
        // tree[si].sum += (se - ss + 1) * diff;
        // tree[si].mx += diff;
        tree[si].mn += diff;

        if (hasAssign[si])
            lazyAssign[si] += diff;
        else
            lazyAdd[si] += diff;
    }

    void pushDown(ll si, ll ss, ll se) {
        if (ss == se) return;

        ll mid = (ss + se) / 2;
        ll L = si * 2 + 1;
        ll R = si * 2 + 2;

        if (hasAssign[si]) {
            applyAssign(L, ss, mid, lazyAssign[si]);
            applyAssign(R, mid + 1, se, lazyAssign[si]);

            hasAssign[si] = false;
            lazyAssign[si] = 0;
        }

        if (lazyAdd[si] != 0) {
            applyAdd(L, ss, mid, lazyAdd[si]);
            applyAdd(R, mid + 1, se, lazyAdd[si]);

            lazyAdd[si] = 0;
        }
    }

    void updateRangeUtil(ll si, ll ss, ll se,
                         ll us, ll ue, ll diff) {
        if (ss > ue || se < us) return;

        if (us <= ss && se <= ue) {
            applyAdd(si, ss, se, diff);
            return;
        }

        pushDown(si, ss, se);

        ll mid = (ss + se) / 2;

        updateRangeUtil(si * 2 + 1, ss, mid, us, ue, diff);
        updateRangeUtil(si * 2 + 2, mid + 1, se, us, ue, diff);

        pull(si);
    }

    void assignRangeUtil(ll si, ll ss, ll se,
                         ll us, ll ue, ll x) {
        if (ss > ue || se < us) return;

        if (us <= ss && se <= ue) {
            applyAssign(si, ss, se, x);
            return;
        }

        pushDown(si, ss, se);

        ll mid = (ss + se) / 2;

        assignRangeUtil(si * 2 + 1, ss, mid, us, ue, x);
        assignRangeUtil(si * 2 + 2, mid + 1, se, us, ue, x);

        pull(si);
    }

    Node getNodeUtil(ll si, ll ss, ll se, ll qs, ll qe) {
        if (ss > qe || se < qs)
            return Node();

        if (qs <= ss && se <= qe)
            return tree[si];

        pushDown(si, ss, se);

        ll mid = (ss + se) / 2;

        Node left = getNodeUtil(si * 2 + 1, ss, mid, qs, qe);
        Node right = getNodeUtil(si * 2 + 2, mid + 1, se, qs, qe);

        if (ss > qe || se < qs) return Node();
        if (qs > mid) return right;
        if (qe <= mid) return left;

        return merge(left, right);
    }

public:
    SegmentTree() {}

    SegmentTree(vll &arr) {
        build(arr);
    }

    void build(vll &arr) {
        n = arr.size();

        tree.assign(4 * n, Node());
        lazyAdd.assign(4 * n, 0);
        lazyAssign.assign(4 * n, 0);
        hasAssign.assign(4 * n, false);

        if (n > 0)
            constructSTUtil(arr, 0, n - 1, 0);
    }

    void updateRange(ll l, ll r, ll diff) {
        if (!validRange(l, r)) {
            cout << "Invalid Input\n";
            return;
        }

        updateRangeUtil(0, 0, n - 1, l, r, diff);
    }

    void assignRange(ll l, ll r, ll x) {
        if (!validRange(l, r)) {
            cout << "Invalid Input\n";
            return;
        }

        assignRangeUtil(0, 0, n - 1, l, r, x);
    }

    Node query(ll l, ll r) {
        if (!validRange(l, r)) {
            cout << "Invalid Input\n";
            return Node();
        }

        return getNodeUtil(0, 0, n - 1, l, r);
    }

    // ll getSum(ll l, ll r) {
    //     return query(l, r).sum;
    // }

    // ll getMax(ll l, ll r) {
    //     return query(l, r).mx;
    // }

    ll getMin(ll l, ll r) {
        return query(l, r).mn;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n = 10;
    vll arr(n, 0);

    SegmentTree st(arr);

    // st.updateRange(2, 7, 5);
    // cout << st.getSum(0, 9) << '\n';

    st.assignRange(0, 9, 3);
    // cout << st.getSum(0, 9) << '\n';

    // st.updateRange(3, 5, 2);
    // cout << st.getSum(0, 9) << '\n';

    // cout << st.getSum(4, 6) << '\n';
    // cout << st.getMax(0, 9) << '\n';
    cout << st.getMin(0, 9) << '\n';

    return 0;
}
