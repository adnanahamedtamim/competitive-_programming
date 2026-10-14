#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MAXSUM = 700000;
bitset<MAXSUM + 1> dp;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    vector<ll> a(n);
    ll sum = 0;

    for (ll &x : a) {
        cin >> x;
        sum += x;
    }

    dp[0] = 1;

    for (ll x : a) {
        dp |= (dp << x);
    }

    for (ll s = 0; s <= sum; s++) {
        if (dp[s]) {
            cout << s << ' ';
        }
    }
    cout << '\n';

    return 0;
}
