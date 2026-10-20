// given a text and some patterns, it will return the starting positions of the patterns in the text
#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick {
    struct Node {
        int next[26], link, exitLink;
        vector<int> out;

        Node() {
            fill(next, next + 26, -1);
            link = 0;
            exitLink = -1;
        }
    };

    vector<Node> trie;
    vector<string> patterns;

    AhoCorasick(const vector<string>& p) {
        patterns = p;
        trie.emplace_back();

        for (int i = 0; i < (int)p.size(); i++)
            insert(p[i], i);

        build();
    }

    void insert(const string& s, int id) {
        int v = 0;

        for (char ch : s) {
            int c = ch - 'a';

            if (trie[v].next[c] == -1) {
                int u = trie.size();
                trie[v].next[c] = u;
                trie.emplace_back();
            }

            v = trie[v].next[c];
        }

        trie[v].out.push_back(id);
    }

    void build() {
        queue<int> q;

        for (int c = 0; c < 26; c++) {
            int u = trie[0].next[c];

            if (u == -1) {
                trie[0].next[c] = 0;
            } else {
                trie[u].link = 0;
                q.push(u);
            }
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int f = trie[v].link;

            // Nearest suffix node containing a pattern
            if (!trie[f].out.empty())
                trie[v].exitLink = f;
            else
                trie[v].exitLink = trie[f].exitLink;

            for (int c = 0; c < 26; c++) {
                int u = trie[v].next[c];

                if (u == -1) {
                    trie[v].next[c] = trie[f].next[c];
                } else {
                    trie[u].link = trie[f].next[c];
                    q.push(u);
                }
            }
        }
    }

    vector<vector<int>> search(const string& text) {
        vector<vector<int>> ans(patterns.size());

        int state = 0;

        for (int i = 0; i < (int)text.size(); i++) {
            int c = text[i] - 'a';
            state = trie[state].next[c];

            // Report patterns ending at this state
            int v = state;

            while (v != -1) {
                for (int id : trie[v].out) {
                    int start = i - (int)patterns[id].size() + 1;
                    ans[id].push_back(start);
                }

                v = trie[v].exitLink;
            }
        }

        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> patterns(n);

    for (auto &s : patterns)
        cin >> s;

    string text;
    cin >> text;

    AhoCorasick ac(patterns);

    vector<vector<int>> ans = ac.search(text);

    for (int i = 0; i < n; i++) {
        cout << patterns[i] << ": ";

        for (int pos : ans[i])
            cout << pos << " ";

        cout << '\n';
    }

    return 0;
}
