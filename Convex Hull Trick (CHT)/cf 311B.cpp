#include <bits/stdc++.h>
using namespace std;

#define  ll long long
#define  vll vector<ll>

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
   
   ll n,m,p;
   cin >>  n >> m >> p;
   
   vll d(n+1,0);
   for(ll i=2;i<=n;i++) cin >> d[i];

   vector<pair<ll,ll>> h(m+1);
   for(ll i=1;i<=m;i++){
      cin >> h[i].first >> h[i].second;
   }

   vll prefd(n+1,0);
   for(ll i=1;i<=n;i++) prefd[i]=prefd[i-1]+d[i];

   vll ara(m+1,0);

   for(ll i=1;i<=m;i++){
      ll d=h[i].second;
      d-=(prefd[h[i].first]);
      ara[i]=d;
   }

   sort(ara.begin()+1,ara.end());

   vll dp(m+1,0);

   LineContainer cht;

   ll sum=0;
   vll pref(m+1,0);

   for(ll i=1;i<=m;i++){
       sum+=ara[i];
       ll cur=(i*ara[i]-sum);
       dp[i]=cur;
       pref[i]=pref[i-1]+ara[i];
   }
 


   if(p==1){
      cout << dp[m] << endl;
      return 0;
   }
//    for(auto it : ara) cout << it << " ";
//    cout << endl;
//    cout << dp[m] << endl;

   for(ll pp=2;pp<=p;pp++){
      vll newdp(m+1,0);
      for(ll i=pp;i<=m;i++){
          
          cht.add((i-1),-pref[i-1]-dp[i-1]);
          ll cur=-cht.query(ara[i])-pref[i]+ara[i]*i;
        //   cout << i << " " << cur << endl;
          newdp[i]=cur; 
      }
      cht.clear();
      dp=newdp;
   }
   
   cout << dp[m] << endl;

   
   return 0;
}
