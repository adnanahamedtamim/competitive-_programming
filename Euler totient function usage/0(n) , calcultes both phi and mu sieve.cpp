const int MAXN = 2000005;

int phi[MAXN], mu[MAXN];
bool composite[MAXN];
vector<int> primes;

void linear_sieve(int n) {
    phi[1] = mu[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (!composite[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
            mu[i] = -1;
        }

        for (int p : primes) {
            if (1LL * i * p > n) break;

            composite[i * p] = true;

            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                mu[i * p] = 0;
                break;
            }

            phi[i * p] = phi[i] * (p - 1);
            mu[i * p] = -mu[i];
        }
    }
}
