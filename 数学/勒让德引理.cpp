#include <bits/stdc++.h>
using namespace std;
// #define int long long

// 求 v_p(n!) = sum{[n/p^i]}，复杂度 O(logn)
int vp(int n, int p) {
    int s = 0;
    while (n) n /= p, s += n;
    return s;
}

// 应用：nCm 中素数 p 的次数
// 可以在逆元不存在的时候计算大组合数模质数
int vpC(int n, int m, int p) {
	if (m < 0 || m > n) return 0;
	return vp(n, p) - vp(m, p) - vp(n - m, p);
}