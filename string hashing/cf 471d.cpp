#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define pb push_back
#define all(x) x.begin(), x.end()
#define vll vector<ll>

struct DoubleHash {
    static const ll mod1 = 1000004249;
    static const ll mod2 = 1000000289;
    static ll p1, p2;
    static vector<ll> pwr1, pwr2;

    int n;
    vector<ll> h1, h2;

    static void ensure_pwrs(int sz) {
        if (pwr1.empty()) {
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            p1 = uniform_int_distribution<ll>(257, mod1 - 2)(rng);
            p2 = uniform_int_distribution<ll>(257, mod2 - 2)(rng);
            pwr1 = {1};
            pwr2 = {1};
        }
        while ((int)pwr1.size() <= sz) {
            pwr1.push_back(pwr1.back() * p1 % mod1);
            pwr2.push_back(pwr2.back() * p2 % mod2);
        }
    }

    DoubleHash() : n(0) {}

    // Changed from (const string &s) to (const vll &s)
    DoubleHash(const vll &s) {
        n = s.size();
        ensure_pwrs(n);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            ll val1 = (s[i] % mod1 + mod1) % mod1 + 1LL;
            ll val2 = (s[i] % mod2 + mod2) % mod2 + 1LL;
            h1[i + 1] = (h1[i] * p1 + val1) % mod1;
            h2[i + 1] = (h2[i] * p2 + val2) % mod2;
        }
    }

    pair<ll, ll> getHash(int l, int r) const {
        if (l > r) return {0, 0};
        int len = r - l + 1;
        ll x = (h1[r + 1] - h1[l] * pwr1[len] % mod1 + mod1) % mod1;
        ll y = (h2[r + 1] - h2[l] * pwr2[len] % mod2 + mod2) % mod2;
        return {x, y};
    }

    pair<ll, ll> fullHash() const {
        return {h1[n], h2[n]};
    }

    static pair<ll, ll> concat(pair<ll, ll> hashA, pair<ll, ll> hashB, int lenB) {
        ensure_pwrs(lenB);
        ll x = (hashA.first * pwr1[lenB] + hashB.first) % mod1;
        ll y = (hashA.second * pwr2[lenB] + hashB.second) % mod2;
        return {x, y};
    }
};

// REQUIRED: Define static variables outside the struct
ll DoubleHash::p1 = 0;
ll DoubleHash::p2 = 0;
vector<ll> DoubleHash::pwr1;
vector<ll> DoubleHash::pwr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, w;
    cin >> n >> w;

    vll a(n), b(w);
    for (ll i = 0; i < n; i++) cin >> a[i];
    for (ll i = 0; i < w; i++) cin >> b[i];

    // If elephant width is 1, it matches every position
    if (w == 1) {
        cout << n << "\n";
        return 0;
    }

    if (n < w) {
        cout << 0 << "\n";
        return 0;
    }

    // s = difference vector of wall 'a', s2 = difference vector of pattern 'b'
    vll s(n - 1), s2(w - 1);
    for (ll i = 0; i < n - 1; i++) s[i] = a[i + 1] - a[i];
    for (ll i = 0; i < w - 1; i++) s2[i] = b[i + 1] - b[i];

    // Exact same matching logic from your main()!
    DoubleHash hs(s), hs2(s2);

    pair<ll, ll> sh = hs2.getHash(0, s2.size() - 1);
    vll ans;

    for (ll i = 0; i + s2.size() <= s.size(); i++) {
        ll right = i + s2.size() - 1;
        pair<ll, ll> sh2 = hs.getHash(i, right);
        if (sh2 == sh) {
            ans.pb(i + 1);
        }
    }

    cout << ans.size() << "\n";

    return 0;
}
