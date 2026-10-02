/**
 * 卡特兰数：C[0] = 1, C[1] = 1, C[2] = 2, C[3] = 5, C[4] = 14, C[5] = 42
 * 通项：C[n] = C(2n, n) / (n + 1) = C(2n, n) - C(2n, n-1)
 * 递推1：C[n] = (4n-2)/(n+1) C[n-1]
 * 递推2：C[n] = sum_{k=0}^{n-1} C[k] * C[n - 1 - k]
 * 性质：
 * 1. ！！一种操作数任意前缀中都不能超过另一种操作数！！：右上走格点，(1,1) 到 (n,n)，走法为 C[n] <==> 01序列，括号匹配，出入栈计数
 * 2. ！！非交叉递归结构！！（由递推2）：n 个节点可以构造多少种不同满二叉树/BST，n+2边凸多边形划分为n个三角形的剖分数：
 * 		固定边(1,n+2)，考虑第三个顶点k，连1-k-n+2，划分为左右两个不相交的剖分子问题 => f[n] = sum f[i] f[n - i - 1]
 * */

// 这里给出 O(nlogV) 的单点求值，适用n很大以及单点查询；使用递推1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int mod = 1e9 + 7;  // 示例mod数；若不是素数，后面求逆元记得改成exgcd求

int qpow(int a, int b) {
	a %= mod;
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % mod;
		a = 1LL * a * a % mod, b >>= 1;
	}
	return res;
}

ll Catlan(int n) {
	ll res = 1;
	for (int i = 1; i <= n; ++i) {
		res = res * (4 * i - 2) % mod * qpow(i + 1, mod - 2) % mod;
	}
	return res;
}