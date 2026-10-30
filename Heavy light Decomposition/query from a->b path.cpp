// Helper: O(log N) HLD LCA
ll get_lca(ll u, ll v) {
    while (head[u] != head[v]) {
        if (depth_[head[u]] < depth_[head[v]]) swap(u, v);
        u = parent_[head[u]];
    }
    return depth_[u] < depth_[v] ? u : v;
}

// Returns 1-based index of the FIRST black node on directed path a -> b (or -1 if none)
ll queryPathFirstBlack(ll a, ll b, Fenwick &fw) {
    ll L = get_lca(a, b);

    // =========================================================================
    // PART 1: Climb UP from `a` to `L` (Moving bottom-up: right-to-left in pos[])
    // We stop at the VERY FIRST chain segment that has sum > 0!
    // =========================================================================
    ll curr = a;
    while (true) {
        // If `L` is in the same chain as `curr`, top of segment is `L`; otherwise `head[curr]`
        ll top = (head[curr] == head[L]) ? L : head[curr];
        ll l_idx = pos[top], r_idx = pos[curr];

        if (fw.rangeSum(l_idx, r_idx) > 0) {
            // Since we are moving UP from curr -> top, we want the DEEPEST (rightmost) black node!
            ll lo = l_idx, hi = r_idx, ans = -1;
            while (lo <= hi) {
                ll mid = (lo + hi) / 2;
                if (fw.rangeSum(mid, r_idx) > 0) { // Check RIGHT half [mid, r_idx]
                    ans = nodeatpos[mid];
                    lo = mid + 1;                  // Push mid further RIGHT (deeper)
                } else {
                    hi = mid - 1;
                }
            }
            return ans + 1;
        }
        if (top == L) break;
        curr = parent_[top];
    }

    // =========================================================================
    // PART 2: Climb UP from `b` to `L` (Corresponds to going DOWN from L -> b)
    // Exact same logic as your original code: overwrite `best_l, best_r` so the
    // HIGHEST chain segment closest to `L` wins!
    // =========================================================================
    ll best_l = -1, best_r = -1;
    curr = b;
    while (true) {
        ll top = (head[curr] == head[L]) ? L : head[curr];
        ll l_idx = pos[top], r_idx = pos[curr];

        if (fw.rangeSum(l_idx, r_idx) > 0) {
            best_l = l_idx;
            best_r = r_idx; // Overwrite: remembers the segment closest to L
        }
        if (top == L) break;
        curr = parent_[top];
    }

    if (best_l == -1) return -1; // No black node anywhere on a -> b

    // Since L -> b goes DOWN the chain, we want the SHALLOWEST (leftmost) black node!
    ll lo = best_l, hi = best_r, ans = -1;
    while (lo <= hi) {
        ll mid = (lo + hi) / 2;
        if (fw.rangeSum(lo, mid) > 0) { // Check LEFT half [lo, mid]
            ans = nodeatpos[mid];
            hi = mid - 1;               // Push mid further LEFT (shallower)
        } else {
            lo = mid + 1;
        }
    }
    return ans + 1;
}
