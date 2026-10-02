#include <bits/stdc++.h>
using namespace std;

/**
 * 扩展欧拉定理化简幂mod意义下的值：
 * a^b == a^b (mod cur_mod), b < phi(cur_mod)
 * a^b == a^{b mod phi(cur_mod) + phi(cur_mod)} (mod cur_mod), b >= phi(cur_mod)
 * 不要求 gcd(a, cur_mod) == 1
 * */
int cur_mod;  // 模数
int phi(int n) {
	int res = n;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			res -= res / i;
			while (n % i == 0) n /= i;
		}
	}
	if (n > 1) res -= res / n;
	return res;
}
int qpow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % cur_mod;
		a = 1LL * a * a % cur_mod, b >>= 1;
	}
	return res;
}
int exeuler(long long a, long long b) {
	a %= cur_mod;
	int p = phi(cur_mod);
	if (b < p) return qpow(a, b);
	b = b % p + p;
	return qpow(a, b);
}