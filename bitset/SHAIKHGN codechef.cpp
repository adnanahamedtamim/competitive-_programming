#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 500;
const int LOG = 31;

int n;

using Row = bitset<MAXN>;
using Mat = vector<Row>;

Mat multiply(const Mat &a, const Mat &b) {
    Mat c(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j]) {
                c[i] |= b[j];
            }
        }
    }

    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    Mat a(n, Row());

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;
            a[i][j] = x;
        }
    }

    vector<Mat> pw(LOG);
    pw[0] = a;

    for (int b = 1; b < LOG; b++) {
        pw[b] = multiply(pw[b - 1], pw[b - 1]);
    }

    int m;
    cin >> m;

    while (m--) {
        ll k;
        int x;
        cin >> k >> x;
        --x;

        Row cur;
        cur[x] = 1;

        for (int b = 0; b < LOG; b++) {
            if ((k >> b) & 1LL) {
                Row nxt;

                for (int v = 0; v < n; v++) {
                    if (cur[v]) {
                        nxt |= pw[b][v];
                    }
                }

                cur = nxt;
            }
        }

        cout << cur.count() << '\n';

        if (cur.none()) {
            cout << -1 << '\n';
        } else {
            bool first = true;

            for (int i = 0; i < n; i++) {
                if (cur[i]) {
                    if (!first) cout << ' ';
                    cout << i + 1;
                    first = false;
                }
            }

            cout << '\n';
        }
    }

    return 0;
}
