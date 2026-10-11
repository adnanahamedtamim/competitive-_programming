#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define vll vector<ll>
#define pb push_back

struct Line {
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


ll n,k;
const int maxn=1e5+2;
vector<vector<ll>> adj(maxn);
LineContainer cht[maxn];
vector<ll> a(maxn),b(maxn),sizee(maxn,0),dp(maxn,0);


void dfs(ll i,ll par){
  ll maxi=-1;
  ll maxchild=-1;
  ll siz=1;

  for(auto it : adj[i]){
     if(it==par) continue;
     dfs(it,i);
     siz+=sizee[it];
     if(sizee[it]>maxi){
          maxi=sizee[it];
          maxchild=it;
     }
  }

  if(maxchild!=-1){
     swap(cht[i],cht[maxchild]);
  }

  for(auto it : adj[i]){
      if(it==par || it==maxchild) continue;
      for(auto line : cht[it]){
          cht[i].add(line.k,line.m);
      }
      cht[it].clear();
  }
  
  if(maxchild==-1){
      dp[i]=0;
  }
  else{
     dp[i]=-cht[i].query(a[i]);
  }
  
  cht[i].add(-b[i],-dp[i]);

}


int main(){

cin >> n;

for(ll i=0;i<n;i++)
{
     cin >> a[i];
}

for(ll i=0;i<n;i++){
     cin >> b[i];
}

for(ll i=0;i<n-1;i++){
      ll x,y;
      cin >> x >> y;
      x--;
      y--;
      adj[x].pb(y);
      adj[y].pb(x);
}

  dfs(0,-1);
 
  for(ll i=0;i<n;i++){
      cout << dp[i] << " ";
  }

}
