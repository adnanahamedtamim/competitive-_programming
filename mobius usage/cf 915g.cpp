#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define el '\n'

const int MOD = 1e9 + 7;
const int MAXK = 2000005;

int mu[MAXK];
bool composite[MAXK];
vector<int> primes;

ll pwn[MAXK]; // pwn[c] = c^n % MOD
ll diff[MAXK]; // diff[i] = f(i) - f(i-1)

// 1. Standard Linear Sieve for Möbius function
void mobius_sieve(int limit) {
    mu[1] = 1;
    for (int i = 2; i <= limit; i++) {
        if (!composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if (i * p > limit) break;
            composite[i * p] = true;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
}

// O(log n) Binary Exponentiation
ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    mobius_sieve(k);

    // 2. Precompute c^n modulo 10^9 + 7 for all possible multipliers
    for (int c = 1; c <= k; c++) {
        pwn[c] = power(c, n);
    }

    // 3. Harmonic Loop to build the difference array in O(k log k)
    for (int d = 1; d <= k; d++) {
        if (mu[d] == 0) continue;
        
        // Push the contribution of this divisor d to all its multiples i = c * d
        for (int c = 1; c * d <= k; c++) {
            int i = c * d;
            
            // term = c^n - (c-1)^n
            ll term = (pwn[c] - pwn[c - 1] + MOD) % MOD;
            
            // Multiply by mu[d] and add to the difference array for i
            ll contribution = (mu[d] * term) % MOD;
            diff[i] = (diff[i] + contribution + MOD) % MOD;
        }
    }

    ll total_ans = 0;
    ll current_f = 0;

    // 4. Reconstruct f(i) from the differences and accumulate the answer
    for (int i = 1; i <= k; i++) {
        current_f = (current_f + diff[i]) % MOD;
        
        // (b_i XOR i) % MOD
        total_ans = (total_ans + (current_f ^ i)) % MOD;
    }

    cout << total_ans << el;

    return 0;
}
