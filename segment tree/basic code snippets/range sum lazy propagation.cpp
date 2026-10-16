#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<vector<ll>> vvll;

class SegmentTree {
private:
    ll n = 0;
    vll tree;
    vll lazyAdd;             // pending range-add
    vll lazyAssign;          // pending range-assign value
    vector<bool> hasAssign;  // is an assign pending?

    bool validRange(ll l, ll r) const {
        return !(l < 0 || r > n - 1 || l > r);
    }

    void constructSTUtil(vll& arr, ll ss, ll se, ll si) {
        if (ss > se) return;

        if (ss == se) {
            tree[si] = arr[ss];
            return;
        }

        ll mid = (ss + se) / 2;

        constructSTUtil(arr, ss, mid, si * 2 + 1);
        constructSTUtil(arr, mid + 1, se, si * 2 + 2);

        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    void applyAssign(ll si, ll ss, ll se, ll x) {
        tree[si] = (se - ss + 1) * x;
        hasAssign[si] = true;
        lazyAssign[si] = x;
        lazyAdd[si] = 0;  // assign overrides any pending add
    }

    void applyAdd(ll si, ll ss, ll se, ll diff) {
        tree[si] += (se - ss + 1) * diff;
        if (hasAssign[si]) lazyAssign[si] += diff;  // fold into assigned value
        else lazyAdd[si] += diff;
    }

    void pushDown(ll si, ll ss, ll se) {
        if (ss == se) return;

        ll mid = (ss + se) / 2;
        ll L = si * 2 + 1, R = si * 2 + 2;

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

    void updateRangeUtil(ll si, ll ss, ll se, ll us, ll ue, ll diff) {
        if (ss > se || ss > ue || se < us) return;

        if (ss >= us && se <= ue) {
            applyAdd(si, ss, se, diff);
            return;
        }

        pushDown(si, ss, se);

        ll mid = (ss + se) / 2;
        updateRangeUtil(si * 2 + 1, ss, mid, us, ue, diff);
        updateRangeUtil(si * 2 + 2, mid + 1, se, us, ue, diff);

        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    void assignRangeUtil(ll si, ll ss, ll se, ll us, ll ue, ll x) {
        if (ss > se || ss > ue || se < us) return;

        if (ss >= us && se <= ue) {
            applyAssign(si, ss, se, x);
            return;
        }

        pushDown(si, ss, se);

        ll mid = (ss + se) / 2;
        assignRangeUtil(si * 2 + 1, ss, mid, us, ue, x);
        assignRangeUtil(si * 2 + 2, mid + 1, se, us, ue, x);

        tree[si] = tree[si * 2 + 1] + tree[si * 2 + 2];
    }

    ll getSumUtil(ll ss, ll se, ll qs, ll qe, ll si) {
        if (ss > se || ss > qe || se < qs) return 0;

        if (ss >= qs && se <= qe) return tree[si];

        pushDown(si, ss, se);

        ll mid = (ss + se) / 2;
        return getSumUtil(ss, mid, qs, qe, 2 * si + 1)
             + getSumUtil(mid + 1, se, qs, qe, 2 * si + 2);
    }

public:
    SegmentTree() {}
    SegmentTree(vll& arr) { build(arr); }

    // build the tree from an array
    void build(vll& arr) {
        n = arr.size();
        tree.assign(4 * n, 0);
        lazyAdd.assign(4 * n, 0);
        lazyAssign.assign(4 * n, 0);
        hasAssign.assign(4 * n, false);

        constructSTUtil(arr, 0, n - 1, 0);
    }

    // add diff to every element in [l, r]
    void updateRange(ll l, ll r, ll diff) {
        if (!validRange(l, r)) {
            cout << "Invalid Input\n";
            return;
        }
        updateRangeUtil(0, 0, n - 1, l, r, diff);
    }

    // replace every element in [l, r] with x
    void assignRange(ll l, ll r, ll x) {
        if (!validRange(l, r)) {
            cout << "Invalid Input\n";
            return;
        }
        assignRangeUtil(0, 0, n - 1, l, r, x);
    }

    // sum of elements in [l, r]
    ll getSum(ll l, ll r) {
        if (!validRange(l, r)) {
            cout << "Invalid Input\n";
            return -1;
        }
        return getSumUtil(0, n - 1, l, r, 0);
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n = 10;
    vll arr(n);

    SegmentTree st(arr);   // or: SegmentTree st; st.build(arr);

    st.updateRange(2, 7, 5);
    cout << st.getSum(0, 9) << endl;   // 30

    st.assignRange(4, 8, 3);
    cout << st.getSum(0, 9) << endl;   // 25

    st.updateRange(3, 5, 2);
    cout << st.getSum(0, 9) << endl;   // 31

    cout << st.getSum(4, 6) << endl;   // 13
}
