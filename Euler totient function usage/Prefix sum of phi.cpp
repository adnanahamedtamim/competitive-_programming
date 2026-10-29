// given T testcase(T<=10^4), given n,m<=10^6, find the sum of gcd(x,y) such that 1<=x<=n, 1<=y<=m
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// First stores phi[i], then becomes prefix sum of phi.
vector<ll> Phi;

void build_totient_prefix(int N) {
    Phi.assign(N + 1, 0);
    if (N == 0) return;

    vector<int> primes;
    Phi[1] = 1;


    for (int i = 2; i <= N; ++i) {
        if (Phi[i] == 0) {
        
            primes.push_back(i);
            Phi[i] = i - 1;
        }

        for (int p : primes) {
            ll x = 1LL * i * p;
            if (x > N) break;

            if (i % p == 0) {
                Phi[x] = Phi[i] * p;
                break;
            } else {
                Phi[x] = Phi[i] * (p - 1);
            }
        }
    }


    for (int i = 1; i <= N; ++i) {
        Phi[i] += Phi[i - 1];
    }
}

ll sum_gcd_rect(int n, int m) {
    ll ans = 0;
    int lim = min(n, m);

    for (int l = 1; l <= lim; ) {
        int qn = n / l;
        int qm = m / l;

        // Largest r for which BOTH quotients remain unchanged.
        int r = min(n / qn, m / qm);

        ll sum_phi = Phi[r] - Phi[l - 1];
        ans += sum_phi * qn * qm;

        l = r + 1;
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    vector<pair<int, int>> queries(T);
    int N = 0;

    for (auto &[n, m] : queries) {
        cin >> n >> m;

       
        N = max(N, min(n, m));
    }

    build_totient_prefix(N);

    for (auto [n, m] : queries) {
        cout << sum_gcd_rect(n, m) << '\n';
    }

    return 0;
}
