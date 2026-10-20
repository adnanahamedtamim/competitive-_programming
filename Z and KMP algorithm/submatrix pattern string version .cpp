// given a n*m matrix, find the number of occurences of pattern x*y matrix inside it
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll BASE = 101;
const ll MOD = 1000000321;

struct Hash {
    vector<ll> pre, power;

    Hash(string s) {
        int n = s.size();

        pre.resize(n + 1, 0);
        power.resize(n + 1, 1);

        for (int i = 0; i < n; i++) {
            power[i + 1] = power[i] * BASE % MOD;

            pre[i + 1] = (pre[i] * BASE
                        + (s[i] - 'a' + 1)) % MOD;
        }
    }

    // Hash of substring [l, r]
    ll get(int l, int r) const {
        ll result = pre[r + 1]
                  - pre[l] * power[r - l + 1] % MOD;

        return (result + MOD) % MOD;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<Hash> text;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        text.emplace_back(s);
    }

    int h, w;
    cin >> h >> w;

    vector<Hash> pattern;
    vector<ll> p(h);

    for (int i = 0; i < h; i++) {
        string s;
        cin >> s;

        pattern.emplace_back(s);
        p[i] = pattern.back().get(0, w - 1);
    }

    if (h > n || w > m) {
        cout << 0 << '\n';
        return;
    }

    // Step 1: KMP prefix function on pattern rows
    vector<int> pi(h, 0);

    for (int i = 1; i < h; i++) {
        int j = pi[i - 1];

        while (j > 0 && p[i] != p[j])
            j = pi[j - 1];

        if (p[i] == p[j])
            j++;

        pi[i] = j;
    }

    // Step 2: Run KMP vertically for every column
    int ans = 0;

    for (int col = 0; col + w <= m; col++) {
        int j = 0;

        for (int row = 0; row < n; row++) {
            ll cur = text[row].get(col, col + w - 1);

            while (j > 0 && cur != p[j])
                j = pi[j - 1];

            if (cur == p[j])
                j++;

            if (j == h) {
                ans++;
                j = pi[j - 1];
            }
        }
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
        solve();

    return 0;
}
