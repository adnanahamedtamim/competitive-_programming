
#include <bits/stdc++.h>
using namespace std;

struct BinaryTrie {
    static constexpr int B = 31;

    struct Node {
        int child[2];
        int cnt;

        Node() : child{-1, -1}, cnt(0) {}
    };

    vector<Node> t;

    BinaryTrie() {
        t.emplace_back();
    }

    int size() const {
        return t[0].cnt;
    }

    // Insert one occurrence
    void insert(uint32_t x) {
        int cur = 0;
        t[cur].cnt++;

        for (int i = B; i >= 0; i--) {
            int b = (x >> i) & 1U;

            if (t[cur].child[b] == -1) {
                int id = (int)t.size();
                t[cur].child[b] = id;
                t.emplace_back();
            }

            cur = t[cur].child[b];
            t[cur].cnt++;
        }
    }

    // Number of occurrences of x
    int count(uint32_t x) const {
        int cur = 0;

        for (int i = B; i >= 0; i--) {
            int b = (x >> i) & 1U;
            cur = t[cur].child[b];

            if (cur == -1)
                return 0;
        }

        return t[cur].cnt;
    }

    // Erase one occurrence; returns false if absent
    bool erase(uint32_t x) {
        if (count(x) == 0)
            return false;

        int cur = 0;
        t[cur].cnt--;

        for (int i = B; i >= 0; i--) {
            int b = (x >> i) & 1U;
            cur = t[cur].child[b];
            t[cur].cnt--;
        }

        return true;
    }

    // Maximum XOR value
    uint32_t maxXor(uint32_t x) const {
        if (size() == 0)
            return 0;

        int cur = 0;
        uint32_t ans = 0;

        for (int i = B; i >= 0; i--) {
            int b = (x >> i) & 1U;
            int want = b ^ 1;

            int p = t[cur].child[want];

            if (p != -1 && t[p].cnt > 0) {
                ans |= (1U << i);
                cur = p;
            } else {
                cur = t[cur].child[b];
            }
        }

        return ans;
    }

    // Minimum XOR value
    uint32_t minXor(uint32_t x) const {
        if (size() == 0)
            return 0;

        int cur = 0;
        uint32_t ans = 0;

        for (int i = B; i >= 0; i--) {
            int b = (x >> i) & 1U;

            int p = t[cur].child[b];

            if (p != -1 && t[p].cnt > 0) {
                cur = p;
            } else {
                ans |= (1U << i);
                cur = t[cur].child[b ^ 1];
            }
        }

        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BinaryTrie bt;

    bt.insert(3);
    bt.insert(5);
    bt.insert(6);
    bt.insert(2);

    cout << bt.maxXor(4) << '\n'; // 7
    cout << bt.minXor(4) << '\n'; // 1

    bt.erase(3);

    cout << bt.maxXor(4) << '\n'; // 6
}
