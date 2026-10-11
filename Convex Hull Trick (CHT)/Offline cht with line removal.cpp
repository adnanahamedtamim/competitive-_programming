#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Offline CHT with deletions.
// Usage:
//   DynamicCHT cht(true);          // true = max, false = min
//   cht.add(k, m);                 // insert y = k*x + m
//   cht.remove(k, m);              // delete one copy of y = k*x + m
//   int id = cht.query(x);         // ask best value at x (returns query index)
//   vector<ll> ans = cht.solve();  // ans[id] = answer (LLONG_MIN/LLONG_MAX if no lines)
// Complexity: O(Q log Q log X)
struct DynamicCHT {
    explicit DynamicCHT(bool maximum = true) : mx(maximum) {}

    void add(ll k, ll m)    { ops.push_back({0, k, m}); }
    void remove(ll k, ll m) { ops.push_back({1, k, m}); }
    int  query(ll x)        { ops.push_back({2, x, 0}); return nq++; }

    vector<ll> solve() {
        int Q = ops.size();
        res.assign(nq, mx ? LLONG_MIN : LLONG_MAX);
        for (auto& o : ops) if (o.t == 2) xs.push_back(o.a);
        if (xs.empty()) return res;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        tr.assign(4 * n, {0, LLONG_MIN});

        P = 1; while (P < Q) P <<= 1;
        seg.assign(2 * P, {});
        hasQ.assign(2 * P, 0);
        qid.assign(Q, -1);

        map<pair<ll, ll>, vector<int>> open;
        for (int i = 0, q = 0; i < Q; i++) {
            auto [t, a, b] = ops[i];
            if (t == 0) open[{a, b}].push_back(i);
            else if (t == 1) {
                auto it = open.find({a, b});
                if (it != open.end() && !it->second.empty()) {
                    place(it->second.back(), i - 1, {a, b});
                    it->second.pop_back();
                }
            } else { qid[i] = q++; hasQ[P + i] = 1; }
        }
        for (auto& [ln, v] : open)
            for (int s : v) place(s, Q - 1, {ln.first, ln.second});
        for (int i = P - 1; i >= 1; i--) hasQ[i] = hasQ[2 * i] | hasQ[2 * i + 1];

        dfs(1);
        return res;
    }

private:
    struct Line { ll k, m; ll operator()(ll x) const { return k * x + m; } };
    struct Op { int t; ll a, b; };

    bool mx; int nq = 0, P = 1, n = 0;
    vector<Op> ops;
    vector<vector<Line>> seg;
    vector<char> hasQ;
    vector<int> qid;
    vector<ll> xs, res;
    vector<Line> tr;
    vector<pair<int, Line>> hist;

    // put line into segment tree over time, alive on [l, r]
    void place(int l, int r, Line ln) {
        if (!mx) ln = {-ln.k, -ln.m};
        for (l += P, r += P + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) seg[l++].push_back(ln);
            if (r & 1) seg[--r].push_back(ln);
        }
    }

    // Li Chao insert (max) with rollback history
    void ins(Line nw, int node, int lo, int hi) {
        while (true) {
            int mid = (lo + hi) / 2;
            bool lef = nw(xs[lo]) > tr[node](xs[lo]);
            bool md  = nw(xs[mid]) > tr[node](xs[mid]);
            if (md) { hist.push_back({node, tr[node]}); swap(tr[node], nw); }
            if (lo == hi || nw.m == LLONG_MIN) return;
            if (lef != md) { node = 2 * node;     hi = mid; }
            else           { node = 2 * node + 1; lo = mid + 1; }
        }
    }

    ll get(int i) {
        int node = 1, lo = 0, hi = n - 1;
        ll r = LLONG_MIN;
        while (true) {
            r = max(r, tr[node](xs[i]));
            if (lo == hi) return r;
            int mid = (lo + hi) / 2;
            if (i <= mid) { node = 2 * node;     hi = mid; }
            else          { node = 2 * node + 1; lo = mid + 1; }
        }
    }

    void dfs(int node) {
        if (!hasQ[node]) return;                 // skip subtrees with no queries
        size_t snap = hist.size();
        for (auto& ln : seg[node]) ins(ln, 1, 0, n - 1);
        if (node >= P) {
            int i = node - P;
            int j = lower_bound(xs.begin(), xs.end(), ops[i].a) - xs.begin();
            ll v = get(j);
            if (v != LLONG_MIN) res[qid[i]] = mx ? v : -v;
        } else {
            dfs(2 * node);
            dfs(2 * node + 1);
        }
        while (hist.size() > snap) { tr[hist.back().first] = hist.back().second; hist.pop_back(); }
    }
};

int main() {
   /*
    solve() is where all the real work happens. 
    Before it’s called, add, remove and query only write the operations down in a list. 
    solve() then processes that whole list at once and fills in every query’s answer
   */ 

    DynamicCHT cmax(true), cmin(false);

    for (auto* c : {&cmax, &cmin}) {
        c->add(1, 0);      // y = x
        c->add(-1, 0);     // y = -x
        c->add(0, 0);      // y = 0
    }
    int a1 = cmax.query(5), b1 = cmin.query(5);  //a1,b2,a2,b2 is the indexes in rmax,rmin where the ans of that query is stored
    cmax.remove(1, 0);  cmin.remove(-1, 0);
    int a2 = cmax.query(5), b2 = cmin.query(5);

    auto rmax = cmax.solve(), rmin = cmin.solve();
    cout << rmax[a1] << " " << rmax[a2] << "\n";   // 5 0
    cout << rmin[b1] << " " << rmin[b2] << "\n";   // -5 0
}
