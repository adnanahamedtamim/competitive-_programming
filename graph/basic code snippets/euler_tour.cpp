
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vll = vector<ll>;
using vvll = vector<vector<ll>>;

ll timer;
vll tin, tout, tour;
vvll adj;

void dfs(ll u, ll p) {
    tin[u] = timer;
    tour[timer++] = u;

    for (ll v : adj[u]) {
        if (v == p) continue;
        dfs(v, u);
    }
   
    tout[u] = timer;
    tour[timer++] = u;
}

void euler_tour(ll n, ll root = 0) {
    timer = 0;
    tin.assign(n, 0);
    tout.assign(n, 0);
    tour.assign(2*n, 0);

    dfs(root, -1);
}

bool is_ancestor(ll u, ll v) {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    adj.assign(n, {});

    for (ll i = 0; i < n - 1; i++) {
        ll u, v;
        cin >> u >> v;
        u--;
        v--;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    euler_tour(n);

    for (ll i=0;i<2*n;i++){
        cout << tour[i] + 1 << " ";
    }
    cout << '\n';

    for(ll i=0;i<n;i++){
          cout << i+1 << " " << tin[i] << " " << tout[i] << endl;
     } 

     cout << is_ancestor(2,0);

    return 0;
}
