#include <bits/stdc++.h>
using namespace std;

/**
 * 用于求解mod数不为质数的乘法逆元
 * ax = 1 (mod m)
 * 等价于扩展欧几里得求同余方程 b = 1 => g = 1 的特殊情况，其解 x = x0 % mod 即为逆元
 * 
 * 复杂度 O(logm)
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
int inv(int a, int m) {
	int x, y;
	int g = exgcd(a, m, x, y);
	if (g != 1) return INT_MAX;  // 不存在逆元
	x = (x % m + m) % m;
	return x;
}