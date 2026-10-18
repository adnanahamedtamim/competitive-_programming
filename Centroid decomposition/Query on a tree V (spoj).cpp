
#include <bits/stdc++.h>
using namespace std;

#define pb push_back
typedef long long ll;

typedef vector<ll> vi;

const int MX = 200006;
const int LOG = 20;
const int INF = 1e9;

int n, m;
vector<multiset<pair<ll,ll>>> st;
vector<vi> adj;

vector<ll> dep, sz, par, col, best,depth,start;
vector<vector<int>> up;
vector<bool> blocked;
vector<vector<ll>> lookup;

void dfs(int u, int p) {
    up[u][0] = p;
    for (int j = 1; j < LOG; j++)
        up[u][j] = up[up[u][j - 1]][j - 1];

    for (int v : adj[u]) {
        if (v == p) continue;
        dep[v] = dep[u] + 1;
        dfs(v, u);
    }
}


// Build Sparse Table
vector<vector<ll>> buildSparseTable(vector<ll> &val) {
    ll n = val.size();
    ll LOG = log2(n) + 1;

    vector<vector<ll>> dp(n, vector<ll>(LOG));

    for (ll i = 0; i < n; i++)
        dp[i][0] = val[i];

    for (ll j = 1; (1LL << j) <= n; j++) {
        for (ll i = 0; i + (1LL << j) - 1 < n; i++) {

            ll left = dp[i][j - 1];
            ll right = dp[i + (1LL << (j - 1))][j - 1];

            if (depth[left] < depth[right])
                dp[i][j] = left;
            else
                dp[i][j] = right;
        }
    }

    return dp;
}

// RMQ Query
ll query(ll L, ll R, vector<vector<ll>> &lookup) {
    ll j = log2(R - L + 1);

    ll left = lookup[L][j];
    ll right = lookup[R - (1LL << j) + 1][j];

    if (depth[left] < depth[right])
        return left;

    return right;
}

// LCA query for one pair
ll getLCA(ll u, ll v, vector<ll> &start,
          vector<vector<ll>> &lookup) {

    ll L = start[u];
    ll R = start[v];

    if (L > R)
        swap(L, R);

    return query(L, R, lookup);
}

// Euler Tour DFS
void dfs(ll i, ll par,
         vector<vector<ll>> &adj,
         vector<ll> &val,
         ll &timer,
         vector<ll> &start) {

    start[i] = timer;
    val[timer++] = i;

    for (auto it : adj[i]) {

        if (it == par)
            continue;

        depth[it] = depth[i] + 1;

        dfs(it, i, adj, val, timer, start);

        val[timer++] = i;
    }
}



int lca(int x, int y) {
   return  getLCA(x ,y, start, lookup);
}

int dist(int u, int v) {
    int w = lca(u, v);
    return dep[u] + dep[v] - 2 * dep[w];
}

int calcSize(int u, int p) {
    sz[u] = 1;

    for (int v : adj[u]) {
        if (v == p || blocked[v]) continue;
        sz[u] += calcSize(v, u);
    }

    return sz[u];
}

int getCentroid(int u, int p, int total) {
    for (int v : adj[u]) {
        if (v == p || blocked[v]) continue;

        if (sz[v] > total / 2)
            return getCentroid(v, u, total);
    }

    return u;
}

void build(int u, int p) {
    int total = calcSize(u, 0);
    int c = getCentroid(u, 0, total);

    par[c] = p;
    blocked[c] = true;

    for (int v : adj[c]) {
        if (!blocked[v])
            build(v, c);
    }
}

void update(int x) {
    if(col[x]==1){

        for (int c = x; c != 0; c = par[c]){
            ll dis=dist(x,c);
            st[c].erase(make_pair(dis,x));
            if(st[c].empty()){ best[c]=INF;}
            else{
                auto it=st[c].begin();
                best[c]=(*it).first;
            }
        } 
        col[x]=0;

    }
    else{
        //   if(!st[x].empty())
        //   st[x].pb({0,x});
          best[x]=0;

        for (int c = x; c != 0; c = par[c]){
            ll dis=dist(x,c);
            best[c] = min(best[c], dis);
            st[c].insert(make_pair(dis,x));
        }
       col[x]=1;
    }
    
}

int query(int x) {
    ll ans = INF;

    for (int c = x; c != 0; c = par[c])
        ans = min(ans, best[c] + dist(x, c));

    return ans;
}



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    st.resize(n + 1);
    adj.resize(n + 1);
    start.assign(n+1,0);

    dep.assign(n + 1, 0);
    sz.assign(n + 1, 0);
    par.assign(n + 1, 0);
    col.assign(n + 1, 0);
    best.assign(n + 1, INF);
    depth.assign(n+1,0);
    blocked.assign(n + 1, false);

    up.assign(n + 1, vector<int>(LOG, 0));
    
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }

    for (int i = 0; i <= n; i++)
        best[i] = INF;

    for(ll i=0;i<=n;i++) col[i]=0;    

    up[0][0] = 0;
    dep[1] = 0;
    dfs(1, 0);

    build(1, 0);
    
    vector<ll> val(2 * n - 1);

    ll timer = 0;

    dfs(1, 0, adj, val, timer, start);

    lookup = buildSparseTable(val);

    // update(1);
    cin >> m;

    while (m--) {
        int t, v;
        cin >> t >> v;

        if (t == 0)
            update(v);
        else{
           ll x=query(v);
           if(x==INF) cout << -1 << "\n";
           else cout << x << "\n";
        }
    }

    return 0;
}
