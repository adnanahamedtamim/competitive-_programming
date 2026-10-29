#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<int> simple_sieve(ll limit) {
    vector<bool> isPrime(limit + 1, true);
    vector<int> primes;

    if (limit >= 0) isPrime[0] = false;
    if (limit >= 1) isPrime[1] = false;

    for (ll i = 2; i * i <= limit; i++) {
        if (isPrime[i]) {
            for (ll j = i * i; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }

    for (ll i = 2; i <= limit; i++) {
        if (isPrime[i]) primes.push_back((int)i);
    }

    return primes;
}

vector<ll> segmented_sieve(ll L, ll R) {
    if (L > R || R < 2) return {};

    L = max(L, 2LL);

    ll limit = sqrtl(R);
    while ((limit + 1) <= R / (limit + 1)) limit++;
    while (limit > R / limit) limit--;

    vector<int> primes = simple_sieve(limit);
    vector<bool> isPrime(R - L + 1, true);

    for (ll p : primes) {
        ll start = max(p * p, ((L + p - 1) / p) * p);

        for (ll j = start; j <= R; j += p) {
            isPrime[j - L] = false;
        }
    }

    vector<ll> result;

    for (ll i = L; i <= R; i++) {
        if (isPrime[i - L]) {
            result.push_back(i);
        }
    }

    return result;
}

int main() {
    ll L, R;
    cin >> L >> R;

    vector<ll> primes = segmented_sieve(L, R);

    for (ll p : primes) {
        cout << p << ' ';
    }
    cout << '\n';

    return 0;
}
