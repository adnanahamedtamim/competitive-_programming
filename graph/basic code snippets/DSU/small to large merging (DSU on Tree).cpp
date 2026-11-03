#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

#define vll vector<ll>
#define ll long long
#define pb push_back

vector<int> adj[N];

ll color[N];
ll sz[N], heavy[N];
ll freq[N], answer[N];

ll distinct = 0;


void prepare(ll i,ll par)
{
     sz[i]=1;
     heavy[i]=-1;
     ll maxi=0;

     for(auto it : adj[i]){
          if(it==par) continue;
          prepare(it,i);
          if(heavy[i]==-1 || sz[it]>maxi){
             maxi=sz[it];
            heavy[i]=it;
          }
          sz[i]+=sz[it];
     }
}

void add_vertex(int u) {
    int c = color[u];

    if (freq[c] == 0)
        ++distinct;

    ++freq[c];
}

void remove_vertex(int u) {
    int c = color[u];

    --freq[c];

    if (freq[c] == 0)
        --distinct;
}


void emne(ll i, ll par,bool add){
    if(add){
        add_vertex(i);
    } 
    else{
          remove_vertex(i);
    }

    for(auto it : adj[i]){
          if(it!=par) emne(it,i,add);
    }

}

void dfs(ll i,ll par,bool keep){


  for(auto it : adj[i]){
      if(it==par || it==heavy[i]){
          continue;
      }
      dfs(it,i,false);
  }
  if(heavy[i]!=-1){
      dfs(heavy[i],i,true);
  }
  add_vertex(i);
  
  for(auto it : adj[i]){

      if(it==par || it==heavy[i]) continue;
    
      emne(it,i,true);
  
  }
  answer[i]=distinct;

  if(!keep) emne(i,par,false);
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for(ll i=0;i<n;i++){
          cin >> color[i];
    }
    for(ll i=0;i<n-1;i++){
         ll x,y;
         cin >> x >> y;
         x--;
         y--;
         adj[x].pb(y);
         adj[y].pb(x);
    }

    prepare(0,-1);

    dfs(0,-1,true);
    
    for(ll i=0;i<n;i++) cout << answer[i] << " ";

    cout << endl;

}
