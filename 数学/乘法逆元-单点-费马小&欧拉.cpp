#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/**
 * 当 gcd(a, m) == 1 时，满足 ab == 1 (mod m) 的 b 为 a 在 mod m 下的乘法逆元
 * 当 gcd(a, m) != 1 时，一定不存在乘法逆元
 * */
ll qpow(ll a, ll b, int p) {
	a %= p;
	ll res = 1;
	while (b) {
		if (b & 1) res = res * a % p;
		a = a * a % p, b >>= 1;
	}
	return res;
}
ll phi(ll n) {
	ll res = n;
	for (ll i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			res -= res / i;
			while (n % i == 0) n /= i;
		}
	}
	if (n > 1) res -= res / n;
	return res;
}
// 当模数为质数，由 FLT a^{p-1} == 1 (mod p) ==> a^{-1} = a^{p-2}
// 求满足 ax = 1 (mod p) 的 x:
ll inv(ll a, int p) {
	return qpow(a, p - 2, p);
}
// 当模数不为质数，由欧拉定理，a^{phim} == 1 (mod m) ==> a^{-1} = a^{phim-1}
ll inv2(ll a, int m) {
	if (gcd(a, m) != 1) return 0;
	ll pm = phi(m);
	return qpow(a, pm - 1, m);
}