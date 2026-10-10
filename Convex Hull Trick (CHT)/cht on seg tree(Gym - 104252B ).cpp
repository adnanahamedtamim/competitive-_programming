
#include <bits/stdc++.h>
#include <unordered_map>
#include <chrono>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;

typedef long long ll;
typedef long double ld;

#define pb push_back
#define all(x) x.begin(), x.end()
#define YES cout<<"YES"<<endl
#define NO cout<<"NO"<<endl
#define Yes cout<<"Yes"<<endl
#define No cout<<"No"<<endl

typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vector<ll>> vvll;
typedef vector<vector<int>> vvi;

ll MOD = 1000000007;
const ld PI = acos(-1.0L);
const ld EPS = 1e-12;


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

struct Node {
    LineContainer cht;
};


struct segtree{
  ll n;
  vector<Node> st;

  segtree(ll n) : n(n),st(4*n+5){}

  void merge(ll node){
      for(auto &line : st[node*2].cht){
          st[node].cht.add(line.k,line.m);
      }
      for(auto &line : st[node*2+1].cht){
          st[node].cht.add(line.k,line.m);
      }
  }

  void build(ll node,ll l,ll r,vector<Line>&a){

     if(l==r){
          st[node].cht.add(a[l].k,a[l].m);
          return;
     } 

     ll mid=l+(r-l)/2;

     build(node*2,l,mid,a);
     build(node*2+1,mid+1,r,a);
     
     merge(node);
  }

  ll get(ll node,ll l,ll r,ll x,ll y){

     if(st[node].cht.query(x)<=y || l>r)  return -1;

     if(l==r) return l;

     ll mid=(l+r)/2;

     ll ans=get(node*2,l,mid,x,y);

     if(ans!=-1) return ans;
    
     return get(node*2+1,mid+1,r,x,y);

  } 

  void build(vector<Line>& a)  // 1 based must
  {
      build(1,1,n,a);
  }

  ll get(ll x, ll y){
       return get(1,1,n,x,y);
  }

};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    ll t;
    cin >> t;

    vector<pair<ll,ll>> ara(t+1);

    for(ll i=1;i<=t;i++)
    {
         cin >> ara[i].first >> ara[i].second;
    }
     
    ll n;
    cin >> n;

    vector<Line> p(n+1);


    for(ll i=1;i<=n;i++){
          cin >> p[i].k >> p[i].m;
    } 

    segtree st(n);
    st.build(p);
    
    vector<vector<ll>> ans(n+1);

    for(ll i=1;i<=t;i++){

     ll niga=st.get(ara[i].first,ara[i].second);
    //  cout << niga << endl;

     if(niga!=-1) ans[niga].pb(i);
    
    }


    for(ll i=1;i<=n;i++){

           cout << ans[i].size() <<  " ";
           for(auto it : ans[i]) cout << it << " ";
           cout << endl;
    }


    return 0;
}
