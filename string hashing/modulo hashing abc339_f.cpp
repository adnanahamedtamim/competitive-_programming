#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

typedef long long ll;

// 1. Custom hash struct for pair<ll, ll> to use gp_hash_table safely
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(const pair<ll, ll>& p) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h1 = splitmix64(p.first + FIXED_RANDOM);
        uint64_t h2 = splitmix64(p.second + FIXED_RANDOM);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

template <class K, class V>
using gmap = gp_hash_table<K, V, custom_hash>;

// Function to compute string modulo a large prime
ll string_mod(const string &s, ll mod) {
    ll res = 0;
    for (char c : s) {
        res = (res * 10 + (c - '0')) % mod;
    }
    return res;
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

    const ll MOD1 = 1000000007;
    const ll MOD2 = 1000000009;

    vector<ll> h1(n), h2(n);
    gmap<pair<ll, ll>, int> freq;

    // Precompute modulo hashes for all 1000-digit numbers and store frequencies
    for (int i = 0; i < n; i++) {
        h1[i] = string_mod(s[i], MOD1);
        h2[i] = string_mod(s[i], MOD2);
        freq[{h1[i], h2[i]}]++;
    }

    long long ans = 0;

    // O(N^2) double loop to check A[i] * A[j] == A[k]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ll prod_h1 = (h1[i] * h1[j]) % MOD1;
            ll prod_h2 = (h2[i] * h2[j]) % MOD2;

            pair<ll, ll> target = {prod_h1, prod_h2};
            auto it = freq.find(target);
            if (it != freq.end()) {
                ans += it->second;
            }
        }
    }

    cout << ans << "\n";

    return 0;
}
