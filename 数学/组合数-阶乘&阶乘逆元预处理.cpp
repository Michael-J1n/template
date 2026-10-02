#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

/**
 * O(N) 预处理组合数
 */
const int mod = 1e9 + 7;
const int MXN = 1e6 + 5;
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
}
int C(int n, int k) {
	if (k < 0 || k > n) return 0;
	return 1LL * fac[n] * ifac[k] % mod * ifac[n - k] % mod;
}