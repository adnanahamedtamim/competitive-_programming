
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Eertree {
    struct Node {
        int len = 0, link = 0, head = -1;
        int num = 0, firstEnd = -1;
        int diff = 0, seriesLink = 0;
        int seriesBest = 0;
        int occ = 0;

        Node(int l = 0) : len(l) {}
    };

    // 8 bytes per edge
    struct Edge {
        uint32_t packed; // high 8 bits: char, low 24: node
        int next;
    };

    struct History {
        int prevLast;
        int parent;       // -1 if no node was created
        int changeStart;
    };

    vector<Node> t;
    vector<Edge> edges;
    vector<History> history;

    // Saved seriesBest changes for rollback
    vector<pair<int, int>> changes;

    vector<int> bestHistory = {-1};
    vector<int> dp = {0};

    string s;
    int last = 1;
    ll total = 0;

    static constexpr int INF = 1e9;
    static constexpr int NODE_LIMIT = (1 << 24);

    // O(1), excluding optional allocation
    explicit Eertree(int capacity = 0) {
        if (capacity > 0) {
            if (capacity >= NODE_LIMIT - 2)
                throw length_error("Node limit exceeded");

            t.reserve(capacity + 2);
            edges.reserve(capacity);
            history.reserve(capacity);
            s.reserve(capacity);
            bestHistory.reserve(capacity + 1);
            dp.reserve(capacity + 1);
        }

        t.emplace_back(-1);
        t.emplace_back(0);

        t[0].link = 0;
        t[1].link = 0;
    }

    // O(26) = O(1)
    int getNext(int v, int c) const {
        for (int e = t[v].head;
             e != -1;
             e = edges[e].next) {

            uint32_t x = edges[e].packed;

            if ((int)(x >> 24) == c)
                return (int)(x & 0xFFFFFFu);
        }
        return 0;
    }

    // O(1)
    void addEdge(int v, int c, int to) {
        uint32_t packed =
            (uint32_t(c) << 24) | uint32_t(to);

        edges.push_back({packed, t[v].head});
        t[v].head = (int)edges.size() - 1;
    }

    // O(n) worst case per call
    int getLink(int v, int pos) const {
        while (true) {
            int left = pos - t[v].len - 1;

            if (left >= 0 && s[left] == s[pos])
                return v;

            v = t[v].link;
        }
    }

    // O(n) worst case for suffix-link search.
    // Series-link partition DP: O(log n) iterations.
    // Returns 1 if a new palindrome is created.
    int add(char ch) {
        if (ch < 'a' || ch > 'z')
            throw invalid_argument("Only lowercase a-z");

        if ((int)t.size() >= NODE_LIMIT - 1)
            throw length_error("Node limit exceeded");

        int prevLast = last;
        int pos = (int)s.size();
        int c = ch - 'a';

        s.push_back(ch);

        int cur = getLink(last, pos);
        int to = getNext(cur, c);

        int parent = -1;
        int created = 0;

        if (to == 0) {
            created = 1;
            parent = cur;

            int id = (int)t.size();
            t.emplace_back(t[cur].len + 2);
            addEdge(cur, c, id);

            int suffix = 1;

            if (t[id].len > 1) {
                int p = getLink(t[cur].link, pos);
                suffix = getNext(p, c);
            }

            t[id].link = suffix;
            t[id].firstEnd = pos;
            t[id].num = 1 + t[suffix].num;

            t[id].diff = t[id].len - t[suffix].len;

            if (t[id].diff == t[suffix].diff)
                t[id].seriesLink = t[suffix].seriesLink;
            else
                t[id].seriesLink = suffix;

            to = id;
        }

        last = to;
        t[last].occ++;
        total += t[last].num;

        int best = bestHistory.back();

        if (best == -1 || t[last].len > t[best].len)
            best = last;

        bestHistory.push_back(best);

        history.push_back({
            prevLast,
            parent,
            (int)changes.size()
        });

        // Optimized palindrome partition DP
        int answer = INF;
        int i = (int)s.size();

        for (int v = last;
             t[v].len > 0;
             v = t[v].seriesLink) {

            int sl = t[v].seriesLink;

            int index =
                i - (t[sl].len + t[v].diff);

            int value = dp[index];

            if (t[v].diff == t[t[v].link].diff) {
                value = min(
                    value,
                    t[t[v].link].seriesBest
                );
            }

            // Save old value for rollback
            changes.push_back({v, t[v].seriesBest});
            t[v].seriesBest = value;

            answer = min(answer, value + 1);
        }

        dp.push_back(answer);
        return created;
    }

    // O(log n) for restoring series-link DP changes
    // Other rollback work is O(1).
    void pop_back() {
        if (s.empty())
            return;

        total -= t[last].num;
        t[last].occ--;

        History h = history.back();
        history.pop_back();

        dp.pop_back();
        bestHistory.pop_back();

        // Undo series-link DP changes
        while ((int)changes.size() > h.changeStart) {
            auto [v, oldValue] = changes.back();
            changes.pop_back();

            t[v].seriesBest = oldValue;
        }

        // Remove newly created node and edge
        if (h.parent != -1) {
            int e = t[h.parent].head;

            t[h.parent].head = edges[e].next;

            edges.pop_back();
            t.pop_back();
        }

        last = h.prevLast;
        s.pop_back();
    }

    // O(1)
    int size() const {
        return (int)s.size();
    }

    // O(1)
    int distinct() const {
        return (int)t.size() - 2;
    }

    // O(1)
    ll totalPalindromes() const {
        return total;
    }

    // O(1)
    int longestPalindromeLength() const {
        int v = bestHistory.back();
        return v == -1 ? 0 : t[v].len;
    }

    // O(L)
    string getPalindrome(int v) const {
        if (v < 2 || v >= (int)t.size())
            return "";

        int len = t[v].len;
        int end = t[v].firstEnd;

        return s.substr(end - len + 1, len);
    }

    // O(L)
    string longestPalindrome() const {
        return getPalindrome(bestHistory.back());
    }

    // O(1)
    int longestSuffixLength() const {
        return t[last].len;
    }

    // O(L)
    string longestSuffix() const {
        if (s.empty())
            return "";

        return s.substr(
            s.size() - t[last].len,
            t[last].len
        );
    }

    // O(1)
    int palindromesEndingHere() const {
        return t[last].num;
    }

    // O(1)
    int minimumPartition() const {
        return dp.back();
    }

    // O(n) time, O(n) temporary space
    vector<ll> frequencies() const {
        int m = (int)t.size();
        vector<ll> cnt(m, 0);

        for (int v = 2; v < m; v++)
            cnt[v] = t[v].occ;

        // Reverse creation order:
        // suffix links always point to older nodes.
        for (int v = m - 1; v >= 2; v--)
            cnt[t[v].link] += cnt[v];

        return cnt;
    }

    // O(n + L)
    string mostFrequent() const {
        if (t.size() == 2)
            return "";

        auto cnt = frequencies();
        int best = 2;

        for (int v = 3; v < (int)t.size(); v++) {
            if (cnt[v] > cnt[best])
                best = v;
        }

        return getPalindrome(best);
    }

    // O(n + L)
    string longestRepeated(ll k) const {
        auto cnt = frequencies();
        int best = -1;

        for (int v = 2; v < (int)t.size(); v++) {
            if (cnt[v] >= k &&
                (best == -1 ||
                 t[v].len > t[best].len)) {
                best = v;
            }
        }

        return getPalindrome(best);
    }

    // O(n + total length of returned strings)
    vector<pair<string, ll>> allPalindromes() const {
        auto cnt = frequencies();

        vector<pair<string, ll>> result;
        result.reserve(distinct());

        for (int v = 2; v < (int)t.size(); v++) {
            result.push_back({
                getPalindrome(v), cnt[v]
            });
        }

        return result;
    }

    // O(n + total printed characters)
    void printFrequencies() const {
        auto cnt = frequencies();

        for (int v = 2; v < (int)t.size(); v++) {
            cout << getPalindrome(v)
                 << " : " << cnt[v] << '\n';
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Eertree et;

    for (char c : string("ababa"))
        et.add(c);

    cout << "String: " << et.s << '\n';
    cout << "Distinct: " << et.distinct() << '\n';
    cout << "Total: " << et.totalPalindromes() << '\n';
    cout << "Longest: " << et.longestPalindrome() << '\n';
    cout << "Longest suffix: " << et.longestSuffix() << '\n';
    cout << "Ending here: "
         << et.palindromesEndingHere() << '\n';
    cout << "Min partitions: "
         << et.minimumPartition() << '\n';
    cout << "Most frequent: "
         << et.mostFrequent() << '\n';
    cout << "Repeated >= 2: "
         << et.longestRepeated(2) << '\n';

    cout << "\nFrequencies:\n";
    et.printFrequencies();

    et.pop_back();

    cout << "\nAfter pop: " << et.s << '\n';
    cout << "Min partitions: "
         << et.minimumPartition() << '\n';

    et.add('c');

    cout << "\nAfter adding c: " << et.s << '\n';
    cout << "Longest: "
         << et.longestPalindrome() << '\n';
}
