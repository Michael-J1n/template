#include <bits/stdc++.h>
using namespace std;

int qpow(int a, int b, int p) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p, b >>= 1;
    }
    return res;
}

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

/**
 * 求 x 对于 mod m 的阶
 * 复杂度 O(sqrtm) 来源于分解以及求phim
 * 
 * 在多次查询时可以使用线性筛预处理优化
 * 在m很大的时候可以使用pr分解优化
 * */
int get_order(int x, int m) {
	if (m == 1) return 0;
	x = (x % m + m) % m;
	if (gcd(x, m) != 1) return -1;  // 无解

	int phim = phi(m);
	vector<int> ds = divisors(phim);
	// 阶一定是phi的因数
	for (auto& d : ds) {
		if (qpow(x, d, m) == 1) return d;
	}
	return -1;
}