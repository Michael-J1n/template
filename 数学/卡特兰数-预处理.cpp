/**
 * 卡特兰数按照计算方法用两种预处理：
 * 1. 适合小n的N^2预处理，直接使用递推2
 * 2. 使用组合公式计算，需要预处理阶乘与阶乘逆元
 * */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 2e5 + 5;
const int mod = 1e9 + 7;
int fac[N], ifac[N];

int qpow(int a, int b) {
	a %= mod;
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % mod;
		a = 1LL * a * a % mod, b >>= 1;
	}
	return res;
}

void init() {
	fac[0] = 1;
	for (int i = 1; i < N; ++i) fac[i] = 1LL * fac[i - 1] * i % mod;
	ifac[N - 1] = qpow(fac[N - 1], mod - 2);
	for (int i = N - 2; ~i; --i) ifac[i] = 1LL * ifac[i + 1] * (i + 1) % mod;
}

int C(int n, int k) {
	if (k < 0 || k > n) return 0;
	return 1LL * fac[n] * ifac[k] % mod * ifac[n - k] % mod;
}

// 求第 n 个卡特兰数 C[n]
int Catlan(int n) {
	return (1LL * C(2 * n, n) - C(2 * n, n + 1) + mod) % mod;
}