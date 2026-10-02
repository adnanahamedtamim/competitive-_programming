#include<bits/stdc++.h>
#include <unordered_map>
#include <chrono>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;

typedef long long ll;
typedef long double ld;


#define pb push_back
#define all(x) x.begin(), x.end()
// #define endl '\n'
#define YES cout<<"YES"<<endl
#define NO cout<<"NO"<<endl
#define Yes cout<<"Yes"<<endl
#define No cout<<"No"<<endl
#define endl "\n"
 
// #define isSet(x, i) ((x>>i)&1)
// #define setbit(x, i) (x | (1LL<<i))
// #define resetbit(x, i) (x & (~(1LL << i)))
// #define toggleBit(x, i) ((x) ^ (1LL << (i)))
// #define clz(x) __builtin_clzll(x)
#define ctz(x) __builtin_ctzll(x)
#define csb(x) __builtin_popcountll(x)
// #define msb(x) (ll)((x) ? (63 - __builtin_clzll((ll)(x))) : -1)
// #define lsb(x) (ll)((x) ? (__builtin_ctzll((ll)(x))) : -1)
typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<vector<ll>> vvll;
typedef vector<vector<int>> vvi;
/*======================
       ordered set for finding kth element in O(logn)
       usage :: find_by_order(pos),,, return the ref to pos'th element(0 based) in the set
               s.order_of_key(x);;; it returns the pos of x(0 based)-> number of elements smaller than x
=======================*/
// using namespace __gnu_pbds;

// typedef tree<
// int,
// null_type,
// less<int>,
// rb_tree_tag,
// tree_order_statistics_node_update> ordered_set;

/*======================
        CONSTANTS
=======================*/

const ll MOD = 1000000007;
// const ld PI = acos(-1.0L);
// const ld EPS = 1e-12;

// const int MAXN = 2e5 + 5;
// const int MAXSIEVE = 1e6 + 5;

// /*======================
//         FAST HASH
// // =======================*/

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }

    template <typename T1, typename T2>
    size_t operator()(const pair<T1,T2>& p) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();

        return splitmix64(hash<T1>()(p.first)) ^
               splitmix64(hash<T2>()(p.second) + FIXED_RANDOM);
    }

    size_t operator()(const vector<ll>& v) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();

        uint64_t h = v.size();
        for(auto &x : v)
            h ^= splitmix64(x + FIXED_RANDOM);

        return h;
    }
    size_t operator()(const string &s) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(hash<string>()(s) + FIXED_RANDOM);
    }
};

template <class K,class V>
using umap = unordered_map<K,V,custom_hash>;

/*======================
    MODULAR ARITHMETIC
=======================*/

// ll binexp(ll a,ll b){
//     // if(a==0 || b==0) return 1;
//     ll res = 1;
//     a %= MOD;
//     while(b){
//         if(b&1) res = (res*a)%MOD;
//         a = (a*a)%MOD;
//         b >>= 1;
//     }
//     return res;
// }

// ll modinv(ll a){
//     return binexp(a,MOD-2);
// }

/*======================
    COMBINATORICS
=======================*/

// ll fact[MAXN];
// ll invfact[MAXN];
// // vector<ll> primes;

// void build_factorial(){
//     fact[0] = 1;
//     for(int i=1;i<MAXN;i++)
//         fact[i] = (fact[i-1]*i)%MOD;
//     invfact[MAXN-1] = modinv(fact[MAXN-1]);
//     for(int i=MAXN-2;i>=0;i--)
//         invfact[i] = (invfact[i+1]*(i+1))%MOD;
// }

// ll ncr(ll n,ll r){
//     if(r<0 || r>n) return 0;
//     return fact[n]*invfact[r]%MOD*invfact[n-r]%MOD;
// }

// /*======================
//     NUMBER THEORY
// =======================*/

ll gcd_ll(ll a,ll b){
    if(b==0) return a;
    return gcd_ll(b,a%b);
}

// ll lcm_ll(ll a,ll b){
//     return (a/gcd_ll(a,b))*b;
// }

// ll lcm(ll a, ll b, ll m){
//     if(a==0 || b==0) return m+1;

//     ll g = gcd_ll(a,b);
//     ll res = a/g;

//     if(res > m/b) return m+1;

//     return res*b;
// }

ll lcmwithoutmod(ll a,ll b){
    if(a==0) return b;
    if(b==0) return a;
    
    ll g = gcd_ll(a,b);
    ll res = a/g;
    
    return res*b;
}

// /*======================
//     SIEVE + FACTORS
// =======================*/


