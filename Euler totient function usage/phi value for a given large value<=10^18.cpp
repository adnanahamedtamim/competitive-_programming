#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
using ll = long long;
using u128 = __uint128_t;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

// ---------------- Modular Arithmetic ----------------

ull mod_mul(ull a, ull b, ull mod) {
    return (u128)a * b % mod;
}

ull mod_pow(ull a, ull e, ull mod) {
    ull res = 1;
    while (e) {
        if (e & 1)
            res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return res;
}

// ---------------- Miller-Rabin ----------------

bool isPrime(ull n) {   // O(log^3n)

    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;

    ull d = n - 1;
    int r = 0;

    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    const ull bases[] = {
        2, 325, 9375, 28178,
        450775, 9780504, 1795265022
    };

    for (ull a : bases) {

        if (a % n == 0) continue;

        ull x = mod_pow(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool ok = false;

        for (int i = 1; i < r; i++) {
            x = mod_mul(x, x, n);

            if (x == n - 1) {
                ok = true;
                break;
            }
        }

        if (!ok)
            return false;
    }

    return true;
}

// ---------------- Pollard Rho ----------------

ull pollard(ull n) { // 0(n^(1/4))

    if (n % 2 == 0)
        return 2;

    while (true) {

        ull c = uniform_int_distribution<ull>(1, n - 1)(rng);
        ull x = uniform_int_distribution<ull>(0, n - 1)(rng);
        ull y = x;
        ull d = 1;

        auto f = [&](ull v) {
            return (mod_mul(v, v, n) + c) % n;
        };

        while (d == 1) {

            x = f(x);
            y = f(f(y));

            ull diff = (x > y ? x - y : y - x);

            d = gcd(diff, n);
        }

        if (d != n)
            return d;
    }
}

// ---------------- Factorization ----------------

vector<ull> factors;

void factor(ull n) {

    if (n == 1)
        return;

    if (isPrime(n)) {
        factors.push_back(n);
        return;
    }

    ull d = pollard(n);

    factor(d);
    factor(n / d);
}

vector<pair<ull,int>> getFactorization(ull n) {

    factors.clear();

    factor(n);

    sort(factors.begin(), factors.end());

    vector<pair<ull,int>> ans;

    for (ull p : factors) {

        if (ans.empty() || ans.back().first != p)
            ans.push_back({p,1});
        else
            ans.back().second++;
    }

    return ans;
}


ll phi(ll n){
   
   vector<pair<ull,int>> result = getFactorization(n);
   
   ll ans=n;

   for(auto &[prime,power] : result){
     ans=ans/prime*(prime-1);
   }
   return ans;

}

// ---------------- Main ----------------
int main() {

    ull n;
    cin >> n;

    // vector<pair<ull,int>> result = getFactorization(n);

    cout << phi(n) << endl;

    return 0;
}
