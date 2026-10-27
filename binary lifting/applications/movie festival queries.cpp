//In a movie festival, n movies will be shown. You know the starting and ending time of each movie.
Your task is to process q queries of the form: if you arrive and leave the festival at specific times, what is the maximum number of movies you can watch?
You can watch two movies if the first movie ends before or exactly when the second movie starts. You can start the first movie exactly when you arrive and leave exactly when the last movie ends.

#include <bits/stdc++.h>
#include <unordered_map>
#include <chrono>
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>

using namespace std;

typedef long long ll;
typedef long double ld;

#define pb push_back
#define all(x) x.begin(), x.end()
#define endl '\n'
#define YES cout << "YES" << endl
#define NO cout << "NO" << endl
#define Yes cout << "Yes" << endl
#define No cout << "No" << endl

#define isSet(x, i) ((x >> i) & 1)
// #define setbit(x, i) (x | (1LL<<i))
// #define resetbit(x, i) (x & (~(1LL << i)))
// #define toggleBit(x, i) ((x) ^ (1LL << (i)))
// #define clz(x) __builtin_clzll(x)
#define ctz(x) __builtin_ctzll(x)
#define csb(x) __builtin_popcountll(x)
#define msb(x) (ll)((x) ? (63 - __builtin_clzll((ll)(x))) : -1)
// #define lsb(x) (ll)((x) ? (__builtin_ctzll((ll)(x))) : -1)
typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vector<ll>> vvll;
typedef vector<vector<int>> vvi;


ll maxi = 1e6;
vvll dp(maxi + 2, vll(21, 1e6 + 1));

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // cout << " dfgdgrgrt";

    ll n, q;
    cin >> n >> q;

    for (ll i = 0; i < n; i++)
    {
        ll l, r;
        cin >> l >> r;
        dp[l][0] = min(dp[l][0], r);
    }

    for (ll i = maxi-1; i >= 0; i--)
    {
        dp[i][0] = min(dp[i][0], dp[i + 1][0]);
    }

    for (ll j = 1; j < 20; j++)
    {
        for (ll i = 1; i <= maxi; i++)
        {
            if (dp[i][j - 1] <= maxi)
            {
                dp[i][j] = dp[dp[i][j - 1]][j - 1];
            }
        }
    }
    // // cout << "sdfwe";

    while (q--)
    {
        ll x, y;
        cin >> x >> y;
        ll ans = 0;

        for (ll i = 20; i >= 0; i--)
        {
            if (dp[x][i] <= y)
            {
                ans += (1LL << i);
                x = dp[x][i];
            }
        }
        cout << ans << endl;
    }

    return 0;
}
