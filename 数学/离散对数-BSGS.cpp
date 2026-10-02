#include <bits/stdc++.h>
using namespace std;
// #define int long long
using ll = long long;

/**
 * 离散对数问题：求满足 a^x = b (mod p) 的最小指数 x，gcd(a, p) = 1
 * 由于 a^phi(p) = 1 (mod p)，因此由扩展欧拉定理 b = a^x = a^(x mod phip) (mod p)，循环节是 O(phi(p)) 的
 * 但是直接枚举太慢，采用BSGS（时间换空间的根号分治）
 * 令 x = i * m - j，m = upfloor(sqrtp), 1<=i<=m, 0<=j<=m-1
 * 则 b = a^(im-j) (mod p) ==> a^{im} = b*a^j (mod p)
 * Baby step：枚举 j，将 (b*a^j % p, j) 加入哈希表
 * Giant step:枚举 i，计算 a^{im} % p 并查键
 * 总体复杂度 O(sqrtp)
 * */

int qpow(int a, int b, int p) {
	ll res = 1;
	while (b) {
		if (b & 1) res = res * a % p;
		a = 1LL * a * a % p, b >>= 1;
	}
	return (int)res;
}

// 求解满足 a^x = b (mod p) 的最小非负整数解 x, 前提 gcd(a, p) == 1
// 数据保证 a>=2, b>=1
// 复杂度 O(sqrtp)
int bsgs(int a, int b, int p) {
	if (p == 1) return 0;
	a = (a % p + p) % p, b = (b % p + p) % p;
	// 如果没有a>=2的限制的特判
	// if (a == 0 && b != 0 || a == 1 && b != 1) return -1;
	// if (a == 0 && b == 0) return 1;
	if (b == 1) return 0;
	if (b == 0 || gcd(a, p) != 1) return -1;
 	int m = (int)(sqrt(p) + 1);

 	unordered_map<int, int> mp;
 	int k;
 	for (int j = 0; j < m; ++j) {
 		if (j == 0) k = b;
 		else k = 1LL * k * a % p;

 		mp[k] = j;  // 更新最大 j 这样 x = im-j 查到就是最小解
 	}
 	int A = qpow(a, m, p);
 	for (int i = 1; i <= m; ++i) {
 		if (i == 1) k = A;
 		else k = 1LL * k * A % p;

 		if (mp.count(k)) return (int)(1LL * i * m - mp[k]);
 	}
 	return -1;
}