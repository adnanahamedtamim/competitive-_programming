#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

ll phi(ll n)
{
    ll ans = n;

    for(ll p = 2; p * p <= n; p++)
    {
        if(n % p == 0)
        {
            while(n % p == 0)
                n /= p;

            ans -= ans / p;
        }
    }

    if(n > 1)
        ans -= ans / n;

    return ans;
}

ll modpow(ll a, ll b, ll mod)
{
    ll ans = 1 % mod;
    a %= mod;

    while(b)
    {
        if(b & 1)
            ans = (i128)ans * a % mod;

        a = (i128)a * a % mod;
        b >>= 1;
    }

    return ans;
}

// Is the tower a[pos]^(a[pos+1]^(...)) >= need ?
bool atLeast(const vector<ll>& a, ll pos, ll need)
{
    if(need <= 1)
        return true;

    if(pos == (ll)a.size() - 1)
        return a[pos] >= need;

    if(a[pos] == 1)
        return false;

    // Find the smallest e such that a[pos]^e >= need
    ll cur = 1;
    ll e = 0;

    while(cur < need)
    {
        e++;

        if(cur > (need - 1) / a[pos])
            break;

        cur *= a[pos];
    }

    // We now only need to know whether
    // the remaining tower >= e
    return atLeast(a, pos + 1, e);
}

ll solve(const vector<ll>& a, ll pos, ll mod)
{
    if(mod == 1)
        return 0;

    if(pos == (ll)a.size() - 1)
        return a[pos] % mod;

    ll ph = phi(mod);

    // Calculate the exponent modulo phi(mod)
    ll e = solve(a, pos + 1, ph);

    // If the REAL exponent >= phi(mod),
    // add phi(mod).
    if(atLeast(a, pos + 1, ph))
        e += ph;

    return modpow(a[pos], e, mod);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;

    vector<ll> a(n);

    for(ll &x : a)
        cin >> x;

    cout << solve(a, 0, m) << '\n';

    return 0;
}
