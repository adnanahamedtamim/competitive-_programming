#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 200005;
const int LOG = 19; // 2^18 = 262,144 > 200,005

int up[LOG][MAXN];
ll cost[LOG][MAXN];
ll x[MAXN], pref[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    for (int i = 1; i <= n; ++i) {
        cin >> x[i];
        pref[i] = pref[i - 1] + x[i];
    }

   
    vector<int> st;
    for (int i = n; i >= 1; --i) {
        while (!st.empty() && x[st.back()] <= x[i]) {
            st.pop_back();
        }
        int nxt = st.empty() ? (n + 1) : st.back();
        up[0][i] = nxt;
        cost[0][i] = 1LL * (nxt - i) * x[i] - (pref[nxt - 1] - pref[i - 1]);
        st.push_back(i);
    }

    
    for (int k = 0; k < LOG; ++k) {
        up[k][n + 1] = n + 1;
        cost[k][n + 1] = 0;
    }


    for (int k = 1; k < LOG; ++k) {
        for (int i = 1; i <= n; ++i) {
            int mid = up[k - 1][i];
            up[k][i] = up[k - 1][mid];
            cost[k][i] = cost[k - 1][i] + cost[k - 1][mid];
        }
    }

    while (q--) {
        int l, r;
        cin >> l >> r;

        ll ans = 0;
        int curr = l;


        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][curr] <= r) {
                ans += cost[k][curr];
                curr = up[k][curr];
            }
        }

        
        ans += 1LL * (r - curr + 1) * x[curr] - (pref[r] - pref[curr - 1]);

        cout << ans << '\n';
    }

    return 0;
}
