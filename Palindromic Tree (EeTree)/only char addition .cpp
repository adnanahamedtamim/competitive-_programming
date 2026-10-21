
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Eertree {
    struct Node {
        int len, link;
        int next[26] = {};

        int num = 0;         // Number of palindromic suffixes
        int firstEnd = -1;   // First ending position
        ll occ = 0;          // Raw occurrence count

        Node(int l = 0) : len(l), link(0) {}
    };

    vector<Node> tree;
    string s;

    int last = 1;
    int best = -1;
    ll total = 0;

    // O(1)
    Eertree() {
        tree.emplace_back(-1); // Root -1
        tree.emplace_back(0);  // Root 0

        tree[0].link = 0;
        tree[1].link = 0;
    }

    // O(n) worst case per call;
    // amortized O(1) during append-only construction
    int getLink(int v, int pos) {
        while (true) {
            int left = pos - tree[v].len - 1;

            if (left >= 0 && s[left] == s[pos])
                return v;

            v = tree[v].link;
        }
    }

    // O(1) amortized; O(n) worst case
    void add(char ch) {
        s += ch;

        int pos = (int)s.size() - 1;
        int c = ch - 'a';

        int cur = getLink(last, pos);

        if (tree[cur].next[c] == 0) {
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

        // Count palindrome occurrences
        tree[last].occ++;
        total += tree[last].num;

        // Track longest palindrome
        if (best == -1 ||
            tree[last].len > tree[best].len) {
            best = last;
        }
    }

    // O(n) total to build a string of length n
    void build(const string& str) {
        for (char c : str)
            add(c);
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
        return best == -1 ? 0 : tree[best].len;
    }

    // O(L), L = returned palindrome length
    string longestPalindrome() const {
        return getPalindrome(best);
    }

    // O(1)
    int longestSuffixLength() const {
        return tree[last].len;
    }

    // O(L)
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

    // O(L)
    string getPalindrome(int v) const {
        if (v < 2 || v >= (int)tree.size())
            return "";

        int len = tree[v].len;
        int end = tree[v].firstEnd;

        return s.substr(end - len + 1, len);
    }

    // O(n)
    // Returns frequency of every palindrome node
    vector<ll> frequencies() const {
        int m = (int)tree.size();
        vector<ll> cnt(m, 0);

        for (int v = 2; v < m; v++)
            cnt[v] = tree[v].occ;

        for (int v = m - 1; v >= 2; v--)
            cnt[tree[v].link] += cnt[v];

        return cnt;
    }

    // O(n + L)
    string mostFrequent() const {
        if (best == -1)
            return "";

        auto cnt = frequencies();
        int ans = 2;

        for (int v = 3; v < (int)tree.size(); v++) {
            if (cnt[v] > cnt[ans])
                ans = v;
        }

        return getPalindrome(ans);
    }

    // O(n + L)
    string longestRepeated(ll k) const {
        auto cnt = frequencies();
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

    // O(n + total length of returned strings)
    vector<pair<string, ll>> allPalindromes() const {
        auto cnt = frequencies();

        vector<pair<string, ll>> result;

        for (int v = 2; v < (int)tree.size(); v++) {
            result.push_back({
                getPalindrome(v),
                cnt[v]
            });
        }

        return result;
    }

    // O(n + total printed characters)
    void printFrequencies() const {
        auto cnt = frequencies();

        for (int v = 2; v < (int)tree.size(); v++) {
            cout << getPalindrome(v)
                 << " : " << cnt[v] << '\n';
        }
    }

    // O(n^2) worst case
    // Minimum number of palindromic substrings
    // needed to partition the current string
    int minimumPartition() const {
        int n = (int)s.size();
        const int INF = 1e9;

        vector<int> dp(n + 1, INF);
        dp[0] = 0;

        // Longest palindrome suffix node for each prefix
        // Reconstruct it using a temporary Eertree
        Eertree temp;

        for (int i = 1; i <= n; i++) {
            temp.add(s[i - 1]);

            for (int v = temp.last;
                 temp.tree[v].len > 0;
                 v = temp.tree[v].link) {

                int len = temp.tree[v].len;

                dp[i] = min(
                    dp[i],
                    dp[i - len] + 1
                );
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

    Eertree et;
    et.build(s);

    cout << "Distinct palindromes: "
         << et.distinct() << '\n';

    cout << "Total palindrome occurrences: "
         << et.totalPalindromes() << '\n';

    cout << "Longest palindrome: "
         << et.longestPalindrome() << '\n';

    cout << "Longest palindrome length: "
         << et.longestPalindromeLength() << '\n';

    cout << "Longest palindromic suffix: "
         << et.longestSuffix() << '\n';

    cout << "Palindromes ending at last position: "
         << et.palindromesEndingHere() << '\n';

    cout << "Most frequent palindrome: "
         << et.mostFrequent() << '\n';

    cout << "Longest palindrome occurring >= 2 times: "
         << et.longestRepeated(2) << '\n';

    cout << "Minimum palindrome partitions: "
         << et.minimumPartition() << '\n';

    cout << "\nAll palindrome frequencies:\n";
    et.printFrequencies();

    return 0;
}
