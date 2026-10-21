// works only with small letters, for capital letters, convert them to smaller first
#include <bits/stdc++.h>
using namespace std;

int LCS(string a, string b) {
    if (a.size() < b.size())
        swap(a, b);

    int m = b.size();
    int W = (m + 63) / 64;

    vector<vector<uint64_t>> mask(
        26, vector<uint64_t>(W, 0)
    );

    for (int i = 0; i < m; i++)
        mask[b[i] - 'a'][i / 64] |= 1ULL << (i % 64);

    vector<uint64_t> dp(W, 0);

    for (char c : a) {
        uint64_t carry = 1, borrow = 0;

        for (int i = 0; i < W; i++) {
            uint64_t old = dp[i];

            uint64_t x = mask[c - 'a'][i] | old;
            uint64_t y = (old << 1) | carry;

            carry = old >> 63;

            uint64_t z = x - y;
            uint64_t b1 = x < y;

            uint64_t result = z - borrow;
            uint64_t b2 = z < borrow;

            borrow = b1 | b2;

            dp[i] = x & ~result;
        }
    }

    int ans = 0;

    for (uint64_t x : dp)
        ans += __builtin_popcountll(x);

    return ans;
}



using ull = uint64_t;

// Bitset LCS with reconstruction
// Time: O(n * ceil(m / 64) + n + m)
// Space: O((n + 256) * ceil(m / 64)) 64-bit words

string LCSPrint(string a, string b) {
    // Keep the shorter string in bitset columns
    if (a.size() < b.size())
        swap(a, b);

    int n = (int)a.size();
    int m = (int)b.size();

    if (m == 0)
        return "";

    int W = (m + 63) / 64;

    // Character masks for B
    vector<vector<ull>> mask(
        256, vector<ull>(W, 0)
    );

    for (int j = 0; j < m; j++) {
        unsigned char c = b[j];
        mask[c][j / 64] |= 1ULL << (j % 64);
    }

    // history[i] = bitset DP after processing i chars of A
    vector<vector<ull>> history(
        n + 1, vector<ull>(W, 0)
    );

    for (int i = 1; i <= n; i++) {
        unsigned char c = a[i - 1];

        ull carry = 1;
        ull borrow = 0;

        for (int k = 0; k < W; k++) {
            ull old = history[i - 1][k];

            ull x = mask[c][k] | old;
            ull y = (old << 1) | carry;

            carry = old >> 63;

            ull tmp = x - y;
            ull b1 = (x < y);

            ull sub = tmp - borrow;
            ull b2 = (tmp < borrow);

            borrow = b1 | b2;

            history[i][k] = x & ~sub;
        }
    }

    // Retrieve ordinary DP value dp[i][j].
    // It equals number of set bits in the first j columns.
    auto value = [&](int i, int j) -> int {
        int full = j / 64;
        int rem = j % 64;
        int ans = 0;

        for (int k = 0; k < full; k++)
            ans += __builtin_popcountll(history[i][k]);

        if (rem > 0) {
            ull bits = history[i][full];
            ull bitmask = (1ULL << rem) - 1;
            ans += __builtin_popcountll(bits & bitmask);
        }

        return ans;
    };

    // Backtrack to reconstruct one LCS
    int i = n, j = m;
    string result;

    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result.push_back(a[i - 1]);
            i--;
            j--;
        } else if (value(i - 1, j) >= value(i, j - 1)) {
            i--;
        } else {
            j--;
        }
    }

    reverse(result.begin(), result.end());
    return result;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    cout << LCS(a, b) << '\n';
}
