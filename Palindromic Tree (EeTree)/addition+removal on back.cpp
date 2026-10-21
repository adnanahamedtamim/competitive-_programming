
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Eertree {
    struct Node {
        int len, link;
        int next[26] = {};

        int num = 0;
        int firstEnd = -1;
        ll occ = 0;

        Node(int l = 0) : len(l), link(0) {}
    };

    struct History {
        int prevLast;
        int parent;
        int c;
        bool created;
    };

    vector<Node> tree;
    vector<History> history;

    string s;
    int last = 1;
    ll total = 0;

    // Best palindrome node for each prefix
    vector<int> bestHistory = {-1};

    // Minimum palindrome partition DP for each prefix
    vector<int> dp = {0};

    // O(1)
    Eertree() {
        tree.emplace_back(-1); // Node 0: odd root
        tree.emplace_back(0);  // Node 1: even root

        tree[0].link = 0;
        tree[1].link = 0;
    }

    // Amortized O(1) in standard append-only construction.
    // O(n) worst case for one call.
    int getLink(int v, int pos) {
        while (true) {
            int left = pos - tree[v].len - 1;

            if (left >= 0 && s[left] == s[pos])
                return v;

            v = tree[v].link;
        }
    }

    // O(n) worst case per add() with rollback.
    // Includes O(n) worst-case partition DP.
    void add(char ch) {
        int prevLast = last;

        s += ch;
        int pos = (int)s.size() - 1;
        int c = ch - 'a';

        int cur = getLink(last, pos);
        bool created = false;

        if (tree[cur].next[c] == 0) {
            created = true;

            int id = (int)tree.size();
            tree.emplace_back(tree[cur].len + 2);

            tree[cur].next[c] = id;
            tree[id].firstEnd = pos;

            if (tree[id].len == 1) {
                tree[id].link = 1;
            } else {
                int p = getLink(tree[cur].link, pos);
                tree[id].link = tree[p].next[c];
            }

            tree[id].num =
                1 + tree[tree[id].link].num;
        }

        last = tree[cur].next[c];

        history.push_back({
            prevLast, cur, c, created
        });

        // Total palindrome occurrences
        total += tree[last].num;

        // Raw frequency count
        tree[last].occ++;

        // Longest palindrome in current prefix
        int best = bestHistory.back();

        if (best == -1 ||
            tree[last].len > tree[best].len) {
            best = last;
        }

        bestHistory.push_back(best);

        // Minimum palindrome partition DP
        int answer = INT_MAX;

        for (int v = last;
             tree[v].len > 0;
             v = tree[v].link) {

            int len = tree[v].len;

            answer = min(
                answer,
                dp[pos + 1 - len] + 1
            );
        }

        dp.push_back(answer);
    }

    // O(1)
    void pop_back() {
        if (s.empty())
            return;

        // Undo statistics for the final position
        total -= tree[last].num;
        tree[last].occ--;

        History h = history.back();
        history.pop_back();

        bestHistory.pop_back();
        dp.pop_back();

        // Undo newly created node and transition
        if (h.created) {
            tree[h.parent].next[h.c] = 0;
            tree.pop_back();
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
        return (int)tree.size() - 2;
    }

    // O(1)
    ll totalPalindromes() const {
        return total;
    }

    // O(1)
    int longestPalindromeLength() const {
        int v = bestHistory.back();
        return v == -1 ? 0 : tree[v].len;
    }

    // O(L), where L = returned string length
    string longestPalindrome() const {
        int v = bestHistory.back();

        if (v == -1)
            return "";

        return getPalindrome(v);
    }

    // O(1)
    int longestSuffixLength() const {
        return tree[last].len;
    }

    // O(L), where L = returned string length
    string longestSuffix() const {
        if (s.empty())
            return "";

        return s.substr(
            s.size() - tree[last].len
        );
    }

    // O(1)
    int palindromesEndingHere() const {
        return tree[last].num;
    }

    // O(1)
    int minimumPartition() const {
        return dp.back();
    }

    // O(L), where L = palindrome length
    string getPalindrome(int v) const {
        if (v < 2 || v >= (int)tree.size())
            return "";

        int len = tree[v].len;
        int end = tree[v].firstEnd;

        return s.substr(end - len + 1, len);
    }

    // O(n), where n = current string length
    vector<ll> frequencies() const {
        int m = (int)tree.size();
        vector<ll> cnt(m, 0);

        for (int v = 2; v < m; v++)
            cnt[v] = tree[v].occ;

        // Nodes are created in increasing length order.
        // Propagate occurrences to shorter suffixes.
        for (int v = m - 1; v >= 2; v--)
            cnt[tree[v].link] += cnt[v];

        return cnt;
    }

    // O(n + L), L = returned palindrome length
    string mostFrequent() const {
        if (tree.size() == 2)
            return "";

        auto cnt = frequencies();
        int best = 2;

        for (int v = 3; v < (int)tree.size(); v++) {
            if (cnt[v] > cnt[best])
                best = v;
        }

        return getPalindrome(best);
    }

    // O(n + L), L = returned palindrome length
    string longestRepeated(ll k) const {
        auto cnt = frequencies();

        int best = -1;

        for (int v = 2; v < (int)tree.size(); v++) {
            if (cnt[v] >= k &&
                (best == -1 ||
                 tree[v].len > tree[best].len)) {
                best = v;
            }
        }

        return getPalindrome(best);
    }

    // O(n + output size)
    vector<pair<string, ll>> allPalindromes() const {
        auto cnt = frequencies();

        vector<pair<string, ll>> result;

        for (int v = 2; v < (int)tree.size(); v++) {
            result.push_back({
                getPalindrome(v), cnt[v]
            });
        }

        return result;
    }

    // O(n + output size)
    void printFrequencies() const {
        auto cnt = frequencies();

        for (int v = 2; v < (int)tree.size(); v++) {
            cout << getPalindrome(v)
                 << " : " << cnt[v] << '\n';
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Eertree et;

    // Build "ababa"
    for (char c : string("ababa"))
        et.add(c);

    cout << "String: " << et.s << '\n';

    cout << "Distinct: "
         << et.distinct() << '\n';

    cout << "Total palindromes: "
         << et.totalPalindromes() << '\n';

    cout << "Longest palindrome: "
         << et.longestPalindrome() << '\n';

    cout << "Longest suffix: "
         << et.longestSuffix() << '\n';

    cout << "Palindromes ending here: "
         << et.palindromesEndingHere() << '\n';

    cout << "Minimum partitions: "
         << et.minimumPartition() << '\n';

    cout << "Most frequent: "
         << et.mostFrequent() << '\n';

    cout << "Longest repeated >= 2: "
         << et.longestRepeated(2) << '\n';

    cout << "\nFrequencies:\n";
    et.printFrequencies();

    // Remove final 'a'
    et.pop_back();

    cout << "\nAfter pop: " << et.s << '\n';
    cout << "Total: "
         << et.totalPalindromes() << '\n';

    // Append 'c'
    et.add('c');

    cout << "\nAfter adding c: " << et.s << '\n';
    cout << "Longest: "
         << et.longestPalindrome() << '\n';

    return 0;
}
