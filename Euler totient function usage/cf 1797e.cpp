#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MAXA = 5000000;
const ll MAXN = 200000 + 5;

ll phi[MAXA + 1];
ll dep[MAXA + 1];
ll a[MAXN];
ll nxt[MAXN];

struct Node {
    ll sum;
    ll lca;
};

Node seg[4 * MAXN];

ll find(ll x) {
    if (nxt[x] == x) return x;
    return nxt[x] = find(nxt[x]);
}

ll getLCA(ll x, ll y) {
    if (x == 0) return y;
    if (y == 0) return x;

    while (dep[x] > dep[y])
        x = phi[x];

    while (dep[y] > dep[x])
        y = phi[y];

    while (x != y) {
        x = phi[x];
        y = phi[y];
    }

    return x;
}

Node mergeNode(Node a, Node b) {
    return { a.sum + b.sum, getLCA(a.lca, b.lca) };
}

void build(ll node, ll l, ll r) {
    if (l == r) {
        seg[node] = {dep[a[l]], a[l]};
        return;
    }

    ll mid = (l + r) / 2;

    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);

    seg[node] = mergeNode(seg[node * 2], seg[node * 2 + 1]);
}

void update(ll node, ll l, ll r, ll pos) {
    if (l == r) {
        seg[node] = {dep[a[pos]], a[pos]};
        return;
    }

    ll mid = (l + r) / 2;

    if (pos <= mid)
        update(node * 2, l, mid, pos);
    else
        update(node * 2 + 1, mid + 1, r, pos);

    seg[node] = mergeNode(seg[node * 2], seg[node * 2 + 1]);
}

Node query(ll node, ll l, ll r, ll ql, ll qr) {
    if (ql <= l && r <= qr)
        return seg[node];

    ll mid = (l + r) / 2;

    if (qr <= mid)
        return query(node * 2, l, mid, ql, qr);

    if (ql > mid)
        return query(node * 2 + 1, mid + 1, r, ql, qr);

    return mergeNode(
        query(node * 2, l, mid, ql, qr),
        query(node * 2 + 1, mid + 1, r, ql, qr)
    );
}

void init_phi() {
    for (ll i = 1; i <= MAXA; i++)
        phi[i] = i;

    for (ll i = 2; i <= MAXA; i++) {
        if (phi[i] == i) {
            for (ll j = i; j <= MAXA; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    dep[1] = 0;

    for (ll i = 2; i <= MAXA; i++)
        dep[i] = dep[phi[i]] + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    init_phi();

    ll n, q;
    cin >> n >> q;

    for (ll i = 1; i <= n; i++)  cin >> a[i];

    build(1, 1, n);

    for (ll i = 1; i <= n + 1; i++)   nxt[i] = i;

    for (ll i = n; i >= 1; i--) {
        if (a[i] == 1)
            nxt[i] = find(i + 1);
    }

    while (q--) {
        ll type, l, r;
        cin >> type >> l >> r;

        if (type == 1) {
            ll i = find(l);

            while (i <= r) {
                a[i] = phi[a[i]];
                update(1, 1, n, i);

                if (a[i] == 1)
                    nxt[i] = find(i + 1);
                    i = find(i + 1);
            }
        }
        else {
            Node res = query(1, 1, n, l, r);
            ll len = r - l + 1;

            ll ans = res.sum - len * dep[res.lca];
            cout << ans << endl;
        }
    }

    return 0;
}
