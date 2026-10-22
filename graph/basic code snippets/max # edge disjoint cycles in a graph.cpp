int maxEdgeDisjointCycles(vector<vector<int>>& adj) {
    int n = adj.size();

    // Extract undirected edges
    vector<pair<int,int>> edges;

    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            if (u < v)
                edges.push_back({u, v});
        }
    }

    int m = edges.size();

    if (m == 0) return 0;

    // Exponential algorithm: use only for small m.
    if (m > 20) return -1;

    int FULL = 1 << m;

    vector<int> cycles;

    // Enumerate all subsets of edges.
    for (int mask = 1; mask < FULL; mask++) {
        vector<vector<int>> g(n);
        vector<int> degree(n, 0);

        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) {
                auto [u, v] = edges[i];

                g[u].push_back(v);
                g[v].push_back(u);

                degree[u]++;
                degree[v]++;
            }
        }

        // A simple cycle has degree 2
        // at every participating vertex.
        bool valid = true;
        int start = -1;

        for (int i = 0; i < n; i++) {
            if (degree[i] != 0 && degree[i] != 2)
                valid = false;

            if (degree[i] == 2)
                start = i;
        }

        if (!valid || start == -1) continue;

        // Check connectivity of participating vertices.
        vector<int> vis(n, 0);
        queue<int> q;

        q.push(start);
        vis[start] = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                if (!vis[v]) {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (degree[i] > 0 && !vis[i])
                valid = false;
        }

        if (valid)
            cycles.push_back(mask);
    }

    const int NEG = -1e9;

    // dp[mask] = maximum cycles covering exactly mask.
    vector<int> dp(FULL, NEG);
    dp[0] = 0;

    for (int mask = 1; mask < FULL; mask++) {
        for (int cyc : cycles) {
            if ((mask & cyc) == cyc &&
                dp[mask ^ cyc] != NEG) {

                dp[mask] = max(
                    dp[mask],
                    dp[mask ^ cyc] + 1
                );
            }
        }
    }

    return dp[FULL - 1];
}
