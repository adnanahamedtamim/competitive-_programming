```cpp
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define pb push_back
#define all(x) x.begin(), x.end()

const ll p1 = 7919;
const ll mod1 = 1000004249;
const ll p2 = 2203;
const ll mod2 = 1000000289;

vector<ll> pwr1, pwr2;

void precalc(int n) {
    pwr1.assign(n + 1, 1);
    pwr2.assign(n + 1, 1);

    for (int i = 1; i <= n; i++) {
        pwr1[i] = pwr1[i - 1] * p1 % mod1;
        pwr2[i] = pwr2[i - 1] * p2 % mod2;
    }
}

struct Hash {
    int n;
    vector<ll> h1, h2;

    Hash() {
        n = 0;
    }

    Hash(const string &s) {
        n = s.size();
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] + (s[i] + 1LL) * pwr1[i]) % mod1;
            h2[i + 1] = (h2[i] + (s[i] + 1LL) * pwr2[i]) % mod2;
        }
    }

    pair<ll, ll> getHash(int l, int r) {
        if (l > r) return {0, 0};

        ll x = (h1[r + 1] - h1[l] + mod1) % mod1;
        ll y = (h2[r + 1] - h2[l] + mod2) % mod2;

        x = x * pwr1[n - 1 - l] % mod1;
        y = y * pwr2[n - 1 - l] % mod2;

        return {x, y};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    precalc(s.size());

    Hash hs(s);

    int l1, r1, l2, r2;
    cin >> l1 >> r1 >> l2 >> r2;

    cout << (hs.getHash(l1, r1) == hs.getHash(l2, r2)
                 ? "Equal"
                 : "Not Equal")
         << '\n';

    return 0;
}
```
