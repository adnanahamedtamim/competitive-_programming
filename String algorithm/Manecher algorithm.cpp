
#include <bits/stdc++.h>
using namespace std;

struct Manacher {
    vector<int> d1, d2;

    Manacher(const string& s) {
        int n = s.size();
        d1.resize(n);
        d2.resize(n);

        // Odd-length palindromes
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = 1;

            if (i <= r)
                k = min(d1[l + r - i], r - i + 1);

            while (i - k >= 0 && i + k < n &&
                   s[i - k] == s[i + k]) {
                k++;
            }

            d1[i] = k;

            if (i + k - 1 > r) {
                l = i - k + 1;
                r = i + k - 1;
            }
        }

        // Even-length palindromes
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = 0;

            if (i <= r)
                k = min(d2[l + r - i + 1], r - i + 1);

            while (i - k - 1 >= 0 && i + k < n &&
                   s[i - k - 1] == s[i + k]) {
                k++;
            }

            d2[i] = k;

            if (i + k - 1 > r) {
                l = i - k;
                r = i + k - 1;
            }
        }
    }

    // Check whether s[l..r] is a palindrome
    bool isPalindrome(int l, int r) {
        int len = r - l + 1;

        if (len % 2 == 1) {
            int mid = (l + r) / 2;
            return d1[mid] >= len / 2 + 1;
        } else {
            int mid = (l + r + 1) / 2;
            return d2[mid] >= len / 2;
        }
    }

    // Total number of palindromic substrings
    long long countPalindromes() {
        long long ans = 0;

        for (int x : d1) ans += x;
        for (int x : d2) ans += x;

        return ans;
    }

    // Longest palindromic substring
    string longestPalindrome(const string& s) {
        int start = 0, best = 0;

        for (int i = 0; i < (int)s.size(); i++) {
            int len1 = 2 * d1[i] - 1;
            int len2 = 2 * d2[i];

            if (len1 > best) {
                best = len1;
                start = i - d1[i] + 1;
            }

            if (len2 > best) {
                best = len2;
                start = i - d2[i];
            }
        }

        return s.substr(start, best);
    }
};

int main() {
    string s;
    cin >> s;

    Manacher man(s);

    cout << "Odd radii: ";
    for (int x : man.d1) cout << x << " ";
    cout << '\n';

    cout << "Even radii: ";
    for (int x : man.d2) cout << x << " ";
    cout << '\n';

    cout << "Total palindromes: "
         << man.countPalindromes() << '\n';

    cout << "Longest palindrome: "
         << man.longestPalindrome(s) << '\n';

    cout << "Is s[1..3] palindrome? "
         << (man.isPalindrome(1, 3) ? "YES" : "NO")
         << '\n';
}
