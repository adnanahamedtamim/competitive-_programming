#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

// Map to store a unique random 64-bit key for each number value
unordered_map<int, ull> val_key;

// Function to generate or retrieve a random 64-bit key for a number
ull get_key(int val) {
    if (val_key.find(val) == val_key.end()) {
        static mt19937_64 rng(1337); // Seeded random number generator
        val_key[val] = rng();
    }
    return val_key[val];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<int> a(n);
    vector<ull> pref(n + 1, 0);

  
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        pref[i + 1] = pref[i] ^ get_key(a[i]);
    }

    // Step 2: Answer queries in O(1) time
    // Each query gives two subarrays: [l1, r1] and [l2, r2] (0-based indexing)
    // have to check if subarrays are anagram
    while (q--) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;

        ull hash1 = pref[r1 + 1] ^ pref[l1];
        
      
        ull hash2 = pref[r2 + 1] ^ pref[l2];

      
        if (hash1 == hash2) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
