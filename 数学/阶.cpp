#include <bits/stdc++.h>
using namespace std;

long long qpow(long long a, long long b, long long p) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p, b >>= 1;
    }
    return res;
}
long long phi(long long n) {
	long long res = n;
	for (long long i = 2; i * i <= n; ++i) {
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
 *
 * 定义：最小正整数 d 使得 x^d ≡ 1 (mod m)；不存在返回 -1
 *
 * 原理：根据欧拉定理，阶一定是 phi(m) 的因数；从 phi(m) 开始，枚举其不同质因子 p，若 x^(res/p) ≡ 1 (mod m)，则阶一定整除 res/p，可以继续缩小；最终得到的 res 即为最小的阶，无需枚举 phi(m) 的全部因数
 *
 * 复杂度：O(sqrt(m) + log^2(m))
 * 多次查询时可以使用线性筛预处理 phi 和最小质因子优化
 * m 很大时可以使用 Pollard-Rho 分解优化
 */
long long get_order(long long x, long long m) {
    if (m == 1) return 1;
    x = (x % m + m) % m;
    if (gcd(x, m) != 1) return -1; // 无解
    long long res = phi(m), n = res;
    // 阶一定是 phi(m) 的因数，从 phi(m) 开始不断除去不必要的质因子
    for (long long p = 2; p <= n / p; ++p) {
        if (n % p) continue;
        while (n % p == 0) n /= p; // 分解 phi(m)，每个不同质因子只处理一次
        while (res % p == 0 && qpow(x, res / p, m) == 1) res /= p;
    }
    if (n > 1) { // 剩余的质因子
        while (res % n == 0 && qpow(x, res / n, m) == 1) res /= n;
    }
    return res;
}