#include <bits/stdc++.h>
using namespace std;

/**
 * 卢卡斯定理递归求组合数，适合 n, k 大数，mod 数小且为素数
 * 
 * 复杂度：预处理 O(MOD)，单次查询 O(logn)
 */
const int MOD = 1e6 + 3;
int fac[MOD], ifac[MOD];
int qpow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % MOD;
		a = 1LL * a * a % MOD, b >>= 1;
	}
	return res;
}
void init() {
	fac[0] = 1;
	for (int i = 1; i <= MOD; ++i) fac[i] = 1LL * fac[i - 1] * i % MOD;
	ifac[MOD - 1] = qpow(fac[MOD - 1], MOD - 2);
	for (int i = MOD - 2; ~i; --i) ifac[i] = 1LL * ifac[i + 1] * (i + 1) % MOD;
}
int _C(int n, int k) {
	if (k < 0 || k > n) return 0;
	return 1LL * fac[n] * ifac[k] % MOD * ifac[n - k] % MOD;
}
int C(long long n, long long k) {
	if (k < 0 || k > n) return 0;
	if (k == 0) return 1;
	return 1LL * _C(n % MOD, k % MOD) % MOD * C(n / MOD, k / MOD) % MOD;
}