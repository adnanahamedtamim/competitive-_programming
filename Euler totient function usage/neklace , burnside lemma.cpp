//Suppose you have a necklace with 
//n beads, and each bead can be painted in one of K colours.
//Two necklaces are considered the same if one can be rotated to look exactly like the other.
//In other words, rotating a necklace does not create a new arrangement.
//We want to find the number of distinct necklaces.

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1000000007;

ll modpow(ll a, ll e) {
    ll res = 1;
    a %= MOD;

    while (e > 0) {
        if (e & 1)
            res = res * a % MOD;

        a = a * a % MOD;
        e >>= 1;
    }

    return res;
}

ll phi(ll n) {
    ll ans = n;

    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            ans -= ans / p;

            while (n % p == 0)
                n /= p;
        }
    }

    if (n > 1)
        ans -= ans / n;

    return ans;
}

ll necklace(ll n, ll K) {
    ll sum = 0;

    for (ll d = 1; d * d <= n; d++) {
        if (n % d != 0)
            continue;

        ll other = n / d;

        sum = (sum + phi(other) % MOD
                    * modpow(K, d) % MOD) % MOD;


        if (d != other) {
            sum = (sum + phi(d) % MOD
                        * modpow(K, other) % MOD) % MOD;
        }
    }


    return sum * modpow(n % MOD, MOD - 2) % MOD;
}

int main() {
    ll n, K;
    cin >> n >> K;

    cout << necklace(n, K) << '\n';
}
