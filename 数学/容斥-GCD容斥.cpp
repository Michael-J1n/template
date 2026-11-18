#include <bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;
const int MXN = 1e6 + 5;
int prim[MXN], cnt = 0; char vis[MXN];
int mu[MXN];
int fac[MXN + 1], ifac[MXN + 1];
int qpow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % mod;
		a = 1LL * a * a % mod, b >>= 1;
	}
	return res;
}
void init() {
	fac[0] = 1;
	for (int i = 1; i <= MXN; ++i) fac[i] = 1LL * fac[i - 1] * i % mod;
	ifac[MXN] = qpow(fac[MXN], mod - 2);
	for (int i = MXN - 1; ~i; --i) ifac[i] = 1LL * ifac[i + 1] * (i + 1) % mod;
	vis[1]=true;
	mu[1]=1;
	for (int i = 2; i < MXN; ++i) {
		if (!vis[i]) prim[++cnt] = i, mu[i] = -1;
		for (int j = 1; i * prim[j] < MXN; ++j) {
			int p = prim[j];
			vis[i * p] = 1;
			if (i % p == 0) {
				mu[i * p] = 0;
				break;
			}
			mu[i * p] = -mu[i];
		}
	}
}
int C(int n, int k) {
	if (k < 0 || k > n) return 0;
	return 1LL * fac[n] * ifac[k] % mod * ifac[n - k] % mod;
}

/**
 * GCD 容斥
 * 给定数组 a，以及整数 k，求从 a 中选出 k 个数使得其 gcd 为 t 的方案数；t∈[1,V].
 * 
 * 核心思想：count(gcd==1) = count(1|gcd) - [count(p|gcd)+count(q|gcd)+..] + [count(pq|gcd)+..] - [count(pqr|gcd)+..]
 * 
 * 定义 frq[x] 为 x 的出现次数；cnt[t] = #{x:t|x} = sum_i frq[i*t] 为 frq 的倍数前缀和。
 * 根据 gcd 上的容斥，有 ans[t] = sum_{i=1}^{t*i<=m} mu[i] * C(cnt[t*i],k).
 * 
 * 复杂度 O(V log V)
 */
vector<long long> gcdPIE(vector<int>& a, int k) {
    int n = a.size(), mx = ranges::max(a) + 1;
    vector<int> frq(mx);
    for (auto& x : a) ++frq[x];
    vector<int> cnt(mx);
    for (int i = 1; i < mx; ++i) {
        for (int j = i; j < mx; j += i) cnt[i] += frq[j];
    }
    vector<long long> res(mx);
    for (int t = 1; t < mx; ++t) {
        for (int i = 1; i * t < mx; ++i) {
            (res[t] += 1LL * mu[i] * C(cnt[t * i], k) % mod) %= mod;
            if (res[t] < 0) res[t] += mod;
        }
    }
    return res;
}