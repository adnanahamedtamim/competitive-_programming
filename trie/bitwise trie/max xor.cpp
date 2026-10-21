
#include <bits/stdc++.h>
using namespace std;

struct BinaryTrie {
    struct Node {
        int child[2];

        Node() {
            child[0] = child[1] = -1;
        }
    };

    vector<Node> trie;

    BinaryTrie() {
        trie.emplace_back(); // root = 0
    }

    // Insert a number: O(32)
    void insert(uint32_t x) {
        int cur = 0;

        for (int bit = 31; bit >= 0; bit--) {
            int b = (x >> bit) & 1U;

            if (trie[cur].child[b] == -1) {
                int id = trie.size();
                trie[cur].child[b] = id;
                trie.emplace_back();
            }

            cur = trie[cur].child[b];
        }
    }

    // Maximum XOR value with x: O(32)
    uint32_t maxXor(uint32_t x) const {
        int cur = 0;
        uint32_t ans = 0;

        for (int bit = 31; bit >= 0; bit--) {
            int b = (x >> bit) & 1U;
            int want = b ^ 1;

            if (trie[cur].child[want] != -1) {
                ans |= (1U << bit);
                cur = trie[cur].child[want];
            } else if (trie[cur].child[b] != -1) {
                cur = trie[cur].child[b];
            } else {
                break;
            }
        }

        return ans;
    }
};

int main() {
    BinaryTrie bt;

    vector<uint32_t> a = {3, 5, 6, 2};

    for (uint32_t x : a)
        bt.insert(x);

    cout << bt.maxXor(4) << '\n'; // 7
}
