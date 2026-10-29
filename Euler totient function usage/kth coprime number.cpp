// given two integer n,X.. where X,n<=10^18, find the kth number x which that x<=X and gcd(x,n)=1

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;
using ull = unsigned long long;

vector<ull> primes;

// Count positive integers x <= X with gcd(x, n) = 1.
i128 count_coprime(ull X) {
    int t = (int)primes.size();
    i128 ans = 0;

    for (int mask = 0; mask < (1 << t); mask++) {
        ull prod = 1;
        int bits = 0;

        for (int i = 0; i < t; i++) {
            if (mask & (1 << i)) {
                prod *= primes[i];
                bits++;
            }
        }

        i128 term = X / prod;

        if (bits & 1) ans -= term;
        else ans += term;
    }

    return ans;
}

// Find the k-th positive integer coprime to n.
ull kth_coprime(ull k) {
    ull lo = 1, hi = 1;

    while (count_coprime(hi) < (i128)k) {
        if (hi > ULLONG_MAX / 2)
            throw overflow_error("Search bound overflow");
        hi *= 2;
    }

    while (lo < hi) {
        ull mid = lo + (hi - lo) / 2;

        if (count_coprime(mid) >= (i128)k)
            hi = mid;
        else
            lo = mid + 1;
    }

    return lo;
}

int main() {
    ull n, k;
    cin >> n >> k;

    // Replace with distinct prime factors obtained
    // using Pollard-Rho factorization.
    // Example: n = 30 -> primes = {2, 3, 5}
    primes = {2, 3, 5};

    cout << kth_coprime(k) << '\n';
}
