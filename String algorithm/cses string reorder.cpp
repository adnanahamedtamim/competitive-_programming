//Your task is to reorder the characters of a string so that no two adjacent characters are the same.
//What is the lexicographically minimal such string?
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = (int)s.size();
    vector<int> cnt(26, 0);

    for (char ch : s) {
        cnt[ch - 'A']++;
    }

    int mx = *max_element(cnt.begin(), cnt.end());

    if (mx > (n + 1) / 2) {
        cout << -1 << '\n';
        return 0;
    }

    string ans;
    ans.reserve(n);
    int prev = -1;

    for (int pos = 0; pos < n; pos++) {
      
        int mx1 = -1, mx2 = -1, idx = -1;

        for (int c = 0; c < 26; c++) {
            if (cnt[c] > mx1) {
                mx2 = mx1;
                mx1 = cnt[c];
                idx = c;
            } else if (cnt[c] > mx2) {
                mx2 = cnt[c];
            }
        }

        int rem = n - pos - 1;
        bool chosen = false;

     
        for (int c = 0; c < 26; c++) {
            if (cnt[c] == 0 || c == prev) continue;

            int otherMax = (c == idx ? mx2 : mx1);
            int after = cnt[c] - 1;

            // Can the remaining suffix be arranged?
            if (after <= rem / 2 &&
                otherMax <= (rem + 1) / 2) {

                ans += char('A' + c);
                cnt[c]--;
                prev = c;
                chosen = true;
                break;
            }
        }


        if (!chosen) {
            cout << -1 << '\n';
            return 0;
        }
    }

    cout << ans << '\n';

    return 0;
}
