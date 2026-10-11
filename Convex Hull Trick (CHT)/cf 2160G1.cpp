#include <bits/stdc++.h>
using namespace std;

#define  ll long long
#define  vll vector<ll>
#define pb push_back

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
    
    ios_base::sync_with_stdio(false);
    
    cin.tie(NULL); 
   
  ll t;
  cin >> t;


  while(t--){
    ll n;
    cin >> n;

    vll ara(n);
    for(ll i=0;i<n;i++){
          cin >> ara[i];
    }
    vll suff(n);
    suff[n-1]=ara[n-1];

    for(ll i=n-2;i>=0;i--){
           suff[i]=min(suff[i+1],ara[i]);
    }

    vll su;

    for(ll i=0;i<n-1;i++){
          if(ara[i]<suff[i+1]){
              su.pb(ara[i]);
          }
    }
    su.pb(ara[n-1]);
    n=su.size();

    // for(auto it : su) cout << it << " " ;
    // cout << endl;
    // cout << "n " << n << endl;

    LineContainer cht;

    vll ans(n,0);
    
    ans[n-1]=1;
    cht.add(0,-su[n-1]);
    
    for(ll i=n-2;i>=0;i--){
          cht.add(-ans[i+1],-su[i]);
          ll cur=-cht.query(su[i]);
          ll real=(cur+su[i]-1)/su[i];
          ans[i]=real;
    }
    
    cout << ans[0] << endl;


  }

   
   return 0;
}
