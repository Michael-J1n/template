#include <bits/stdc++.h>
using namespace std;

/**
 * 求 ax = b (mod m) 的解
 * <=> 存在 y 使得 ax + my = b，有解当且仅当 gcd(a, m) | b
 * 使用扩欧求解 ax + my = gcd(a,m)，得到特解 x0, y0 => a(x0*b/g) + m(y0*b/g) = b
 * => x = x0 * b / g (mod b) = x0 * b / g (mod m / g)
 * 
 * 注意 x0 * b / g 是 mod m / g 的特解，在 mod m 下一共有 g 个解
 * 其他解可以通过 + m / g * k 得到
 * */

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

// ax = b (mod m)，x 传引用，返回是否有解
bool solve(int a, int b, int m, int &x) {
	int x0, y0;
	int g = exgcd(a, m, x0, y0);

	if (b % g != 0) return false;

	int mod = m / g;
	x = (x0 * b / g) % mod;
	if (x < 0) x += mod;
	return true;
}