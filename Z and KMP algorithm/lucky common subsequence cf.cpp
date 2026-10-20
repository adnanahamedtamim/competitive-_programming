// find the lcs between two string a,b such that the lcs does not contains string s as a substring
#include <bits/stdc++.h>
using namespace std;

string a, b, virus;
int n, m, k;

int pi[105];
int aut[105][26];
int dp[105][105][105];

string ans;

// Build KMP prefix function
void buildPrefix() {
    for (int i = 1; i < k; i++) {
        int j = pi[i - 1];

        while (j > 0 && virus[i] != virus[j])
            j = pi[j - 1];

        if (virus[i] == virus[j])
            j++;

        pi[i] = j;
    }
}

// Build KMP automaton
void buildAutomaton() {
    for (int state = 0; state < k; state++) {
        for (int c = 0; c < 26; c++) {
            char ch = 'A' + c;
            int j = state;

            while (j > 0 && virus[j] != ch)
                j = pi[j - 1];

            if (virus[j] == ch)
                j++;

            aut[state][c] = j;
        }
    }
}


int solve(int i, int j, int state) {
    if (i == n || j == m)
        return 0;

    int &res = dp[i][j][state];

    if (res != -1)
        return res;

    
    res = max(
        solve(i + 1, j, state),
        solve(i, j + 1, state)
    );

   
    if (a[i] == b[j]) {
        int nextState = aut[state][a[i] - 'A'];


        if (nextState < k) {
            res = max(
                res,
                1 + solve(i + 1, j + 1, nextState)
            );
        }
    }

    return res;
}

// Reconstruct the answer
void buildAnswer(int i, int j, int state) {
    if (i == n || j == m)
        return;

    int best = solve(i, j, state);

    if (best == 0)
        return;

    if (a[i] == b[j]) {
        int nextState = aut[state][a[i] - 'A'];

        if (nextState < k &&
            best == 1 + solve(i + 1, j + 1, nextState)) {

            ans += a[i];
            buildAnswer(i + 1, j + 1, nextState);
            return;
        }
    }

    if (best == solve(i + 1, j, state))
        buildAnswer(i + 1, j, state);
    else
        buildAnswer(i, j + 1, state);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> a >> b >> virus;

    n = a.size();
    m = b.size();
    k = virus.size();

    buildPrefix();
    buildAutomaton();

    memset(dp, -1, sizeof(dp));

    int length = solve(0, 0, 0);

    if (length == 0) {
        cout << 0 << '\n';
        return 0;
    }

    buildAnswer(0, 0, 0);

    cout << ans << '\n';

    return 0;
}
