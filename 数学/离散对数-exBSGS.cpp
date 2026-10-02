#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/**
 * 离散对数问题 a^x = b (mod p) 在 gcd(a, p) != 1 时的做法：
 * 每次剥洋葱：计算 gcd(a, p)=d，若 d !| b 无解，否则 => a/d * a^{x-1} = b/d (mod p/d)
 * 若现在 gcd(a, p/d)=d2=1 则推出使用 bsgs，否则继续 a/d * a/d2 * a^{x-2} = b/dd2 (mod p/dd2)
 * 循环直到 gcd = 1 后推出使用 Bsgs
 * */

int qpow(int a, int b, int p) {
	ll res = 1;
	while (b) {
		if (b & 1) res = res * a % p;
		a = 1LL * a * a % p, b >>= 1;
	}
	return (int)res;
}

int exgcd(int a, int b, int& x, int& y) {
	if (b == 0) {
		x = 1, y = 0;
		return a;
	}
	int x1, y1;
	int g = exgcd(b, a % b, x1, y1);
	x = y1, y = x1 - (a / b) * y1;
	return g;
}

// 求 ax = 1 (mod m) 的 a 的逆元 x = inv[a]
int inv(int a, int m) {
	int x, y;
	int g = exgcd(a, m, x, y);
	if (g != 1) return INT_MAX;  // 不存在逆元
	x = (x % m + m) % m;
	return x;
}

// 求解满足 a^x = b (mod p) 的最小非负整数解 x, 前提 gcd(a, p) == 1
// 复杂度 O(sqrtp)
int bsgs(int a, int b, int p) {
	if (p == 1) return 0;
	a = (a % p + p) % p, b = (b % p + p) % p;
	if (b == 1) return 0;
	if (b == 0 || gcd(a, p) != 1) return -1;
 	int m = (int)(sqrt(p) + 1);

 	unordered_map<int, int> mp;
 	int k;
 	for (int j = 0; j < m; ++j) {
 		if (j == 0) k = b;
 		else k = 1LL * k * a % p;

 		mp[k] = j;  // 更新最大 j 这样 x = im-j 查到就是最小解
 	}
 	int A = qpow(a, m, p);
 	for (int i = 1; i <= m; ++i) {
 		if (i == 1) k = A;
 		else k = 1LL * k * A % p;

 		if (mp.count(k)) return (int)(1LL * i * m - mp[k]);
 	}
 	return -1;
}

// 求解满足 a^x = b (mod p) 的最小非负整数解 x
// 复杂度 O(sqrtp)
int exbsgs(int a, int b, int p) {
	if (p == 1) return 0;
	a = (a % p + p) % p, b = (b % p + p) % p;
	if (b == 1) return 0;

	int d, k = 0, A = 1;
	while (true) {
		d = gcd(a, p);
		if (d == 1) break;
		if (b % d != 0) return -1;  // 无解
		a %= p, ++k, b /= d, p /= d, A = (int)(1LL * A * a / d % p);
		if (A == b) return k;  // 特判提早返回
	}
	int v = (int)(1LL * b * inv(A, p) % p);
	int y = bsgs(a, v, p);
	if (y == -1) return -1;  // 无解
	return k + y;
}