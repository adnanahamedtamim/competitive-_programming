#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vll vector<ll>

struct Line {
    // k -> slope, m -> denotes c in y=mx+c; p-> int x where it gives max value
    mutable long long k, m, p;
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(long long x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    static const long long inf = LLONG_MAX;
    long long div(long long a, long long b) { // floored integer division
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) { x->p = inf; return false; }
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(long long k, long long m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    long long query(long long x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};

int main(){

    
    LineContainer cht;

    ll n,k;
    cin  >> n >> k;

    vll ara(n+1);
    for(ll i=1;i<=n;i++) cin >> ara[i];
    vll dp(n+1,0);

    if(k==1){
          cout  << (ara[n]-ara[1])*(ara[n]-ara[1]);
          return 0;
    }

    for(ll i=1;i<=n;i++){
          dp[i]=(ara[i]-ara[1])*(ara[i]-ara[1]);
       
    }
    ll ans=0;
    for(ll kk=2;kk<=k;kk++){

       vll newdp(n+1,1e9);
        for(ll i=kk;i<=n;i++){ 
            cht.add(2*ara[i],-(dp[i-1]+ara[i]*ara[i]));
            ll cur=-cht.query(ara[i])+ara[i]*ara[i];
            //  cout <<kk << " "  << i << " " << cur << endl;
            ans=cur;
            newdp[i]=cur;
        }
       dp=newdp;
       cht.clear();
    }

    cout << ans << endl;
    

}
