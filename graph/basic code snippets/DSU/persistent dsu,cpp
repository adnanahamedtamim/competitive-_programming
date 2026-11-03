#include <bits/stdc++.h>
using namespace std;

struct PersistentDSU {
  
    struct Node {
        int left = 0, right = 0;
        int value = -1;
    };

    int n, initial;
    vector<Node> tree;

    // Vertices are numbered 1...n. Requires n >= 1.
    PersistentDSU(int n) : n(n) {
        tree.push_back(Node{}); // Dummy node.
        initial = build(1, n);
    }

    int build(int l, int r) {
        int id = (int)tree.size();
        tree.push_back(Node{});

        if (l == r)
            return id;

        int mid = (l + r) / 2;
        int left = build(l, mid);
        int right = build(mid + 1, r);

        tree[id].left = left;
        tree[id].right = right;
        return id;
    }

    // Read data[pos] in the version represented by root.
    int get(int root, int pos) const {
        int l = 1, r = n;

        while (l < r) {
            int mid = (l + r) / 2;

            if (pos <= mid) {
                root = tree[root].left;
                r = mid;
            } else {
                root = tree[root].right;
                l = mid + 1;
            }
        }

        return tree[root].value;
    }

    // Return a new root after setting data[pos] = value.
    int update(int root, int l, int r, int pos, int value) {
        Node copy = tree[root];
        int id = (int)tree.size();
        tree.push_back(copy);

        if (l == r) {
            tree[id].value = value;
            return id;
        }

        int mid = (l + r) / 2;

        if (pos <= mid) {
            int child = update(copy.left, l, mid, pos, value);
            tree[id].left = child;
        } else {
            int child = update(copy.right, mid + 1, r, pos, value);
            tree[id].right = child;
        }

        return id;
    }

    int find(int root, int u) const { //0(log^2n)
        int p = get(root, u);

        while (p >= 0) {
            u = p;
            p = get(root, u);
        }

        return u;
    }

    // Return a new version; the supplied version stays unchanged.
    int unite(int root, int u, int v) {
        u = find(root, u);
        v = find(root, v);

        if (u == v)
            return root;

        int su = get(root, u); // Negative component sizes.
        int sv = get(root, v);

        // More negative means larger.
        if (su > sv) {
            swap(u, v);
            swap(su, sv);
        }

        root = update(root, 1, n, u, su + sv);
        root = update(root, 1, n, v, u);

        return root;
    }

    bool same(int root, int u, int v) const {
        return find(root, u) == find(root, v);
    }

    int size(int root, int u) const {
        return -get(root, find(root, u));
    }
};

int main() {
    PersistentDSU dsu(5);
    vector<int> root(4);

    root[0] = dsu.initial;

    root[1] = dsu.unite(root[0], 1, 2);
    root[2] = dsu.unite(root[1], 2, 3);

    // Branch from version 1.
    root[3] = dsu.unite(root[1], 4, 5);

    cout << dsu.same(root[0], 1, 2) << '\n'; // 0
    cout << dsu.same(root[1], 1, 2) << '\n'; // 1

    cout << dsu.same(root[2], 1, 3) << '\n'; // 1
    cout << dsu.same(root[3], 1, 3) << '\n'; // 0

    cout << dsu.same(root[3], 4, 5) << '\n'; // 1
    cout << dsu.size(root[2], 1) << '\n';    // 3
}
