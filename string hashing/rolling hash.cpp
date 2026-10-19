
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll p = 31;
const ll mod = 1000000007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s = "abcdef";
    int n = s.size(), k = 4;

    ll pw = 1;
    for (int i = 1; i < k; i++)
        pw = pw * p % mod;

    ll h = 0;

    for (int i = 0; i < k; i++)
        h = (h * p + (s[i] - 'a' + 1)) % mod;

    cout << s.substr(0, k) << " : " << h << '\n';

    for (int i = k; i < n; i++) {
        h = (h - (s[i - k] - 'a' + 1) * pw % mod + mod) % mod;
        h = (h * p + (s[i] - 'a' + 1)) % mod;

        cout << s.substr(i - k + 1, k) << " : " << h << '\n';
    }

    return 0;
}
