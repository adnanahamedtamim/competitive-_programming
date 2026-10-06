#include <bits/stdc++.h>
using namespace std;

#define int long long
#define el '\n'

const int MAXV = 1000005; // Adjust based on problem's max array value
int mu[MAXV];
bool composite[MAXV];
vector<int> primes;

int freq[MAXV];
int cnt[MAXV];

// 1. Precompute Möbius function once globally
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

void solve() {
    int n;
    cin >> n;
    
    int max_val = 0;
    vector<int> a(n);
    
    // Read input and populate exact frequencies
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        freq[a[i]]++;
        max_val = max(max_val, a[i]);
    }
    
    // 2. Build the "at least" count array in O(V log V)
    for (int i = 1; i <= max_val; i++) {
        for (int j = i; j <= max_val; j += i) {
            cnt[i] += freq[j];
        }
    }
    
    int total_gcd_sum = 0;
    
    // 3. Two-loop pure Möbius Inversion in O(V log V)
    for (int g = 1; g <= max_val; g++) {
        int exact_pairs = 0;
        
        for (int d = 1; d <= max_val / g; d++) {
            int X = g * d;
            
            // "At least" count: n choose 2 for multiples of X
            int pairs = cnt[X] * (cnt[X] - 1) / 2;
            
            // Scale by mu[d] to filter out overlaps
            exact_pairs += mu[d] * pairs;
        }
        
        // Add exact_pairs * true GCD to the final answer
        total_gcd_sum += g * exact_pairs;
    }
    
    cout << total_gcd_sum << el;
    
    // 4. Clean up global arrays up to max_val for the next testcase
    for (int i = 1; i <= max_val; i++) {
        freq[i] = 0;
        cnt[i] = 0;
    }
}

signed main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // Precompute sieve up to absolute maximum limit required
    mobius_sieve(MAXV - 1);

    int t = 1;
    cin >> t; // Comment out if only a single testcase
    while (t--) {
        solve();
    }

    return 0;
}
