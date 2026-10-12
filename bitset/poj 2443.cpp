```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MAXV = 10001;
bitset<1001> has[MAXV];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    for (ll i = 1; i <= n; i++) {
        ll c;
        cin >> c;

        while (c--) {
            ll x;
            cin >> x;
            has[x].set(i);
        }
    }

    ll q;
    cin >> q;

    while (q--) {
        ll x, y;
        cin >> x >> y;

        if ((has[x] & has[y]).any())
            cout << "Yes\n";
        else
            cout << "No\n";
    }

    return 0;
}
```
