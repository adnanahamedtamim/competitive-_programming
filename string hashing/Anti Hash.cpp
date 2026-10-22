
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define vll vector<ll>
#define pb push_back

mt19937 rng(
    chrono::steady_clock::now().time_since_epoch().count()
);

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int Q;
    cin >> Q;

    while (Q--) {
        int n;
        ll B, M;
        cin >> n >> B >> M;

        string s;
        cin >> s;

        // pw[i] = B^(n-1-i) % M
        vll pw(n);
        pw[n - 1] = 1;

        for (int i = n - 2; i >= 0; i--) {
            pw[i] = (pw[i + 1] * B) % M;
        }

        int mid = n / 2;
        const int LIMIT = 200000;

        string answer;

        while (answer.empty()) {
            string cur = s;

            // hash difference -> number of left operations
            unordered_map<ll, int> mp;
            mp.max_load_factor(0.7);
            mp.reserve(LIMIT * 2);

            vector<pair<int, char>> leftOps;
            vector<pair<int, char>> rightOps;

            mp[0] = 0;

            ll diff = 0;

            // Generate left-half variations
            for (int i = 1; i <= LIMIT; i++) {
                int pos = rng() % mid;

                char old = cur[pos];
                char now = 'a' +
                    (old - 'a' + 1 + rng() % 25) % 26;

                cur[pos] = now;
                leftOps.pb({pos, now});

                ll change =
                    (ll)(now - old) * pw[pos] % M;

                diff = (diff + change + M) % M;

                if (!mp.count(diff)) {
                    mp[diff] = i;
                }

                // A nonempty left-only change with zero
                // hash difference already solves the problem.
                if (diff == 0) {
                    answer = s;

                    for (int j = 0; j < i; j++) {
                        auto [p, c] = leftOps[j];
                        answer[p] = c;
                    }

                    if (answer == s) answer.clear();
                    if (!answer.empty()) break;
                }
            }

            if (!answer.empty()) break;

            // Generate right-half variations
            diff = 0;

            for (int i = 1; i <= LIMIT; i++) {
                int pos = mid + rng() % (n - mid);

                char old = cur[pos];
                char now = 'a' +
                    (old - 'a' + 1 + rng() % 25) % 26;

                cur[pos] = now;
                rightOps.pb({pos, now});

                ll change =
                    (ll)(now - old) * pw[pos] % M;

                diff = (diff + change + M) % M;

                ll need = (M - diff) % M;

                auto it = mp.find(need);

                if (it != mp.end()) {
                    answer = s;

                    // Reconstruct left changes
                    int steps = it->second;

                    for (int j = 0; j < steps; j++) {
                        auto [p, c] = leftOps[j];
                        answer[p] = c;
                    }

                    // Reconstruct right changes
                    for (auto [p, c] : rightOps) {
                        answer[p] = c;
                    }

                    break;
                }
            }
        }

        cout << answer << '\n';
    }

    return 0;
}
