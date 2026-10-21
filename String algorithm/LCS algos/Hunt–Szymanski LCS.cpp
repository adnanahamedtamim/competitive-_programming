
#include <bits/stdc++.h>
using namespace std;

// Hunt–Szymanski LCS
// Time: O(n + m + r log m)
// Space: O(m)
//
// r = number of matching position pairs
//   = sum(freqA[c] * freqB[c])
// if r is small this is nlogn, else n^2

int LCS(const string& a, const string& b) {
    // Positions of each character in B
    vector<int> pos[256];

    for (int j = 0; j < (int)b.size(); j++) {
        pos[(unsigned char)b[j]].push_back(j);
    }

    // LIS tails
    vector<int> lis;
    lis.reserve(min(a.size(), b.size()));

    for (unsigned char c : a) {
        const vector<int>& positions = pos[c];

        // Process matching positions in reverse order
        for (int k = (int)positions.size() - 1; k >= 0; k--) {
            int x = positions[k];

            auto it = lower_bound(
                lis.begin(), lis.end(), x
            );

            if (it == lis.end())
                lis.push_back(x);
            else
                *it = x;
        }
    }

    return (int)lis.size();
}

string LCSPrint(const string& a, const string& b) {
    vector<int> pos[256];

    for (int j = 0; j < (int)b.size(); j++)
        pos[(unsigned char)b[j]].push_back(j);

    struct Match {
        int position;
        int parent;
        char value;
    };

    vector<Match> matches;
    vector<int> tails;
    vector<int> tailIndex;

    for (unsigned char c : a) {
        const auto& p = pos[c];

        // Descending order is essential
        for (int k = (int)p.size() - 1; k >= 0; k--) {
            int x = p[k];

            int len = lower_bound(
                tails.begin(), tails.end(), x
            ) - tails.begin();

            int parent = (len == 0)
                ? -1
                : tailIndex[len - 1];

            int id = (int)matches.size();
            matches.push_back({x, parent, (char)c});

            if (len == (int)tails.size()) {
                tails.push_back(x);
                tailIndex.push_back(id);
            } else {
                tails[len] = x;
                tailIndex[len] = id;
            }
        }
    }

    string result;

    if (tailIndex.empty())
        return result;

    int id = tailIndex.back();

    while (id != -1) {
        result.push_back(matches[id].value);
        id = matches[id].parent;
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

    return 0;
}
