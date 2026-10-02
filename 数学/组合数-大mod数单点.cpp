#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;

/**
 * 在mod数很大的时候，显然类似lucas等的逆元预处理会MLE
 * 这时如果查询很少，就可以用暴力展开+单点exgcd逆元的方法在 O(mlogp) 的时间内处理一个 nCm 的查询
 * 
 * 具体来说 nCm=prod((n-m+i)/i), i从1到m
 * 或者：在只用到了固定 n 的组合数（例如，需要计算类似 sum nCi 的时候），可以线性递推：nCi = nC(i-1)*(n-i+1)*inv(i)，做到 O(n) 复杂度
 * 【注意】如果 mod 数为素数，下面的求逆元可以直接替换为 a^{mod-2} 来计算
 */
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
long long inv(long long a, long long m) {
	long long x, y;
	long long g = exgcd(a, m, x, y);
	if (g != 1) return LLONG_MAX;
	x = (x % m + m) % m;
	return x;
}
// long long inv(long long a, long long m) { return qpow(a, m - 2, m); }
long long C(long long n, long long m) {
    if (m < 0 || m > n) return 0;
    long long res = 1;
    for (int i = 1; i <= m; ++i) {
        res = res * (n - m + i) % mod * inv(i, mod) % mod;
    }
    return res;
}