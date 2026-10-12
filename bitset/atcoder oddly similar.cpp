```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MAXN = 2000;
const ll MAXV = 1000;

ll a[MAXN][MAXN];
bitset<MAXN> bt[MAXN], bs[MAXV];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;

    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }

    for (ll j = 0; j < m; j++) {
        // Group rows having the same value in column j.
        for (ll i = 0; i < n; i++) {
            bs[a[i][j]].set(i);
        }

        // Toggle all rows matching row i in this column.
        for (ll i = 0; i < n; i++) {
            bt[i] ^= bs[a[i][j]];
        }

        // Clear this column's groups before processing the next column.
        for (ll i = 0; i < n; i++) {
            bs[a[i][j]].reset(i);
        }
    }

    ll ans = 0;

    for (ll i = 0; i < n; i++) {
        ans += bt[i].count();
    }

    // If m is odd, every row matches itself an odd number of times.
    if (m & 1) ans -= n;

    // Each distinct pair is counted in both directions.
    cout << ans / 2 << '\n';

    return 0;
}
```
