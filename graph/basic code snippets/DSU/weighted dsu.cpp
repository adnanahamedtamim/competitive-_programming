#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

int parent[N], sz[N];
long long diff[N];

// diff[u] = value[u] - value[parent[u]]

void init(int n) {
    for (int u = 1; u <= n; ++u) {
        parent[u] = u;
        sz[u] = 1;
        diff[u] = 0;
    }
}

int find(int u) {
    if (parent[u] == u)
        return u;

    int p = parent[u];
    parent[u] = find(p);
    diff[u] += diff[p];

    return parent[u];
}

// Adds value[u] - value[v] = w.
// Returns false if contradictory; rejects that constraint.
bool add_constraint(int u, int v, long long w) {
    int a = find(u);
    int b = find(v);

    if (a == b)
        return diff[u] - diff[v] == w;

    // Required difference: value[a] - value[b].
    long long delta = w - diff[u] + diff[v];

    if (sz[a] < sz[b]) {
        parent[a] = b;
        diff[a] = delta;
        sz[b] += sz[a];
    } else {
        parent[b] = a;
        diff[b] = -delta;
        sz[a] += sz[b];
    }

    return true;
}

// Returns value[u] - value[v], or nullopt if unknown.
optional<long long> difference(int u, int v) {
    int a = find(u);
    int b = find(v);

    if (a != b)
        return nullopt;

    return diff[u] - diff[v];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    init(n);

    while (q--) {
        char type;
        int u, v;
        cin >> type >> u >> v;

        if (type == '!') {
            long long w;
            cin >> w;

            if (!add_constraint(u, v, w))
                cout << "CONTRADICTION\n";
        } else if (type == '?') {
            auto answer = difference(u, v);

            if (answer.has_value())
                cout << *answer << '\n';
            else
                cout << "UNKNOWN\n";
        }
    }
}
