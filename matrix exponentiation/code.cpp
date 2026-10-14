```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 1e9 + 7;

using Mat = vector<vector<ll>>;

Mat multiply(const Mat &a, const Mat &b) {
    int n = a.size();
    int m = b.size();
    int p = b[0].size();

    Mat c(n, vector<ll>(p, 0));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            if (a[i][k] == 0) continue;

            for (int j = 0; j < p; j++) {
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }

    return c;
}

Mat matpow(Mat a, ll k) {
    int n = a.size();

    Mat res(n, vector<ll>(n, 0));

    for (int i = 0; i < n; i++) {
        res[i][i] = 1;
    }

    while (k > 0) {
        if (k & 1) {
            res = multiply(res, a);
        }

        a = multiply(a, a);
        k >>= 1;
    }

    return res;
}


int main() {
    Mat a = {
        {1, 1},
        {1, 0}
    };

    ll k = 5;

    Mat ans = matpow(a, k);

    for (auto &row : ans) {
        for (ll x : row) cout << x << ' ';
        cout << '\n';
    }
}



```
