#include <bits/stdc++.h>
using namespace std;

struct RollbackDSU {
    struct Change {
        int child;
        int root;
        int old_size;
    };

    vector<int> parent, sz;
    vector<Change> history;
    int components;

    RollbackDSU(int n) : parent(n), sz(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    // No path compression!
    int find(int v) const {
        while (v != parent[v])
            v = parent[v];
        return v;
    }

    bool same(int a, int b) const {
        return find(a) == find(b);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (sz[a] < sz[b])
            swap(a, b);

        history.push_back({b, a, sz[a]});

        parent[b] = a;
        sz[a] += sz[b];
        --components;

        return true;
    }

    int snapshot() const {
        return (int)history.size();
    }

    void rollback(int checkpoint) {
        while ((int)history.size() > checkpoint) {
            Change c = history.back();
            history.pop_back();

            parent[c.child] = c.child;
            sz[c.root] = c.old_size;
            ++components;
        }
    }

    int size(int v) const {
        return sz[find(v)];
    }
};


int main(){ 
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);


    RollbackDSU dsu(5);

    dsu.unite(0, 1);

    int checkpoint = dsu.snapshot();

    dsu.unite(1, 2);
    dsu.unite(3, 4);

    cout << dsu.same(0, 2) << '\n'; // 1
    cout << dsu.components << '\n'; // 2

    dsu.rollback(checkpoint);

    cout << dsu.same(0, 2) << '\n'; // 0
    cout << dsu.same(0, 1) << '\n'; // 1
    cout << dsu.components << '\n'; // 4

}
