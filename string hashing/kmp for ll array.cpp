#include <bits/stdc++.h>
using namespace std;

// Function to compute the LPS (Longest Prefix Suffix) array for a long long pattern
vector<int> compute_lps(const vector<long long>& pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1]; // Fallback in the LPS array
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// Function to search pattern in text using KMP and return all starting positions
vector<int> kmp_search(const vector<long long>& text, const vector<long long>& pattern) {
    int n = text.size();
    int m = pattern.size();
    vector<int> occurrences;

    if (m == 0 || n < m) return occurrences;

    vector<int> lps = compute_lps(pattern);

    int i = 0; // Index for text
    int j = 0; // Index for pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            // Found a match! Store the starting index (i - j)
            occurrences.push_back(i - j);
            j = lps[j - 1]; // Prepare for the next potential match
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return occurrences;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example Test:
    // Text: [1, 2, 3, 1, 2, 3, 4, 1, 2, 3]
    // Pattern: [1, 2, 3]
    vector<long long> text = {1, 2, 3, 1, 2, 3, 4, 1, 2, 3};
    vector<long long> pattern = {1, 2, 3};

    vector<int> positions = kmp_search(text, pattern);

    cout << "Number of occurrences: " << positions.size() << "\n";
    cout << "Starting positions (0-based): ";
    for (int pos : positions) {
        cout << pos << " ";
    }
    cout << "\n";

    return 0;
}
