#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

void print128(i128 x) {
    if (x == 0) {
        cout << 0;
        return;
    }

    string s;
    while (x > 0) {
        s.push_back(char('0' + x % 10));
        x /= 10;
    }

    reverse(s.begin(), s.end());
    cout << s;
}

i128 sumLcmSquare(int n) {
    if (n == 0) return 0;

    // F[i] = sum_{d | i} d * phi(d)
    vector<ll> F(n + 1, 0);
    vector<int> primes;

    F[1] = 1;
    i128 answer = 1; // 1 * F[1]

    for (int i = 2; i <= n; ++i) {
        if (F[i] == 0) {
            // i is prime.
            primes.push_back(i);
            F[i] = 1LL * i * i - i + 1;
        }

        answer += (i128)i * F[i];

        for (int p : primes) {
            ll x = 1LL * i * p;
            if (x > n) break;

            ll p2 = 1LL * p * p;

            if (i % p == 0) {
                F[x] = (p2 + 1) * F[i] - p2 * F[i / p];
                break;
            } else {
                F[x] = F[i] * (p2 - p + 1);
            }
        }
    }

    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    print128(sumLcmSquare(n));
    cout << '\n';

    return 0;
}
