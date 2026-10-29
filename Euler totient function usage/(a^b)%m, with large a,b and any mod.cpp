#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

// 1. Compute Euler's Totient Function phi(m) in O(sqrt(m))
ll get_phi(ll m) {
    ll res = m;
    for (ll i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            while (m % i == 0) m /= i;
            res -= res / i;
        }
    }
    if (m > 1) res -= res / m;
    return res;
}

// 2. Reduce large string 'a' modulo m
ll mod_string(const string& s, ll m) {
    i128 res = 0;
    for (char c : s) {
        res = (res * 10 + (c - '0')) % m;
    }
    return (ll)res;
}

// 3. Reduce exponent string 'b' using:
//    exp = b                      if b < phi(m)
//    exp = (b % phi(m)) + phi(m)  if b >= phi(m)
ll reduce_exponent(const string& b_str, ll phi_m) {
    i128 cur = 0;
    bool exceeded = false;
    for (char c : b_str) {
        cur = cur * 10 + (c - '0');
        if (cur >= phi_m) {
            exceeded = true;
            cur %= phi_m;
        }
    }
    return (ll)(exceeded ? (cur + phi_m) : cur);
}

// 4. Modular exponentiation (base^exp) % m
ll power(ll base, ll exp, ll m) {
    if (m == 1) return 0;
    i128 res = 1 % m;
    i128 b = base % m;
    while (exp > 0) {
        if (exp & 1) res = (res * b) % m;
        b = (b * b) % m;
        exp >>= 1;
    }
    return (ll)res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a_str, b_str;
    ll m;
    
    // Input format: string_a string_b m
    if (!(cin >> a_str >> b_str >> m)) return 0;

    if (m == 1) {
        cout << 0 << "\n";
        return 0;
    }

    ll phi_m = get_phi(m);
    ll base = mod_string(a_str, m);
    ll exp = reduce_exponent(b_str, phi_m);

    cout << power(base, exp, m) << "\n";

    return 0;
}
