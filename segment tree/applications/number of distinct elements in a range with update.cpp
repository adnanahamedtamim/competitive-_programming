#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
#include <unordered_map>
#include <chrono>
 
 
//custom hash for unordered_map is 0(1)
struct custom_hash {
    // A high-quality 64-bit mixer used to distribute bits evenly
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
 
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
 
    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2> &Y) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(hash<T1>()(Y.first)) ^ splitmix64(hash<T2>()(Y.second) + FIXED_RANDOM);
    }
};
 
template <class K, class V> 
using umap = unordered_map<K, V, custom_hash>;
 
void buildsegtree(ll i,ll l,ll r,vector<ll>& st,vector<ll>& ara){
  if(l==r){
 st[i]=ara[l];  
 return ;
  }
  
  ll mid=l+(r-l)/2;
  
  buildsegtree(2*i+1,l,mid,st,ara);
  buildsegtree(2*i+2,mid+1,r,st,ara);
  st[i]=min(st[2*i+1],st[2*i+2]);
  
  return;
 
}
 
ll query(ll start,ll end,ll i,ll l,ll r,vector<ll>& st){
  if(l>end  || r<start){
    return INT_MAX;
  }
   if(start<=l && end>=r){
     return st[i];
   }
   else{
      ll mid=l+(r-l)/2;
      
      return min(query(start,end,2*i+1,l,mid,st),query(start,end,2*i+2,mid+1,r,st));
   }
   
   return LLONG_MAX;
}
 
void  update(ll i,ll l,ll r,ll ind,ll val,vector<ll>& st,vector<ll>& ara){
  if(l==r){
    st[i]=val;
    return;
  }
  
  ll mid=l+(r-l)/2;
  
  if(ind<=mid){
     update(2*i+1,l,mid,ind,val,st,ara);
  }
  else{
    update(2*i+2,mid+1,r,ind,val,st,ara);
  }
  st[i]=min(st[2*i+1],st[2*i+2]);
  
  return ;
}
 
 
int main() {
    ll n,q;
 cin >> n >> q;
 
 
vector<ll> ara(n);
for(ll i=0;i<n;i++){
  cin >> ara[i];
}
 
vector<ll> ng(n);
map<ll,ll> mp2;
umap<ll,set<ll>> mp;
 
for(ll i=n-1;i>=0;i--){
  if(mp2.count(ara[i])>0){
       ng[i]=mp2[ara[i]];
  } 
  else{
     ng[i]=LLONG_MAX;
  }
  mp[ara[i]].insert(i);
  mp2[ara[i]]=i;
}
 
vector<ll> segtree(4*n,LLONG_MAX);
buildsegtree(0,0,n-1,segtree,ng);
 
// for(ll i=0;i<n;i++){
//     cout << ng[i] << " ";
// }
// cout << endl;
while(q--){
  // ll a,l,r;
  // cin >> a >> l >> r;
  
  
  // if(a==1){
  //   l--;
    
  //   update(0,0,n-1,l,r,segtree,ara);
  // }
  // else{
  //     cout << query(l-1,r-1,0,0,n-1,segtree) << endl;
  // }
  ll t;
  cin >> t;
  if(t==1){
      ll k,u;
      cin >> k >> u;
      k--;
      ll val=ara[k];
   auto it = mp[val].lower_bound(k);
 
if(it != mp[val].end()) {
    auto it_copy = it;
    if(it != mp[val].begin()) {
        auto prev_it = prev(it);
        ll ind2 = *prev_it;
        ng[ind2] = ng[k];
        update(0,0,n-1,ind2,ng[ind2],segtree,ng);
    }
    mp[val].erase(it_copy);
}
      
    val=u;
    auto it2=mp[val].upper_bound(k);
    if(it2!=mp[val].end()){
        ll ind=*(it2);
        ng[k]=ind;
       update(0,0,n-1,k,ng[k],segtree,ng);
    }
    else{
        ng[k]=LLONG_MAX;
        update(0,0,n-1,k,ng[k],segtree,ng);
    }
    
  if(it2 != mp[val].begin()) {
    auto prev_it = prev(it2);
    ll ind = *prev_it;
    ng[ind] = k;
    update(0,0,n-1,ind,ng[ind],segtree,ng);
}
    mp[u].insert(k);
    ara[k]=u;
  }
  else{
     ll a,b;
     cin >> a >> b;
     a--;
     b--;
     ll mini=query(a,b,0,0,n-1,segtree);
    // cout << mini << endl;
     if(mini>b){
       cout << "YES" << endl;
     }
     else{
       cout << "NO" << endl;
     }
  }
  
  
}
  
    return 0;
}
