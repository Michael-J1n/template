#include <bits/stdc++.h>
using namespace std;
// #define int long long
using pii = pair<int, int>;

int norm(int x, int m) { return (x % m + m) % m; }

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

// 合并方程 x = r1 (mod m1), x = r2 (mod m2)
// 返回 {r, m} 表示 x = r (mod m)，无解返回 {0, -1};
/**
 * 推导：x = r1 (mod m1) ==> r1 + m1k ==> r1 + m1k = r2 (mod m2) ==> m1k = r2 - r1 (mod m2), 当 g := gcd(m1, m2) | r2 - r1 有解
 * 若有解，(m1/g) k = (r2-r1)/g (mod m2/g) 有解 k = (r2-r1)/g * inv(m1/g) (mod m2/g)
 * 带回得 x = r1 + m1 * (r2-r1)/g * inv(m1/g)，mod 数为最小周期 lcm(m1, m2)
 * */
pii merge(int r1, int m1, int r2, int m2) {
	r1 = norm(r1, m1), r2 = norm(r2, m2);
	int x, y, d = r2 - r1;
	int g = exgcd(m1, m2, x, y);  // m1x + m2y = g，解得的 x 即为 m1/g 对于 mod m2/g 的逆元
	if (d % g != 0) return {0, -1};  // 无解

	int m = m2 / g, lcm = m1 / g * m2;
	int k = norm((d / g) % m * x, m), r = norm(r1 + m1 * k % lcm, lcm);
	return {r, lcm};
}

// 求方程组 x = ri (mod mi) 最小非负整数解
// 不需要 gcd(mi) = 1；复杂度 O(NlogM)
pii excrt(vector<int>& r, vector<int>& m) {
	assert(r.size() == m.size());
	int n = r.size(), R = r[0], M = m[0];
	for (int i = 1; i < n; ++i) {
		auto [tr, tm] = merge(R, M, r[i], m[i]);
		if (tm == -1) return {0, -1};  // 无解
		R = tr, M = tm;
	}
	return {R, M};
}