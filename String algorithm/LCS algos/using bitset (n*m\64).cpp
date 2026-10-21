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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    cout << LCS(a, b) << '\n';
}
