
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Eertree {
    // 12 bytes per node
    struct Node {
        int len, link, head;

        Node(int l = 0)
            : len(l), link(0), head(-1) {}
    };

    // 8 bytes per edge
    // High 8 bits: character (0..25)
    // Low 24 bits: destination node ID
    struct Edge {
        uint32_t packed;
        int next;
    };

    const string& s;

    vector<Node> tree;
    vector<Edge> edges;

    // Metadata for each palindrome node
    vector<int> firstEnd;
    vector<int> num;
    vector<int> seriesLink;
    vector<int> diff;
    vector<ll> occ;

    // Longest palindromic suffix node for every prefix
    vector<int> prefixLast;

    int last = 1;
    int best = -1;
    ll total = 0;

    // Cached propagated frequencies
    mutable vector<ll> freqCache;
    mutable bool freqValid = false;

    // O(1), plus reserve allocation
    explicit Eertree(const string& str) : s(str) {
        int n = (int)s.size();

        if ((size_t)n + 2 >= (1u << 24))
            throw length_error("Too many Eertree nodes");

        tree.reserve(n + 2);
        edges.reserve(n);

        firstEnd.reserve(n + 2);
        num.reserve(n + 2);
        occ.reserve(n + 2);
        diff.reserve(n + 2);
        seriesLink.reserve(n + 2);
        prefixLast.reserve(n);

        tree.emplace_back(-1);
        tree.emplace_back(0);

        for (int i = 0; i < 2; i++) {
            firstEnd.push_back(-1);
            num.push_back(0);
            occ.push_back(0);
            diff.push_back(0);
            seriesLink.push_back(0);
        }
    }

    // O(26) = O(1)
    int getNext(int v, int c) const {
        for (int e = tree[v].head;
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
        uint32_t x =
            (uint32_t(c) << 24) | uint32_t(to);

        edges.push_back({x, tree[v].head});
        tree[v].head = (int)edges.size() - 1;
    }

    // O(n) worst case for one call
    // Amortized O(1) over append-only construction
    int getLink(int v, int pos) const {
        while (true) {
            int left = pos - tree[v].len - 1;

            if (left >= 0 && s[left] == s[pos])
                return v;

            v = tree[v].link;
        }
    }

    // O(1) amortized over the full build
    // Returns 1 if a new palindrome was created
    int add(int pos) {
        int c = s[pos] - 'a';

        int cur = getLink(last, pos);
        int to = getNext(cur, c);

        if (to != 0) {
            last = to;
            occ[last]++;
            total += num[last];
            prefixLast.push_back(last);
            freqValid = false;
            return 0;
        }

        int id = (int)tree.size();
        tree.emplace_back(tree[cur].len + 2);
        addEdge(cur, c, id);

        int suffix = 1;

        if (tree[id].len > 1) {
            int p = getLink(tree[cur].link, pos);
            suffix = getNext(p, c);
        }

        tree[id].link = suffix;

        firstEnd.push_back(pos);
        num.push_back(1 + num[suffix]);
        occ.push_back(0);

        int d = tree[id].len - tree[suffix].len;
        diff.push_back(d);

        if (d == diff[suffix])
            seriesLink.push_back(seriesLink[suffix]);
        else
            seriesLink.push_back(suffix);

        last = id;
        occ[last]++;
        total += num[last];

        if (best == -1 ||
            tree[id].len > tree[best].len) {
            best = id;
        }

        prefixLast.push_back(last);
        freqValid = false;

        return 1;
    }

    // O(n) total
    void build() {
        for (int i = 0; i < (int)s.size(); i++)
            add(i);
    }

    // O(1)
    int distinct() const {
        return (int)tree.size() - 2;
    }

    // O(1)
    ll totalPalindromes() const {
        return total;
    }

    // O(1)
    int longestPalindromeLength() const {
        return best == -1 ? 0 : tree[best].len;
    }

    // O(L)
    string getPalindrome(int v) const {
        if (v < 2 || v >= (int)tree.size())
            return "";

        int len = tree[v].len;
        int end = firstEnd[v];

        return s.substr(end - len + 1, len);
    }

    // O(L)
    string longestPalindrome() const {
        return getPalindrome(best);
    }

    // O(1)
    int longestSuffixLength() const {
        return tree[last].len;
    }

    // O(L)
    string longestSuffix() const {
        if (prefixLast.empty())
            return "";

        return s.substr(
            prefixLast.size() - tree[last].len,
            tree[last].len
        );
    }

    // O(1)
    int palindromesEndingHere() const {
        return num[last];
    }

    // O(n) first call after add(), O(1) cached
    const vector<ll>& frequencies() const {
        if (freqValid)
            return freqCache;

        int m = (int)tree.size();

        freqCache.assign(m, 0);

        for (int v = 2; v < m; v++)
            freqCache[v] = occ[v];

        for (int v = m - 1; v >= 2; v--)
            freqCache[tree[v].link] += freqCache[v];

        freqValid = true;
        return freqCache;
    }

    // O(n + L)
    // Frequencies are cached
    string mostFrequent() const {
        if (best == -1)
            return "";

        const auto& cnt = frequencies();

        int ans = 2;

        for (int v = 3; v < (int)tree.size(); v++) {
            if (cnt[v] > cnt[ans])
                ans = v;
        }

        return getPalindrome(ans);
    }

    // O(n + L)
    string longestRepeated(ll k) const {
        const auto& cnt = frequencies();

        int ans = -1;

        for (int v = 2; v < (int)tree.size(); v++) {
            if (cnt[v] >= k &&
                (ans == -1 ||
                 tree[v].len > tree[ans].len)) {
                ans = v;
            }
        }

        return getPalindrome(ans);
    }

    // O(n + total returned string lengths)
    vector<pair<string, ll>> allPalindromes() const {
        const auto& cnt = frequencies();

        vector<pair<string, ll>> result;
        result.reserve(distinct());

        for (int v = 2; v < (int)tree.size(); v++) {
            result.push_back({
                getPalindrome(v), cnt[v]
            });
        }

        return result;
    }

    // O(n + total printed characters)
    void printFrequencies() const {
        const auto& cnt = frequencies();

        for (int v = 2; v < (int)tree.size(); v++) {
            cout << getPalindrome(v)
                 << " : " << cnt[v] << '\n';
        }
    }

    // O(n^2) worst case, O(n) extra space
    // Does NOT rebuild another Eertree
    int minimumPartition() const {
        int n = (int)prefixLast.size();
        const int INF = 1e9;

        vector<int> dp(n + 1, INF);
        dp[0] = 0;

        for (int i = 1; i <= n; i++) {
            int v = prefixLast[i - 1];

            while (tree[v].len > 0) {
                int len = tree[v].len;

                dp[i] = min(
                    dp[i],
                    dp[i - len] + 1
                );

                v = tree[v].link;
            }
        }

        return dp[n];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    Eertree et(s);
    et.build();

    cout << "Distinct: "
         << et.distinct() << '\n';

    cout << "Total occurrences: "
         << et.totalPalindromes() << '\n';

    cout << "Longest palindrome: "
         << et.longestPalindrome() << '\n';

    cout << "Longest length: "
         << et.longestPalindromeLength() << '\n';

    cout << "Longest suffix: "
         << et.longestSuffix() << '\n';

    cout << "Palindromes ending here: "
         << et.palindromesEndingHere() << '\n';

    cout << "Most frequent: "
         << et.mostFrequent() << '\n';

    cout << "Longest repeated >= 2: "
         << et.longestRepeated(2) << '\n';

    cout << "Minimum partitions: "
         << et.minimumPartition() << '\n';

    cout << "\nFrequencies:\n";
    et.printFrequencies();
}
