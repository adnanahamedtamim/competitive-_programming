// super fast for primitve data type, only work for them . amortized 0(1)

#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

// Ultra-fast flat open-addressing hash table with version reset
struct FastHashTable {
    static const int SZ = 1 << 20; // 1,048,576 slots (must be power of 2) and must be at least 2*n . (n is the number of estimated keys)
    // use 1<<22 if n<=2*(10^6) 
   static const int MASK = SZ - 1;

    ull keys[SZ];
    int vals[SZ];
    int ver[SZ];
    int cur_ver = 0;

    void clear() {
        cur_ver++; // O(1) reset!
    }

    static inline int get_bucket(ull key) {
        key ^= key >> 33;
        key *= 0xff51afd7ed558ccdULL;
        key ^= key >> 33;
        return key & MASK;
    }

    void insert(ull key, int val) {
        int b = get_bucket(key);
        while (ver[b] == cur_ver && keys[b] != key) {
            b = (b + 1) & MASK;
        }
        if (ver[b] != cur_ver) {
            ver[b] = cur_ver;
            keys[b] = key;
            vals[b] = val;
        } else {
            vals[b] = val; // update value if already exists
        }
    }

    // Increases the value associated with 'key' by 'delta'
    // Returns the new updated value!
    int increase(ull key, int delta = 1) {
        int b = get_bucket(key);
        while (ver[b] == cur_ver && keys[b] != key) {
            b = (b + 1) & MASK;
        }
        if (ver[b] != cur_ver) {
            // Key doesn't exist yet -> initialize it to 'delta'
            ver[b] = cur_ver;
            keys[b] = key;
            vals[b] = delta;
            return vals[b];
        } else {
            // Key already exists -> add delta to its current value!
            vals[b] += delta;
            return vals[b];
        }
    }
    int find(ull key) const {
        int b = get_bucket(key);
        while (ver[b] == cur_ver) {
            if (keys[b] == key) return vals[b];
            b = (b + 1) & MASK;
        }
        return -1; // Not found
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<ull> arr = {10592, 429104, 10592, 998244, 429104};

    FastHashTable ht;
    ht.clear();
    for (int i = 0; i < arr.size(); i++) {
        int prev_idx = ht.find(arr[i]);
        if (prev_idx != -1) {
            cout << "Duplicate found! Value " << arr[i] 
                 << " was previously at index " << prev_idx 
                 << ", now at index " << i << "\n";
        } else {
            ht.insert(arr[i], i);
        }
    }

    return 0;
}
