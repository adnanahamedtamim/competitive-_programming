#include<bits/stdc++.h>
#define ull unsigned long long
using namespace std;
mt19937_64 rng(time(0));
const int N = 500011;
ull hashcode[N], h[N];
int a[N], freq[N];
queue<int> pos[N];
unordered_map<ull, int> cnt;
int n; 
signed main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cin >> n;
	for (int i = 1; i <= n; ++i)
		hashcode[i] = 1ull * rng() * rng();

	ull code = 0, ans = 0;
	int cur = 0; cnt[0]++;
	for (int i = 1, x; i <= n; ++i) {
		cin >> x;
		code -= 1ull * freq[x] * hashcode[x];
		freq[x] = (freq[x] + 1) % 3;
		code += 1ull * freq[x] * hashcode[x];

		h[i] = code;
		pos[x].push(i);
		if (pos[x].size() == 4) {
			while (cur < pos[x].front()) 
				cnt[h[cur++]]--;
			pos[x].pop();
		}
		ans += cnt[code]++;
	}

	cout << ans;
	return 0;
}
