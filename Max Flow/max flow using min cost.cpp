// all edges also have cost per unit flow with their capacity, wehave to find the max flow with minimum cost

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

struct Edge {
    int to;
    ll cap;
    ll cost;
    ll flow;
    int rev;
};

struct MinCostMaxFlow {
    int n;
    vector<vector<Edge>> adj;
    vector<ll> dist;
    vector<int> parent_node, parent_edge;
    vector<bool> in_queue;

    MinCostMaxFlow(int n) : n(n), adj(n), dist(n), parent_node(n), parent_edge(n), in_queue(n) {}

    void add_edge(int from, int to, ll cap, ll cost) {
        adj[from].push_back({to, cap, cost, 0, (int)adj[to].size()});
        adj[to].push_back({from, 0, -cost, 0, (int)adj[from].size() - 1});
    }

    bool spfa(int s, int t) {
        fill(dist.begin(), dist.end(), INF);
        fill(in_queue.begin(), in_queue.end(), false);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        in_queue[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = false;

            for (int i = 0; i < adj[u].size(); i++) {
                auto& edge = adj[u][i];
                if (edge.cap - edge.flow > 0 && dist[edge.to] > dist[u] + edge.cost) {
                    dist[edge.to] = dist[u] + edge.cost;
                    parent_node[edge.to] = u;
                    parent_edge[edge.to] = i;
                    if (!in_queue[edge.to]) {
                        q.push(edge.to);
                        in_queue[edge.to] = true;
                    }
                }
            }
        }
        return dist[t] != INF;
    }

    pair<ll, ll> min_cost_max_flow(int s, int t) {
        ll max_flow = 0;
        ll min_cost = 0;

        while (spfa(s, t)) {
            // Find bottleneck capacity along the path found by SPFA
            ll push = INF;
            for (int v = t; v != s; v = parent_node[v]) {
                int u = parent_node[v];
                int idx = parent_edge[v];
                push = min(push, adj[u][idx].cap - adj[u][idx].flow);
            }

            // Push flow and add costs
            for (int v = t; v != s; v = parent_node[v]) {
                int u = parent_node[v];
                int idx = parent_edge[v];
                adj[u][idx].flow += push;
                int rev_idx = adj[u][idx].rev;
                adj[v][rev_idx].flow -= push;
                min_cost += push * adj[u][idx].cost;
            }

            max_flow += push;
        }

        return {max_flow, min_cost};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int src = 1, sink = n;
    MinCostMaxFlow mcmf(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        ll cap, cost;
        cin >> u >> v >> cap >> cost;
        mcmf.add_edge(u, v, cap, cost);
    }

    auto [max_flow, min_cost] = mcmf.min_cost_max_flow(src, sink);

    cout << max_flow << " " << min_cost << '\n';

    return 0;
}
