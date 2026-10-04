#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define el '\n'
#define int ll
#define all(x) begin(x), end(x)

const int N = 1e6 + 4;
int phi[N], ans[N];

void pre_solve() {
    // 1. Compute Euler's Totient Function (phi)
    for (int i = 1; i < N; i++) {
        phi[i] += i;
        for (int j = i + i; j < N; j += i) {
            phi[j] -= phi[i];
        }
    }

    // 2. Compute ans array (Simplified)
    for (int i = 1; i < N; i++) {
        // Hoist the invariant math out of the inner loop
        int base_val = (phi[i] + (i == 1)) * i;
        
        // i acts as the divisor, j iterates through its multiples
        for (int j = i; j < N; j += i) {
            ans[j] += (j * base_val) / 2;
        }
    }
}

void solve() {
    int n; 
    cin >> n;
    cout << ans[n] << el;
}

signed main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    pre_solve();
    
    int t; 
    cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
}
