#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

struct DynHash {
    static const ull MOD = (1ULL << 61) - 1;

    static inline ull mul(ull a, ull b) {
        __uint128_t t = (__uint128_t)a * b;
        ull r = (ull)(t & MOD) + (ull)(t >> 61);
        return r >= MOD ? r - MOD : r;
    }
    static inline ull add(ull a, ull b) { ull r = a + b; return r >= MOD ? r - MOD : r; }
    static inline ull sub(ull a, ull b) { return a >= b ? a - b : a + MOD - b; }

    int n = 0;                    // current length
    ull B1, B2;                   // two random bases
    vector<ull> p1, p2;           // powers of the bases
    vector<ull> f1, f2;           // forward prefix hashes:  f[i] = f[i-1]*B + c
    vector<ull> r1, r2;           // reverse prefix hashes:  r[i] = r[i-1] + c*B^(i-1)

    DynHash() {}
    DynHash(int maxLen) { init(maxLen); }

    void init(int maxLen) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        B1 = rng() % (MOD - 1000) + 500;
        B2 = rng() % (MOD - 1000) + 500;
        p1.assign(maxLen + 2, 1); p2.assign(maxLen + 2, 1);
        f1.assign(maxLen + 2, 0); f2.assign(maxLen + 2, 0);
        r1.assign(maxLen + 2, 0); r2.assign(maxLen + 2, 0);
        for (int i = 1; i < maxLen + 2; i++) {
            p1[i] = mul(p1[i-1], B1);
            p2[i] = mul(p2[i-1], B2);
        }
        n = 0;
    }

    // append character c.  O(1)
    void push(char ch) {
        ull c = (unsigned char)ch + 1;               // +1 so that value 0 never appears
        f1[n+1] = add(mul(f1[n], B1), c);
        f2[n+1] = add(mul(f2[n], B2), c);
        r1[n+1] = add(r1[n], mul(c, p1[n]));
        r2[n+1] = add(r2[n], mul(c, p2[n]));
        n++;
    }

    // remove last character.  O(1)
    void pop() { n--; }

    // clear everything.  O(1)
    void clear() { n = 0; }

    // hash of whole current string (forward).  O(1)
    pair<ull,ull> get() const { return {f1[n], f2[n]}; }

    // forward hash of substring [l, r)  (0-indexed, half-open).  O(1)
    pair<ull,ull> sub(int l, int r) const {
        return { sub(f1[r], mul(f1[l], p1[r - l])),
                 sub(f2[r], mul(f2[l], p2[r - l])) };
    }

    // is the whole current string a palindrome?  O(1)
    bool isPal() const { return f1[n] == r1[n] && f2[n] == r2[n]; }

    // is substring [l, r) a palindrome?  O(1)
    bool isPal(int l, int r) const {
        // forward hash of [l,r)
        ull a1 = sub(f1[r], mul(f1[l], p1[r - l]));
        ull a2 = sub(f2[r], mul(f2[l], p2[r - l]));
        // reverse-direction hash of [l,r) is (r[r]-r[l]) scaled by B^l
        ull b1 = sub(r1[r], r1[l]);
        ull b2 = sub(r2[r], r2[l]);
        // forward hash of reversed string = (r[r]-r[l]) / B^l  ->  compare a*B^l == b
        return mul(a1, p1[l]) == b1 && mul(a2, p2[l]) == b2;
    }

    // are substrings [l1,r1) and [l2,r2) equal?  O(1)
    bool equalSub(int l1, int r1_, int l2, int r2_) const {
        return (r1_ - l1 == r2_ - l2) && sub(l1, r1_) == sub(l2, r2_);
    }
};
int main(){
    DynHash h(n);        // once per test, pass the max possible length
    h.push('a');
    h.push('b');
    h.push('a');
    h.isPal();           // true  ("aba")
    h.pop();
    h.isPal();           // false ("ab")
    h.get();             // hash pair of current string
    h.sub(0, 1);         // hash of substring [0,1)
    h.isPal(0, 3);       // palindrome check on any substring of the current string

}
