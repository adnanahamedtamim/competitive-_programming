
struct Node {
    ll sum = 0;

    Node() {}

    Node(ll val) {
        sum = val;
    }
};

class SegmentTree {
private:
    ll n;
    vector<Node> seg;

    vll lazyAdd;
    vll lazyAssign;
    vector<bool> hasAssign;

    // Merge two child nodes into one parent node
    Node merge(Node a, Node b) {
        Node res;
        res.sum = a.sum + b.sum;
        return res;
    }

    // PUSH UP:
    // Recalculate the parent using its two children.
    // Called after building or updating child segments.
    // Ensures the parent always contains the correct sum.
    void pushUp(ll node) {
        seg[node] = merge(
            seg[2 * node + 1],
            seg[2 * node + 2]
        );
    }

    void build(ll node, ll l, ll r, vector<ll>& ara) {
        if (l == r) {
            seg[node] = Node(ara[l]);
            return;
        }

        ll mid = l + (r - l) / 2;

        build(2 * node + 1, l, mid, ara);
        build(2 * node + 2, mid + 1, r, ara);

        pushUp(node);
    }

    // Apply range assignment directly to a node.
    // Every element in [l,r] becomes val.
    void applyAssign(ll node, ll l, ll r, ll val) {
        seg[node].sum = (r - l + 1) * val;

        hasAssign[node] = true;
        lazyAssign[node] = val;

        // Assignment overrides all previous additions.
        lazyAdd[node] = 0;
    }

    // Apply range addition directly to a node.
    // Every element in [l,r] increases by val.
    void applyAdd(ll node, ll l, ll r, ll val) {
        seg[node].sum += (r - l + 1) * val;

        if (hasAssign[node]) {
            // An assignment is already pending.
            // Example: assign 5, then add 3 = assign 8.
            lazyAssign[node] += val;
        }
        else {
            // Otherwise accumulate the pending addition.
            lazyAdd[node] += val;
        }
    }

    // PUSH DOWN:
    // Transfer pending lazy operations from parent to children.
    //
    // Why?
    // The parent sum is already correct after a range update,
    // but its children may still contain outdated values.
    // Before visiting those children, propagate the updates.
    //
    // Example:
    // Assign 5 to [0,7].
    // Parent sum becomes 40, but children are not updated yet.
    // When querying [0,3], pushDown updates the children first.
    //
    // Assignment must be propagated before addition.
    void pushDown(ll node, ll l, ll r) {
        if (l == r) return;

        ll mid = l + (r - l) / 2;

        ll left = 2 * node + 1;
        ll right = 2 * node + 2;

        // First propagate pending assignment.
        // This overrides any older updates in the children.
        if (hasAssign[node]) {
            applyAssign(left, l, mid, lazyAssign[node]);
            applyAssign(right, mid + 1, r, lazyAssign[node]);

            // Parent's assignment has been transferred.
            hasAssign[node] = false;
            lazyAssign[node] = 0;
        }

        // Then propagate pending addition, if any.
        if (lazyAdd[node] != 0) {
            applyAdd(left, l, mid, lazyAdd[node]);
            applyAdd(right, mid + 1, r, lazyAdd[node]);

            // Parent's addition has been transferred.
            lazyAdd[node] = 0;
        }
    }

    Node query(ll start, ll end, ll node, ll l, ll r) {
        if (r < start || l > end)
            return Node();

        if (start <= l && r <= end)
            return seg[node];

        // Children must receive pending updates before querying.
        pushDown(node, l, r);

        ll mid = l + (r - l) / 2;

        return merge(
            query(start, end, 2 * node + 1, l, mid),
            query(start, end, 2 * node + 2, mid + 1, r)
        );
    }

    void updateRange(ll node, ll l, ll r,
                     ll start, ll end, ll val) {

        if (r < start || l > end)
            return;

        if (start <= l && r <= end) {
            applyAdd(node, l, r, val);
            return;
        }

        // Push pending updates before modifying children.
        pushDown(node, l, r);

        ll mid = l + (r - l) / 2;

        updateRange(2 * node + 1, l, mid, start, end, val);
        updateRange(2 * node + 2, mid + 1, r, start, end, val);

        // Recalculate parent after children have changed.
        pushUp(node);
    }

    void assignRange(ll node, ll l, ll r,
                     ll start, ll end, ll val) {

        if (r < start || l > end)
            return;

        if (start <= l && r <= end) {
            applyAssign(node, l, r, val);
            return;
        }

        // Propagate older updates before applying new ones.
        pushDown(node, l, r);

        ll mid = l + (r - l) / 2;

        assignRange(2 * node + 1, l, mid, start, end, val);
        assignRange(2 * node + 2, mid + 1, r, start, end, val);

        // Rebuild current node using updated children.
        pushUp(node);
    }

public:
    // Constructor with initial array
    SegmentTree(vector<ll>& ara) {
        n = ara.size();

        seg.assign(4 * n, Node());
        lazyAdd.assign(4 * n, 0);
        lazyAssign.assign(4 * n, 0);
        hasAssign.assign(4 * n, false);

        if (n > 0)
            build(0, 0, n - 1, ara);
    }

    // Constructor with size n (all values = 0)
    SegmentTree(ll size) {
        n = size;

        seg.assign(4 * n, Node());
        lazyAdd.assign(4 * n, 0);
        lazyAssign.assign(4 * n, 0);
        hasAssign.assign(4 * n, false);
    }

    // Range sum query
    Node query(ll l, ll r) {
        return query(l, r, 0, 0, n - 1);
    }

    // Add val to every element in [l,r]
    void updateRange(ll l, ll r, ll val) {
        updateRange(0, 0, n - 1, l, r, val);
    }

    // Assign val to every element in [l,r]
    void assignRange(ll l, ll r, ll val) {
        assignRange(0, 0, n - 1, l, r, val);
    }

    // Point assignment
    void update(ll idx, ll val) {
        assignRange(idx, idx, val);
    }
};
