#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 2e18; // Large enough for long long infinity

struct Line {
    long long m, c;
    long long eval(long long x) const {
        if (c == INF) return INF; // Handle "empty" lines safely
        return m * x + c;
    }
};

// Dynamic Li Chao Tree tailored for MINIMIZATION
class LiChaoTree {
private:
    struct Node {
        Line line;
        int left_child, right_child;
        Node() : line({0, INF}), left_child(-1), right_child(-1) {}
    };

    vector<Node> tree;
    long long MIN_X, MAX_X;

    // Helper to get or create a child node
    int get_child(int& child_idx) {
        if (child_idx == -1) {
            child_idx = tree.size();
            tree.emplace_back();
        }
        return child_idx;
    }

    // Internal function to add an infinite line to a specific node range
    void add_line_internal(int node, long long l, long long r, Line new_line) {
        long long mid = l + (r - l) / 2;
        
        bool left_better = new_line.eval(l) < tree[node].line.eval(l);
        bool mid_better = new_line.eval(mid) < tree[node].line.eval(mid);

        if (mid_better) {
            swap(tree[node].line, new_line);
        }

        if (l == r) return;

        if (left_better != mid_better) {
            add_line_internal(get_child(tree[node].left_child), l, mid, new_line);
        } else {
            add_line_internal(get_child(tree[node].right_child), mid + 1, r, new_line);
        }
    }

    // Internal function to add a line segment valid ONLY on [seg_L, seg_R]
    void add_segment_internal(int node, long long l, long long r, long long seg_L, long long seg_R, Line new_line) {
        // If the current node's range is completely outside the segment bounds, ignore it
        if (l > seg_R || r < seg_L) return;

        // If the current node's range is completely inside the segment bounds, 
        // treat it as an infinite line for this subtree
        if (seg_L <= l && r <= seg_R) {
            add_line_internal(node, l, r, new_line);
            return;
        }

        long long mid = l + (r - l) / 2;
        
        // Otherwise, the segment partially covers this node. Recurse down.
        // Notice we DO NOT evaluate the line at the midpoint here, because the line 
        // might not mathematically exist at the midpoint of this node!
        add_segment_internal(get_child(tree[node].left_child), l, mid, seg_L, seg_R, new_line);
        add_segment_internal(get_child(tree[node].right_child), mid + 1, r, seg_L, seg_R, new_line);
    }

    // Internal function to query the minimum value at x
    long long query_internal(int node, long long l, long long r, long long x) {
        if (node == -1) return INF; // Reached an unallocated subtree
        
        long long val = tree[node].line.eval(x);
        if (l == r) return val;

        long long mid = l + (r - l) / 2;
        if (x <= mid) {
            return min(val, query_internal(tree[node].left_child, l, mid, x));
        } else {
            return min(val, query_internal(tree[node].right_child, mid + 1, r, x));
        }
    }

public:
    // Constructor: Define the absolute minimum and maximum x-coordinates you will ever query
    LiChaoTree(long long min_x, long long max_x) {
        MIN_X = min_x;
        MAX_X = max_x;
        tree.emplace_back(); // Initialize root at index 0
    }

    // Add a line y = mx + c valid from x = -infinity to +infinity
    void add_line(long long m, long long c) {
        add_line_internal(0, MIN_X, MAX_X, {m, c});
    }

    // Add a line segment y = mx + c valid ONLY for x in the range [seg_L, seg_R]
    void add_segment(long long seg_L, long long seg_R, long long m, long long c) {
        // Clamp the segment to our allowed domain
        seg_L = max(seg_L, MIN_X);
        seg_R = min(seg_R, MAX_X);
        if (seg_L > seg_R) return;
        
        add_segment_internal(0, MIN_X, MAX_X, seg_L, seg_R, {m, c});
    }

    // Find the minimum y value at coordinate x
    long long query(long long x) {
        if (x < MIN_X || x > MAX_X) return INF;
        return query_internal(0, MIN_X, MAX_X, x);
    }
};

int main() {
    // Example Usage for "Segment Add Get Min"
    
    // We expect x coordinates (queries and bounds) to be between -10^9 and 10^9
    LiChaoTree lct(-1000000000, 1000000000);

    // Add a segment y = 2x + 5 valid only for x between 0 and 10
    lct.add_segment(0, 10, 2, 5);
    
    // Add an infinite line y = -x + 20
    lct.add_line(-1, 20);

    // Query at x = 5
    // Segment gives: 2(5) + 5 = 15
    // Infinite line gives: -5 + 20 = 15
    cout << "Min at x=5: " << lct.query(5) << "\n"; // Output: 15

    // Query at x = -5
    // Segment does NOT exist here.
    // Infinite line gives: -(-5) + 20 = 25
    cout << "Min at x=-5: " << lct.query(-5) << "\n"; // Output: 25

    return 0;
}
