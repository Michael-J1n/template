/**
 * CRT 解方程组 x = ri (mod mi)，假设 m1,m2,...,mn 两两互质
 * 则其对于 mod M = m1m2..mn 有唯一解：
 * x = ∑r[i]c[i]inv[c[i]] (mod M)，其中 c[i]=M/m[i], inv[ci] 为 ci 对于 mod mi 的逆元
 * 复杂度 O(NlogC)
 * */

#include <bits/stdc++.h>
using namespace std;
// #define int long long

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

// 求方程组 x = ri (mod mi) 最小非负整数解
// 前提：gcd(mi) = 1；复杂度 O(NlogM)
int solve(vector<int>& r, vector<int>& m) {
	assert(r.size() == m.size());
	int n = r.size(), M = 1;
	for (int i = 0; i < n; ++i) M = M * m[i];

	int res = 0;
	for (int i = 0; i < n; ++i) {
		int ci = M / m[i], ivc = inv(ci % m[i], m[i]);
		(res += r[i] * ci * ivc) %= M;
	}
	return res;
}
