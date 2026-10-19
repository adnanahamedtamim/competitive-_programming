#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

typedef long long ll;

// Custom hash for tuple/vector of remainders across our random primes
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(const vector<ll>& v) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h = v.size() + FIXED_RANDOM;
        for (auto x : v) {
            h ^= splitmix64(x + FIXED_RANDOM) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        }
        return h;
    }
};

template <class K, class V>
using gmap = gp_hash_table<K, V, custom_hash>;

// Function to parse a 1000-digit string modulo a prime x
ll string_mod(const string &s, ll mod) {
    ll res = 0;
    for (char c : s) {
        res = (res * 10 + (c - '0')) % mod;
    }
    return res;
}

// Function to check if a number is prime
bool is_prime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> s(n);
    for (int i = 0; i < n; i++) {
        cin >> s[i];
    }

    // Step 1: Choose a set S of randomly chosen primes between 10^9 and 2*10^9
    // Following editorial: 3 or 4 random primes are sufficient to avoid collisions entirely.
    mt19937_64 rng(1337); // Fixed seed or steady_clock for randomness
    uniform_int_distribution<ll> dist(1000000000LL, 2000000000LL);

    vector<ll> S;
    while (S.size() < 3) { // 3 random primes
        ll candidate = dist(rng);
        if (is_prime(candidate)) {
            // Ensure no duplicate primes
            bool unique = true;
            for (ll p : S) if (p == candidate) unique = false;
            if (unique) S.push_back(candidate);
        }
    }

    int num_primes = S.size();

    // Step 2: Precalculate Ak modulo x for all Ak and all x in S
    // hashes[i] will store a vector of remainders for string s[i] across all primes in S
    vector<vector<ll>> hashes(n, vector<ll>(num_primes));
    gmap<vector<ll>, int> freq;

    for (int i = 0; i < n; i++) {
        for (int p_idx = 0; p_idx < num_primes; p_idx++) {
            hashes[i][p_idx] = string_mod(s[i], S[p_idx]);
        }
        freq[hashes[i]]++;
    }

    long long ans = 0;

    // Step 3: Evaluate Ai * Aj modulo x for all pairs and check if it exists in freq
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<ll> target(num_primes);
            for (int p_idx = 0; p_idx < num_primes; p_idx++) {
                target[p_idx] = (hashes[i][p_idx] * hashes[j][p_idx]) % S[p_idx];
            }

            auto it = freq.find(target);
            if (it != freq.end()) {
                ans += it->second;
            }
        }
    }

    cout << ans << "\n";

    return 0;
}
