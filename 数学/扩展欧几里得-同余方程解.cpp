#include <bits/stdc++.h>
using namespace std;

/**
 * 求 ax = b (mod m) 的解
 * <=> 存在 y 使得 ax + my = b，有解当且仅当 gcd(a, m) | b
 * 使用扩欧求解 ax + my = gcd(a,m)，得到特解 x0, y0 => a(x0*b/g) + m(y0*b/g) = b
 * => x = x0 * b/g (mod m/g) = x0 * b/g (mod m/g)
 * 
 * 注意 x0 * b/g 是 mod m/g 的特解，在 mod m 下一共有 g 个解
 * 其他解可以通过 + k*(m/g) 得到
 * */
long long exgcd(long long a, long long b, long long& x, long long& y) {
	if (b == 0) {
		x = 1, y = 0;
		return a;
	}
	long long x1, y1;
	long long g = exgcd(b, a % b, x1, y1);
	x = y1, y = x1 - (a / b) * y1;
	return g;
}
/** 
 * ax = b (mod m)，x 传引用，返回是否有解；
 * 若有解，x 被更新为满足 0<=x<m/g 的最小非负解。
 * 令 step = m/g，那么 mod m 下的 g 个不同解即为 {x + k * step}，其中 0<=k<g.
 */
bool solve(long long a, long long b, long long m, long long &x) {
	long long x0, y0;
	long long g = exgcd(a, m, x0, y0);
	if (b % g != 0) return false;
	long long mod = m / g;
	x = (x0 * b / g) % mod;
	if (x < 0) x += mod;
	return true;
}