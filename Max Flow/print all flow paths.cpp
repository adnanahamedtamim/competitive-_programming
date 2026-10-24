// put it inside the dinic struct, it will print all the  paths formed from src to  dest using flow

void print_paths(ll s, ll t, ll maxPaths) {
    for (ll i = 0; i < maxPaths; i++) {
        vector<ll> path;
        ll cur = s;
        path.push_back(cur);

        while (cur != t) {
            for (auto &e : adj[cur]) {
                if (e.cap > 0 && e.flow > 0) {
                    e.flow--; // Consume 1 unit of flow along this edge
                    cur = e.to;
                    path.push_back(cur);
                    break;
                }
            }
        }

        cout << path.size() << "\n";
        for (ll j = 0; j < (ll)path.size(); j++) {
            cout << path[j] << (j == (ll)path.size() - 1 ? "" : " ");
        }
        cout << "\n";
    }
}
