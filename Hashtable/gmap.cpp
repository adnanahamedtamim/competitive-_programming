#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

typedef long long ll;

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    static uint64_t get_rnd() {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return FIXED_RANDOM;
    }

    template <typename T, typename enable_if<is_integral<T>::value, int>::type = 0>
    size_t operator()(T x) const {
        return splitmix64(uint64_t(x) + get_rnd());
    }

    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& p) const {
        uint64_t h1 = (*this)(p.first);
        uint64_t h2 = (*this)(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }

    template <typename T>
    size_t operator()(const vector<T>& v) const {
        uint64_t h = v.size();
        for (const auto& x : v) {
            uint64_t hx = (*this)(x);
            h ^= hx + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        }
        return h;
    }

    size_t operator()(const string& s) const {
        uint64_t h = s.size() + get_rnd();
        for (unsigned char c : s) {
            h = splitmix64(h ^ c);
        }
        return h;
    }
};

template <class K, class V>
using umap = unordered_map<K, V, custom_hash>;

template <class K, class V>
using gmap = gp_hash_table<K, V, custom_hash>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // =========================================================================
    // EXAMPLE 1: gmap<ll, int> — Count subarrays with sum == X (Fastest!)
    // =========================================================================
    vector<ll> arr = {2, -1, 3, 5, -2, 3};
    ll target_sum = 4;

    gmap<ll, int> pref_cnt;
    pref_cnt[0] = 1; // Base case: prefix sum 0 seen once

    ll cur_sum = 0, ways = 0;
    for (ll x : arr) {
        cur_sum += x;
        // IMPORTANT: Use .find() != .end() so we don't accidentally insert (cur_sum - target_sum)!
        auto it = pref_cnt.find(cur_sum - target_sum);
        if (it != pref_cnt.end()) {
            ways += it->second;
        }
        pref_cnt[cur_sum]++;
    }
    cout << "Subarrays with sum 4: " << ways << "\n";


    // =========================================================================
    // EXAMPLE 2: gmap<pair<ll, ll>, int> — Store DoubleHash pairs or 2D points!
    // =========================================================================
    // With custom_hash, you can use pair<ll, ll> directly as a key in gmap!
    gmap<pair<ll, ll>, int> point_id;

    point_id[{1000000007LL, 998244353LL}] = 1; // e.g., storing a DoubleHash pair!
    point_id[{5LL, -10LL}] = 2;

    pair<ll, ll> query_pt = {5LL, -10LL};
    if (point_id.find(query_pt) != point_id.end()) {
        cout << "Found point ID: " << point_id[query_pt] << "\n";
    }


    // =========================================================================
    // EXAMPLE 3: gmap<vector<ll>, int> — Hashing whole vectors as keys!
    // =========================================================================
    gmap<vector<ll>, int> vec_freq;
    vec_freq[{1, 2, 3}]++;
    vec_freq[{3, 2, 1}]++;
    vec_freq[{1, 2, 3}]++;

    cout << "Freq of {1, 2, 3}: " << vec_freq[{1, 2, 3}] << "\n"; // Outputs 2
    cout << "Freq of {3, 2, 1}: " << vec_freq[{3, 2, 1}] << "\n"; // Outputs 1


    // =========================================================================
    // EXAMPLE 4: umap<string, int> — When using umap, always reserve()!
    // =========================================================================
    umap<string, int> word_cnt;
    word_cnt.reserve(1024);          // Prevents slow rehashing
    word_cnt.max_load_factor(0.25);  // Keeps bucket chains very short

    word_cnt["hello"] = 10;
    word_cnt["world"] = 20;

    // umap supports .count() and structured bindings
    if (word_cnt.count("hello")) {
        word_cnt.erase("hello");
    }

    for (auto &[word, count] : word_cnt) {
        cout << word << " -> " << count << "\n";
    }

    return 0;
}
