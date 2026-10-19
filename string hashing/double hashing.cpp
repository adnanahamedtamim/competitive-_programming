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
    vector<ll> rh1, rh2; // Added for O(1) palindrome check

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

    DoubleHash(const string &s) {
        n = s.size();
        ensure_pwrs(n);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);
        rh1.assign(n + 1, 0);
        rh2.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            ll val = (unsigned char)s[i] + 1LL;
            h1[i + 1] = (h1[i] * p1 + val) % mod1;
            h2[i + 1] = (h2[i] * p2 + val) % mod2;
        }

        // Build reverse hash prefix arrays
        for (int i = 0; i < n; i++) {
            ll val = (unsigned char)s[n - 1 - i] + 1LL;
            rh1[i + 1] = (rh1[i] * p1 + val) % mod1;
            rh2[i + 1] = (rh2[i] * p2 + val) % mod2;
        }
    }

    pair<ll, ll> getHash(int l, int r) const {
        if (l > r) return {0, 0};
        int len = r - l + 1;
        ll x = (h1[r + 1] - h1[l] * pwr1[len] % mod1 + mod1) % mod1;
        ll y = (h2[r + 1] - h2[l] * pwr2[len] % mod2 + mod2) % mod2;
        return {x, y};
    }

    pair<ll, ll> getRevHash(int l, int r) const {
        if (l > r) return {0, 0};
        int len = r - l + 1;
        int rl = n - 1 - r;
        int rr = n - 1 - l;
        ll x = (rh1[rr + 1] - rh1[rl] * pwr1[len] % mod1 + mod1) % mod1;
        ll y = (rh2[rr + 1] - rh2[rl] * pwr2[len] % mod2 + mod2) % mod2;
        return {x, y};
    }

    // O(1) palindrome check for substring s[l ... r]
    bool isPalindrome(int l, int r) const {
        if (l >= r) return true;
        return getHash(l, r) == getRevHash(l, r);
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

    ll t;
    cin >> t;

    while (t--) {
        string s, s2;
        cin >> s >> s2;

        if (s.size() < s2.size()) {
            cout << "Not Found\n\n";
            continue;
        }

        DoubleHash hs(s), hs2(s2);

        pair<ll, ll> sh = hs2.getHash(0, s2.size() - 1);
        vll ans;

        for (ll i = 0; i + s2.size() <= s.size(); i++) {
            ll right = i + s2.size() - 1;
            pair<ll, ll> sh2 = hs.getHash(i, right);
            if (sh2 == sh) {
                ans.pb(i + 1); // 1-based index stored here
            }
        }

        if (ans.size() == 0) {
            cout << "Not Found\n";
        } else {
            cout << ans.size() << "\n";
            for (auto &it : ans) {
                cout << it << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    return 0;
}
