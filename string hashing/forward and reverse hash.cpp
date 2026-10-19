//  calculates forward and reverse hash of all the 4 len substring of a given string
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll p = 31;
const ll mod = 1000000007;

ll binpow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s = "abcdef";
    int n = s.size(), k = 4;

    if (k > n || k <= 0) return 0;

    ll pw = binpow(p, k - 1);
    ll invp = binpow(p, mod - 2);

    ll hf = 0, hr = 0;
    ll cur = 1;

    for (int i = 0; i < k; i++) {
        ll x = s[i] - 'a' + 1;
        hf = (hf * p + x) % mod;
        hr = (hr + x * cur) % mod;
        cur = cur * p % mod;
    }

    cout << s.substr(0, k)
         << " forward = " << hf
         << " reverse = " << hr << '\n';

    for (int i = k; i < n; i++) {
        ll out = s[i - k] - 'a' + 1;
        ll in = s[i] - 'a' + 1;

        hf = (hf - out * pw % mod + mod) % mod;
        hf = (hf * p + in) % mod;

        hr = (hr - out + mod) % mod;
        hr = hr * invp % mod;
        hr = (hr + in * pw % mod) % mod;

        cout << s.substr(i - k + 1, k)
             << " forward = " << hf
             << " reverse = " << hr << '\n';
    }

    return 0;
}
```
