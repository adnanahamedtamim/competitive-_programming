cpp_int best;
bool found = false;

void updateBest(const cpp_int& value) {
    if (!found || value < best) {
        best = value;
        found = true;
    }
}

void dfsMin(int start, ll remaining, const cpp_int& value) {
    // Current value is a complete answer.
    if (remaining == 1) {
        updateBest(value);
        return;
    }

    // Every completion must be at least this large.
    cpp_int lowerBound = value * (cpp_int(remaining) + 1);

    if (found && lowerBound >= best) {
        return;
    }

    // Can we achieve the lower bound using one allowed prime?
    ll p = remaining + 1;

    auto it = lower_bound(
        candidates.begin() + start,
        candidates.end(),
        p
    );

    if (it != candidates.end() && *it == p) {
        updateBest(lowerBound);
        return; // Optimal completion of this state.
    }

    for (int i = start; i < (int)candidates.size(); ++i) {
        p = candidates[i];

        if (p - 1 > remaining) break;
        if (remaining % (p - 1) != 0) continue;

        ll contribution = p - 1;
        cpp_int nextValue = value * p;

        while (remaining % contribution == 0) {
            // Further factors only increase nextValue.
            if (found && nextValue >= best) break;

            dfsMin(
                i + 1,
                remaining / contribution,
                nextValue
            );

            if (contribution > remaining / p) break;

            contribution *= p;
            nextValue *= p;
        }
    }
}