const int N = 1e6 + 5;

vector<int> spf(N);

void build_spf() {
    for (int i = 1; i < N; i++) spf[i] = i;

    for (int i = 2; i * i < N; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j < N; j += i) {
                if (spf[j] == j)
                    spf[j] = i;
            }
        }
    }
}

vll getFactors(ll p)
{
    vll factors;

    while(p>1)
    {
        ll x=spf[p];
        factors.pb(x);

        while(p%x==0)
            p/=x;
    }

    return factors;
}



// /*======================
//         INPUT
// =======================*/

void get_vector0based(vector<ll>& v){
    for(ll i=0;i<v.size();i++)
        cin>>v[i];
}

void get_vector1based(vector<ll>& v){
    for(ll i=1;i<v.size();i++)
        cin>>v[i];
}

// /*======================
//     BIT TRICK UTILITY
// =======================*/

// long long sum_floor_log2(long long n){
//     long long ans=0;
//     long long power=1;
//     long long k=0;
//     while((power<<1LL)<=n){
//         ans=(ans+k%MOD*power%MOD)%MOD;
//         power<<=1LL;
//         k++;
//     }
//     long long cnt=(n-power+1)%MOD;
//     ans=(ans+k%MOD*cnt%MOD)%MOD;
//     return ans;
// }

// /*======================
//     3D GEOMETRY
// =======================*/

// struct vec{
//     ld x,y,z;
// };

// vec operator+(vec a,vec b){ return {a.x+b.x,a.y+b.y,a.z+b.z}; }
// vec operator-(vec a,vec b){ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
// vec operator/(vec a,ld d){ return {a.x/d,a.y/d,a.z/d}; }
// ld dot(vec a,vec b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
// ld norm(vec a){ return sqrt(dot(a,a)); }

// /*======================
//     random hash
// =======================*/
// // mersenne twister engine, generates a 64-bit random number
// mt19937_64 rng(
//     chrono::steady_clock::now().time_since_epoch().count()
// );
 
// /*======================
//     string
// =======================*/
// bool small(string a,string b){
//    ll n=a.size();
//    ll m=b.size();
//    ll i=0; 
//    ll j=0;
//    while(i<n && j<m){
//        if(a[i]<b[j]) return true;
//        if(a[i]>b[j]) return false;
//        i++;
//        j++;
//    }
//    if(i==n && j==m) return true;
//    if(j<m) return true;
//    return false;
// }


// struct Fraction {
//     ll num, den;

//     Fraction(ll n = 0, ll d = 1) {
//         num = n;
//         den = d;
//         simplify();
//     }

//     void simplify() {
//         if (den < 0) {
//             num = -num;
//             den = -den;
//         }

//         ll g = gcd(abs(num), abs(den));
//         num /= g;
//         den /= g;
//     }

//     Fraction operator+(const Fraction &other) const {
//         return Fraction(
//             num * other.den + other.num * den,
//             den * other.den
//         );
//     }

//     Fraction operator-(const Fraction &other) const {
//         return Fraction(
//             num * other.den - other.num * den,
//             den * other.den
//         );
//     }

//     Fraction operator*(const Fraction &other) const {
//         return Fraction(
//             num * other.num,
//             den * other.den
//         );
//     }

//     Fraction operator/(const Fraction &other) const {
//         return Fraction(
//             num * other.den,
//             den * other.num
//         );
//     }
// };

ll func(vll& primes,ll x,ll mid){

ll n=primes.size();
ll ans1=0,ans2=0;
for(ll i=1;i<(1LL<<(n));i++){
      ll sign=csb(i);
      sign=(sign%2==1 ? 1 : -1);
      
      ll mul=1;
      for(ll j=0;j<n;j++){
          if((i&(1LL<<j))>0){
              mul*=primes[j];
          }
      }
     ans1+=(sign*(x/mul));
     ans2+=(sign*(mid/mul));
}

ll cnt=(mid-ans2)-(x-ans1);

return cnt;
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

   ll la,ra,ta,lb,rb,tb;
   cin >> la >> ra >> ta;
   cin >> lb >> rb >> tb;

   ll lena=ra-la+1;
   ll lenb=rb-lb+1;

  
  ll ans=0;
  ll t=gcd(ta,tb);
  ll d=((lb-la)%t+t)%t;


  auto calc = [&](ll x) { 
    ll l=max(0LL,x);
    ll r=min(lena,lenb+x);

    ans=max(ans,r-l);
  };



  calc(d);
  calc(d-t);

  cout << ans << endl;



    return 0;
}
