// Given a string, your task is to generate all different strings that can be created using its characters.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    sort(s.begin(), s.end());

    vector<string> ans;

    do {
        ans.push_back(s);
    } while (next_permutation(s.begin(), s.end()));

    cout << ans.size() << '\n';

    for (const string &str : ans) {
        cout << str << '\n';
    }

    return 0;
}
