#include <bits/stdc++.h>
using namespace std;

const int MAXN = 20001;
bitset<MAXN> a[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    while (q--) {
        char op;
        int x, y;
        cin >> op >> x >> y;

        if (op == '+') {
            a[x].set(y);
        }
        else if (op == '-') {
            a[x].reset(y);
        }
        else if (op == 'v') {
            cout << (a[x] | a[y]).count() << '\n';
        }
        else if (op == '^') {
            cout << (a[x] & a[y]).count() << '\n';
        }
        else if (op == '!') {
            cout << (a[x] ^ a[y]).count() << '\n';
        }
        else if (op == '\\') {
            cout << (a[x] & ~a[y]).count() << '\n';
        }
    }

    return 0;
}
