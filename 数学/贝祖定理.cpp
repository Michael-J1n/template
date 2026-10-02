#include <bits/stdc++.h>
using namespace std;

/**
 * Bezout 定理：
 * ax + by = c 存在整数解的条件是 gcd(a, b) | c
 * <==> a1x1 + a2x2 + ... + anxn = g 存在正整数解的条件是 gcd(a1,...,an) | g
 * <==> gcd(x1, x2, ..., xn) 一定能表示为 x1, x2, ..., xn 的线性组合
 * <==> x1, x2, ..., xn 的任意正线性组合中最小的是 gcd(x1, x2, ..., xn)
 * 
 * 应用：
 * LC 365, LC 1250
 * */

int gcd(int x, int y) {
    if (y == 0) return x;
    return gcd(y, x % y);
}

/**
 * 判断 nums[0],...,nums[n-1] 的线性组合是否有解使得值为 k
 * */
bool bezout(vector<int>& nums, int k) {
	int g = 0;
	for (auto& x : nums) g = gcd(g, x);
	return k % g == 0;
}
