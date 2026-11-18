#include <bits/stdc++.h>
using namespace std;

/**
 * 单点求数论函数
 * 复杂度：均为 O(sqrt n)
 * 
 * 附：数论函数值数量级
 * 素因子个数：O(log n)
 * 正因子个数：O(sqrt n)
 * 正因子和：O(n log n)
 */

// 所有单点计算的复杂度均为 O(sqrtn)

// 素因子：见分解质因数部分

// 正约数列表 {d: d | n}
vector<int> divisors(int n) {
	vector<int> a, b;
	for (int i = 1; i * i <= n; ++i) {
		if (n % i == 0) {
			a.emplace_back(i);
			if (i * i != n) b.emplace_back(n / i);
		}
	}
	reverse(b.begin(), b.end());
	a.insert(a.end(), b.begin(), b.end());
	return a;
}

// 欧拉函数 phi(n)=nΠ(1-1/p)
int phi(int n) {
	int res = n;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			res -= res / i;
			while (n % i == 0) n /= i;
		}
	}
	if (n > 1) res -= res / n;
	return res;
}

// 约数个数 d(n)=Π(ai+1)
long long d(int n) {
	long long res = 1;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			int e = 0;
			while (n % i == 0) ++e, n /= i;
			res *= 1LL * (e + 1);
		}
	}
	if (n > 1) res *= 2;
	return res;
}

// 约数和 f(n)=Π(1+p+...+p^a)
long long f(int n) {
	long long res = 1;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			long long p = 1, s = 1;
			while (n % i == 0) n /= i, p *= i, s += p;
			res *= s;
		}
	}
	if (n > 1) res *= (1LL + n);
	return res;
}

// 莫比乌斯函数 mu(1)=1, mu(n)=0, n含平方因子，否则 mu(p1p2..pk)=(-1)^k
int mu(int n) {
	if (n == 1) return 1;
	int k = 0;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			int e = 0;
			while (n % i == 0) n /= i, ++e;
			if (e >= 2) return 0;
			++k;
		}
	}
	if (n > 1) ++k;
	return (k & 1) ? -1 : 1;
}