#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define el '\n'
#define int ll
#define all(x) begin(x), end(x)

const int MAXV = 1000005;
const int MOD = 998244353;

int spf[MAXV];
int c[MAXV];
vector<int> primes;

void precompute() {
    // 1. Linear sieve to find the Smallest Prime Factor (spf) of every number
    for (int i = 2; i < MAXV; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > spf[i] || i * p >= MAXV) break;
            spf[i * p] = p;
        }
    }

    // 2. DP to calculate the "token cost" c[p] for each prime
    c[2] = 1;
    for (int p : primes) {
        if (p == 2) continue;
        int temp = p - 1;
        int total_c = 0;
        
        // Fast factorization of (p - 1) using the spf array
        while (temp > 1) {
            int q = spf[temp];
            total_c = (total_c + c[q]) % MOD;
            temp /= q;
        }
        c[p] = total_c;
    }
}

signed main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);

#ifdef LOCAL
    freopen("../in.txt", "r", stdin);
    freopen("../out.txt", "w", stdout);
#endif

    precompute();

    int k;
    if (cin >> k) {
        vector<int> p(k), e(k);
        bool has_two = false;
        
        for (int i = 0; i < k; i++) {
            cin >> p[i];
            if (p[i] == 2) {
                has_two = true;
            }
        }
        
        for (int i = 0; i < k; i++) {
            cin >> e[i];
        }

        int ans = 0;
        for (int i = 0; i < k; i++) {
            // Add (e_i * c[p_i]) to the total
            int term = (e[i] % MOD) * c[p[i]] % MOD;
            ans = (ans + term) % MOD;
        }

        // The "Odd Number" exception: if 2 is not in the prime factorization, add the 1 free turn
        if (!has_two) {
            ans = (ans + 1) % MOD;
        }

        cout << ans << el;
    }

    return 0;
}
