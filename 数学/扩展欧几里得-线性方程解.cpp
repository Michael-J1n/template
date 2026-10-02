#include <bits/stdc++.h>
using namespace std;

/**
 * ax + by = c:
 * 1. ax + by = gcd(a, b) 有解的话，解特解 x0, y0
 * 2. 扩展到 c，x = c / g x0, y = c / g y0
 * 3. 通解： x + bk/g, y - ak/g
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

/**
 * 求解 ax + by = c 的第 k 组解
 * */
pair<int, int> solve(int a, int b, int c, int k) {
	if (c % gcd(a, b) != 0 || (a == 0 && b == 0 && c != 0)) return {INT_MAX, INT_MAX};  // 无解的情况
	if (a == 0 && b == 0 && c == 0) return {1, 1};

	int x, y;
	int g = exgcd(a, b, x, y);
	if (g < 0) g *= -1, x *= -1, y *= -1;
	x *= c / g, y *= c / g;
	x += b / g * k, y -= a / g * k;
	return {x, y};
}