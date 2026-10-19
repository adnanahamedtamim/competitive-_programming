#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

const ull MOD = (1ULL<<61) - 1;
static inline ull mul(ull a, ull b){
    __uint128_t t = (__uint128_t)a * b;
    ull r = (ull)(t & MOD) + (ull)(t >> 61);
    return r >= MOD ? r - MOD : r;
}

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
const ull B = rng() % (MOD - 1000) + 500;

vector<ull> pw, H, R;     // each of size n+2, allocated per test
vector<vector<int>> adj;
string s;
ll ans;

void dfs(int v, int par, int d){          // d = length of parent's prefix
    ull c = (unsigned char)s[v];
    H[d+1] = (mul(H[d], B) + c) % MOD;
    R[d+1] = (R[d] + mul(c, pw[d])) % MOD;
    if(H[d+1] == R[d+1]) ans++;

    for(int u : adj[v])
        if(u != par) dfs(u, v, d + 1);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for(int tt = 1; tt <= T; tt++){
        int n;
        cin >> n >> s;

        adj.assign(n, {});
        for(int i = 0; i < n - 1; i++){
            int a, b;
            cin >> a >> b;
            a--; b--;
            adj[a].push_back(b);
            adj[b].push_back(a);
        }

        pw.assign(n + 2, 0);
        H.assign(n + 2, 0);
        R.assign(n + 2, 0);
        pw[0] = 1;
        for(int i = 1; i < n + 2; i++) pw[i] = mul(pw[i-1], B);

        ans = 0;
        dfs(0, -1, 0);

        ll g = __gcd(ans, (ll)n);
        cout << "Case " << tt << ": " << ans / g << "/" << n / g << "\n";
    }
    return 0;
}
