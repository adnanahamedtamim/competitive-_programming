#include <bits/stdc++.h>
using namespace std;
 
#define int long long
 
const int MOD = 1e9 + 7;
const int P = 31;
const int MAXN = 200005;
 
int p_pow[MAXN];
string S;
 
struct Node {
    int fhash;
    int rhash;
    int len;
};
 
Node tree[4 * MAXN];
 
Node combine(Node left, Node right) {
    if (left.len == 0) return right;
    if (right.len == 0) return left;
    
    Node res;
    res.len = left.len + right.len;
    res.fhash = (left.fhash + right.fhash * p_pow[left.len]) % MOD;
    res.rhash = (right.rhash + left.rhash * p_pow[right.len]) % MOD;
    return res;
}
 
void build(int node, int start, int end) {
    if (start == end) {
        tree[node].fhash = S[start] - 'a' + 1;
        tree[node].rhash = S[start] - 'a' + 1;
        tree[node].len = 1;
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
}
 
void update(int node, int start, int end, int idx, char val) {
    if (start == end) {
        tree[node].fhash = val - 'a' + 1;
        tree[node].rhash = val - 'a' + 1;
        return;
    }
    int mid = (start + end) / 2;
    if (start <= idx && idx <= mid) {
        update(2 * node, start, mid, idx, val);
    } else {
        update(2 * node + 1, mid + 1, end, idx, val);
    }
    tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
}
 
Node query(int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return {0, 0, 0};
    }
    if (l <= start && end <= r) {
        return tree[node];
    }
    int mid = (start + end) / 2;
    Node left = query(2 * node, start, mid, l, r);
    Node right = query(2 * node + 1, mid + 1, end, l, r);
    return combine(left, right);
}
 
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    int n, m;
    cin >> n >> m;
    
    cin >> S;
    S = " " + S;
 
    p_pow[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        p_pow[i] = (p_pow[i - 1] * P) % MOD;
    }
 
    build(1, 1, n);
 
    while (m--) {
        int type;
        cin >> type;
        if (type == 1) {
            int k;
            char x;
            cin >> k >> x;
            update(1, 1, n, k, x);
        } else {
            int a, b;
            cin >> a >> b;
            Node res = query(1, 1, n, a, b);
            if (res.fhash == res.rhash) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
 
    return 0;

}
