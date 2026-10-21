
#include <bits/stdc++.h>
using namespace std;

struct BinaryTrie {
    static constexpr int B = 20;

    struct Node {
        int child[2];
        int cnt;

        Node() : child{-1, -1}, cnt(0) {}
    };

    vector<Node> t;

    BinaryTrie() {
        t.emplace_back();
    }

    // Insert a prefix XOR
    void insert(int x) {
        int cur = 0;

        for (int i = B; i >= 0; i--) {
            int b = (x >> i) & 1;

            if (t[cur].child[b] == -1) {
                int id = (int)t.size();
                t[cur].child[b] = id;
                t.emplace_back();
            }

            cur = t[cur].child[b];
            t[cur].cnt++;
        }
    }

    // Count inserted q such that (x XOR q) < k
    long long countLess(int x, int k) const {
        int cur = 0;
        long long ans = 0;

        for (int i = B; i >= 0 && cur != -1; i--) {
            int xb = (x >> i) & 1;
            int kb = (k >> i) & 1;

            if (kb == 1) {
                // XOR bit 0: strictly smaller
                int same = t[cur].child[xb];

                if (same != -1)
                    ans += t[same].cnt;

                // XOR bit 1: still equal so far
                cur = t[cur].child[xb ^ 1];
            } else {
                // Must produce XOR bit 0
                cur = t[cur].child[xb];
            }
        }

        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, k;
        cin >> n >> k;

        BinaryTrie trie;

        trie.insert(0); // Empty prefix

        int prefix = 0;
        long long ans = 0;

        for (int i = 0; i < n; i++) {
            int x;
            cin >> x;

            prefix ^= x;

            ans += trie.countLess(prefix, k);

            trie.insert(prefix);
        }

        cout << ans << '\n';
    }
}
