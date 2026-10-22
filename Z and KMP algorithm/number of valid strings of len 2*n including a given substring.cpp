// find the lcs between two string a,b such that the lcs does not contains string s as a substring
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define MOD 1000000007
string s;
ll n,m;

int pi[205];
int aut[205][2];
vector<vector<vector<ll>>> dp;

// Build KMP prefix function
void buildPrefix() {
    for (int i = 1; i < m; i++) {
        int j = pi[i - 1];

        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];

        if (s[i] == s[j])
            j++;

        pi[i] = j;
    }
}

// Build KMP automaton
void buildAutomaton() {
    for (int state = 0; state < m; state++) {
        for (int c = 0; c < 2; c++) {
            char ch = (c==0 ? '(' : ')');
            int j = state;

            while (j > 0 && s[j] != ch)
                j = pi[j - 1];

            if (s[j] == ch)
                j++;

            aut[state][c] = j;
        }
    }
    aut[m][0]=aut[m][1]=m;
}



ll func(ll i,ll j,ll bal){

    if(bal>n) return 0;
 
   if(i==2*n){
      return bal==0 && j==m;
   }

    if(dp[i][j][bal]!=-1) return dp[i][j][bal];

    ll ans=0;
    ll next=aut[j][0];
   
    ans=func(i+1,next,bal+1); 
    

    if(bal>0){
        ll next=aut[j][1];
        
        ans=(ans+func(i+1,next,bal-1))%MOD; 
    
    } 
  
    return dp[i][j][bal]=ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    cin >> s;
    m=s.size();

    dp.resize(2*n+5, vector<vector<ll>>(m + 5, vector<ll>(n + 5, -1)));
    
    
    buildPrefix();
    buildAutomaton();



    cout << func(0,0,0);

    // cout << ans << endl;
    

    return 0;
}
