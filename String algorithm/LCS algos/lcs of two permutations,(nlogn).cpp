
#include <bits/stdc++.h>
using namespace std;

// LCS of two sequences with unique elements
// Expected time: O(n + m + n log m)
// Space: O(n + m)

int LCS(const vector<int>& a, const vector<int>& b) {
    unordered_map<int, int> pos;
    pos.reserve(b.size() * 2 + 1);

    // Store each element's position in B
    for (int i = 0; i < (int)b.size(); i++) {
        pos[b[i]] = i;
    }

    vector<int> tails;
    tails.reserve(min(a.size(), b.size()));

    for (int x : a) {
        auto found = pos.find(x);

        // Skip values absent from B
        if (found == pos.end())
            continue;

        int p = found->second;

        // LIS on mapped positions
        auto it = lower_bound(
            tails.begin(), tails.end(), p
        );

        if (it == tails.end())
            tails.push_back(p);
        else
            *it = p;
    }

    return (int)tails.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n), b(m);

    for (int& x : a) cin >> x;
    for (int& x : b) cin >> x;

    cout << LCS(a, b) << '\n';
}
