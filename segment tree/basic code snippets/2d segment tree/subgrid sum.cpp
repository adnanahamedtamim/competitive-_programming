#include <bits/stdc++.h>
using namespace std;

// 1. Define the Node (Range Sum implementation)
struct Node {
    long long val;
    
    Node() : val(0) {} // Identity element (0 for sum)
    Node(long long v) : val(v) {}

    static Node merge(const Node& left, const Node& right) {
        return Node(left.val + right.val); // Range Sum combination
    }
};

struct SegmentTree2D {
    int n, m;
    vector<vector<Node>> tree;

    SegmentTree2D(int _n, int _m) : n(_n), m(_m) {
        tree.assign(4 * n, vector<Node>(4 * m));
    }

    // --- INNER COLUMN TREE HELPERS ---
    void updateY(int nodeX, int lx, int rx, int nodeY, int ly, int ry, int y, long long val, bool isLeafX) {
        if (ly == ry) {
            if (isLeafX) {
                tree[nodeX][nodeY] = Node(val);
            } else {
                tree[nodeX][nodeY] = Node::merge(tree[2 * nodeX][nodeY], tree[2 * nodeX + 1][nodeY]);
            }
            return;
        }
        int midY = ly + (ry - ly) / 2;
        if (y <= midY) updateY(nodeX, lx, rx, 2 * nodeY, ly, midY, y, val, isLeafX);
        else updateY(nodeX, lx, rx, 2 * nodeY + 1, midY + 1, ry, y, val, isLeafX);
        
        tree[nodeX][nodeY] = Node::merge(tree[nodeX][2 * nodeY], tree[nodeX][2 * nodeY + 1]);
    }

    Node queryY(int nodeX, int nodeY, int ly, int ry, int qy1, int qy2) {
        if (qy1 > ry || qy2 < ly) return Node();
        if (qy1 <= ly && ry <= qy2) return tree[nodeX][nodeY];
        
        int midY = ly + (ry - ly) / 2;
        Node leftRes = queryY(nodeX, 2 * nodeY, ly, midY, qy1, qy2);
        Node rightRes = queryY(nodeX, 2 * nodeY + 1, midY + 1, ry, qy1, qy2);
        return Node::merge(leftRes, rightRes);
    }

    // --- OUTER ROW TREE HELPERS ---
    void updateX(int nodeX, int lx, int rx, int x, int y, long long val) {
        if (lx != rx) {
            int midX = lx + (rx - lx) / 2;
            if (x <= midX) updateX(2 * nodeX, lx, midX, x, y, val);
            else updateX(2 * nodeX + 1, midX + 1, rx, x, y, val);
        }
        updateY(nodeX, lx, rx, 1, 1, m, y, val, (lx == rx));
    }

    Node queryX(int nodeX, int lx, int rx, int qx1, int qx2, int qy1, int qy2) {
        if (qx1 > rx || qx2 < lx) return Node();
        if (qx1 <= lx && rx <= qx2) {
            return queryY(nodeX, 1, 1, m, qy1, qy2);
        }
        
        int midX = lx + (rx - lx) / 2;
        Node leftRes = queryX(2 * nodeX, lx, midX, qx1, qx2, qy1, qy2);
        Node rightRes = queryX(2 * nodeX + 1, midX + 1, rx, qx1, qx2, qy1, qy2);
        return Node::merge(leftRes, rightRes);
    }

    // --- PUBLIC INTERFACE ---
    void update(int x, int y, long long val) {
        updateX(1, 1, n, x, y, val);
    }

    long long query(int qx1, int qy1, int qx2, int qy2) {
        return queryX(1, 1, n, qx1, qx2, qy1, qy2).val;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 4, m = 4; // 4x4 Grid
    SegmentTree2D st(n, m);

    // Initial grid values / point updates
    // Let's populate some cells (1-based indexing)
    st.update(1, 1, 1);
    st.update(1, 2, 2);
    st.update(2, 1, 3);
    st.update(2, 2, 4);
    st.update(3, 3, 5);

    // Test query: sum of rectangle from (1, 1) to (2, 2)
    // Should be 1 + 2 + 3 + 4 = 10
    cout << "Sum from (1,1) to (2,2): " << st.query(1, 1, 2, 2) << "\n";

    // Test query: sum of rectangle from (1, 1) to (3, 3)
    // Should be 10 + 5 = 15
    cout << "Sum from (1,1) to (3,3): " << st.query(1, 1, 3, 3) << "\n";

    // Update cell (2, 2) by adding 6 (making it 4 + 6 = 10)
    st.update(2, 2, 6);

    // Query again from (1, 1) to (2, 2) -> should now be 1 + 2 + 3 + 10 = 16
    cout << "Sum from (1,1) to (2,2) after update: " << st.query(1, 1, 2, 2) << "\n";

    return 0;
}
