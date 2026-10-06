#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define el '\n'

const int MAXV = 100005;
const int MOD = 1e9 + 7;

int mu[MAXV];
bool composite[MAXV];
vector<int> primes;

int freq[MAXV]; // freq[x] = frequency of x in the array
int cnt[MAXV];  // cnt[d] = number of array elements that are multiples of d
ll pw2[MAXV];   // Precomputed powers of 2 modulo 10^9 + 7

// 1. Standard Linear Sieve for Möbius function
void mobius_sieve(int n) {
    mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if (i * p > n) break;
            composite[i * p] = true;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
}

// Precompute powers of 2
void precompute_powers(int n) {
    pw2[0] = 1;
    for (int i = 1; i <= n; i++) {
        pw2[i] = (pw2[i - 1] * 2) % MOD;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    mobius_sieve(MAXV - 1);
    precompute_powers(MAXV - 1);

    int n;
    if (!(cin >> n)) return 0;

    int max_val = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        freq[x]++;
        max_val = max(max_val, x);
    }

    // 2. Build the "at least" count array cnt[d] in O(V log V)
    for (int i = 1; i <= max_val; i++) {
        for (int j = i; j <= max_val; j += i) {
            cnt[i] += freq[j];
        }
    }

    // 3. Apply Möbius inversion to get subsequences with GCD = 1
    ll ans = 0;
    for (int d = 1; d <= max_val; d++) {
        if (mu[d] == 0) continue;
        
        ll subs = pw2[cnt[d]] - 1; // 2^cnt[d] - 1 non-empty subsequences
        if (subs < 0) subs += MOD;

        ll term = (mu[d] * subs) % MOD;
        ans = (ans + term + MOD) % MOD;
    }

    cout << ans << el;

    return 0;
}
