#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct RollingHash {
    static const ll MOD = 1000004249; // Large prime modulus
    static ll P;                      // Base multiplier
    static vector<ll> p_pow;          // Precalculated powers of P

    int n;
    vector<ll> h;

    // Initialize or extend the power table up to 'sz'
    static void ensure_pwrs(int sz) {
        if (p_pow.empty()) {
            // Random base generator seeded with high-resolution clock
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            P = uniform_int_distribution<ll>(257, MOD - 2)(rng);
            p_pow = {1};
        }
        while ((int)p_pow.size() <= sz) {
            p_pow.push_back((p_pow.back() * P) % MOD);
        }
    }

    // Default constructor
    RollingHash() : n(0) {}

    // Constructor that builds the hash prefix array for a string s
    RollingHash(const string &s) {
        n = s.size();
        ensure_pwrs(n);
        h.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            ll val = (unsigned char)s[i] + 1LL; // 1-based indexing for chars to avoid 0 ambiguity
            h[i + 1] = (h[i] * P + val) % MOD;
        }
    }

    // Get the hash of substring s[l ... r] in O(1) time (0-based indices)
    ll getHash(int l, int r) const {
        if (l > r) return 0;
        int len = r - l + 1;
        ll res = (h[r + 1] - h[l] * p_pow[len] % MOD + MOD) % MOD;
        return res;
    }
};

// Static member definitions
ll RollingHash::P = 0;
vector<ll> RollingHash::p_pow;
