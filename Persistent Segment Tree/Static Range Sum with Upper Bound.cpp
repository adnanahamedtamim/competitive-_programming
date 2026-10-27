#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Data {
    ll cnt = 0;
    ll sum = 0;
};

Data merge(const Data& a, const Data& b) {
    return {a.cnt + b.cnt, a.sum + b.sum};
}

Data subtract(const Data& a, const Data& b) {
    return {a.cnt - b.cnt, a.sum - b.sum};
}

struct Node {
    Data data;
    Node *l = nullptr, *r = nullptr;
};

Data get(Node* node) {
    return node ? node->data : Data{};
}

Node* update(Node* prev, int tl, int tr, int pos, ll val) {
    Node* cur = prev ? new Node(*prev) : new Node();

    if (tl == tr) {
        cur->data = merge(cur->data, Data{1, val});
        return cur;
    }

    int tm = (tl + tr) / 2;

    if (pos <= tm)
        cur->l = update(cur->l, tl, tm, pos, val);
    else
        cur->r = update(cur->r, tm + 1, tr, pos, val);

    cur->data = merge(get(cur->l), get(cur->r));

    return cur;
}

Data query(Node* R, Node* L, int tl, int tr, int ql, int qr) {
    if (qr < tl || tr < ql || ql > qr || R == L)
        return {};

    if (ql <= tl && tr <= qr)
        return subtract(get(R), get(L));

    int tm = (tl + tr) / 2;

    Data left = query(
        R ? R->l : nullptr,
        L ? L->l : nullptr,
        tl, tm, ql, qr
    );

    Data right = query(
        R ? R->r : nullptr,
        L ? L->r : nullptr,
        tm + 1, tr, ql, qr
    );

    return merge(left, right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    vector<ll> a(N + 1), comp;

    for (int i = 1; i <= N; i++) {
        cin >> a[i];
        comp.push_back(a[i]);
    }

    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());

    int M = comp.size();
    vector<Node*> root(N + 1, nullptr);

    for (int i = 1; i <= N; i++) {
        int pos = lower_bound(comp.begin(), comp.end(), a[i])
                  - comp.begin() + 1;

        root[i] = update(root[i - 1], 1, M, pos, a[i]);
    }

    while (Q--) {
        int l, r;
        ll x;
        cin >> l >> r >> x;

        int idx = upper_bound(comp.begin(), comp.end(), x)
                  - comp.begin();

        Data ans = query(root[r], root[l - 1], 1, M, 1, idx);

        cout << ans.cnt << ' ' << ans.sum << '\n';
    }
}
