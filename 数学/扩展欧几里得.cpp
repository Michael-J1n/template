#include <bits/stdc++.h>
using namespace std;

/**
 * 扩展欧几里得定理：
 * 求 ax + by = gcd(a, b) 特解：
 * gcd(a, b) = ax + by
 * gcd(b, a % b) = bx1 + (a%b)y1 = bx1 + (a-[a/b]*b)y1 = ay1 + b(x1-[a/b]y1)
 * 对比系数，x = y1, y = x1 - [a / b] y1
 * 因此可以递归求解，一边欧几里得递降 gcd，一边更新 x, y
 * 底层为 b == 0，有 ax + 0y = ax = gcd(a, 0) = a ==> (x, y) = (1, 0)
 * */


/**
 * 求 ax + by = gcd(a, b) 的特解
 * 函数返回 gcd(a, b)，同时 x 和 y 由于按引用传参，直接原地修改为特解值
 * 复杂度 O(log(min(a,b)))
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
 * 求 ax + by = gcd(a, b) 的第 k 组通解
 * 通解：(x, y) = (x0 + bk / g, y0 - ak / g)
 * */
pair<int, int> kth_sol(int a, int b, int k) {
	int x, y;
	int g = exgcd(a, b, x, y);
	x += b / g * k, y -= a / g * k;
	return {x, y};
}