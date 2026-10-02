#include <bits/stdc++.h>
using namespace std;
const int N = 1e7 + 5;  // 按照题目调整

int prim[N];
int cnt = 0;
int vis[N];
int phi[N];
long long phiS[N];  // phiS[N] 为 phi[i] 在 i=1 ~ i=N-1 的前缀和
int mu[N];
long long muS[N];

void init() {
	vis[1] = true, phi[1] = 1, mu[1] = 1;
	for (int i = 2; i < N; ++i) {
		if (!vis[i]) prim[++cnt] = i, phi[i] = i - 1, mu[i] = -1;
		for (int j = 1; i * prim[j] < N; ++j) {
			int p = prim[j];
			vis[i * p] = true;
			if (i % p == 0) {
				phi[i * p] = phi[i] * p, mu[i * p] = 0;
				break;
			}
			phi[i * p] = phi[i] * (p - 1), mu[i * p] = -mu[i];
		}
	}

    muS[0] = 0, phiS[0] = 0;
    for (int i = 1; i < N; ++i) muS[i] = muS[i - 1] + mu[i], phiS[i] = phiS[i - 1] + phi[i];
}