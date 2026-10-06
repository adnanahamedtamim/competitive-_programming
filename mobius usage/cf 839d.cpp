#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define vll vector<ll>
#define el '\n'

const int MAXV = 1000005;
const int MOD = 1e9 + 7;

int mu[MAXV];
bool composite[MAXV];
vector<int> primes;
ll pw2[MAXV];   

// 1. Standard Linear Sieve for Möbius function
void mobius_sieve(int n) {
    mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if (i * p > n) break;
            composite[i * p] = true;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
}

// Precompute powers of 2
void precompute_powers(int n) {
    pw2[0] = 1;
    for (int i = 1; i <= n; i++) {
        pw2[i] = (pw2[i - 1] * 2) % MOD;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    mobius_sieve(MAXV - 1);
    precompute_powers(MAXV - 1);

    ll n;
    cin >> n;

    vll ara(n);
    ll maxi=0;
    for(ll i=0;i<n;i++){
          cin >> ara[i];
          maxi=max(maxi,ara[i]);
    }

    vll cnt(maxi+1,0);
    for(auto  it : ara){
          cnt[it]++;
    }

    for(ll i=1;i<=maxi;i++){
          for(ll j=2*i;j<=maxi;j+=i){
               cnt[i]+=cnt[j];
          }
    }

    ll ans=0;

    for(ll i=2;i<=maxi;i++){ 
          ll total=0;
          
          for(ll j=i;j<=maxi;j+=i){
              if(mu[j/i]==0) continue;
              ll ache=(pw2[cnt[j]-1]*cnt[j])%MOD;
              ache=(ache*mu[j/i])%MOD; 
              total=(total+ache)%MOD;
          }
          ans=(ans+(total*i)%MOD)%MOD;
    }
    cout << ans << endl;

   


    return 0;
}
