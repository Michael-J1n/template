#include <bits/stdc++.h>
using namespace std;

/**
 * 定义 mod n 的原根 g：最小的 x 满足 g^x = 1 (mod n) 为 x = phi(n)
 * 或：g 对 mod n 的阶 是 phi(n)
 * 性质：
 * 0. mod n 的原根存在当且仅当 n = 2, 4, p^k, 或 2 * p^k，p 为素数
 * 1. 若 g 为原根，则 {g mod n, g^2 mod n, ..., g^phi[n] mod n} 为 n 的缩系 = {x: x < n, gcd(x, n) = 1}
 * 		反正假设 g^i=g^j 则 g^{i-j}=1 mod n 找到更小阶，矛盾。因此 phi(n) 个数两两不同余，为缩系
 * 2. 若 mod n 原根存在，则有 phi(phi(n)) 个原根：
 * 		记其中一原根为g其阶为δ(g)=phi(n)，有δ(g^k)=δ(g)/gcd(phi(n),k)=phi(n)/gcd(phi(n),k):=phi(n) => gcd(phi(n),k)=1
 * 		因此若g为原根，所有g^k, gcd(k,phi(n))=1也是原根，共有 phi(phi(n)) 个
 * 3. 最小原根的大小约为 O(n^0.25)
 * */

const int N = 1e6 + 5;
int vis[N], prim[N], phi[N], cnt = 0;

// 快速幂，O(logb)
int qpow(int a, int b, int p) {
	a %= p;
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % p;
		a = 1LL * a * a % p, b >>= 1;
	}
	return res;
}

// 预处理素数列表和欧拉函数，O(N)
// 在mod数较大查询较小时可以单点来求，每个点 O(sqrtn)
void init() {
	vis[1] = true, phi[1] = 1;
	for (int i = 2; i < N; ++i) {
		if (!vis[i]) prim[++cnt] = i, phi[i] = i - 1;
		for (int j = 1; i * prim[j] < N; ++j) {
			int p = prim[j];
			vis[i * p] = true;
			if (i % p == 0) {
				phi[i * p] = phi[i] * p;
				break;
			}
			phi[i * p] = phi[i] * (p - 1);
		}
	}
}

// 判断 n 是否存在原根，O(sqrtn)
// 性质 0
bool check_exist(int n) {
	if (n == 2 || n == 4) return true;
	if (n % 2 == 0) n /= 2;
	if (n % 2 == 0) return false;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			while (n % i == 0) n /= i;
			return (n == 1);
		}
	}
	return true;
}

// 判断 g 是否是 mod n 的原根
// 只需判断是否有 g^phi(n)=1 (mod n) 以及其他更小的数不是阶即可；实现是不用所有素因子，只要下一层的即可
// 复杂度：循环枚举所有 phi[n] 以下的素数，循环内部每次仅有快速幂，总体约为 O(log^2n))
bool check_isroot(int g, int n) {
	int pn = phi[n];
	if (qpow(g, pn, n) != 1) return false;
	for (int j = 1; prim[j] <= pn; ++j) {
		if (pn % prim[j] == 0) {
			int pm = phi[n] / prim[j];
			if (qpow(g, pm, n) == 1) return false;
		}
	}
	return true;
}

// 假设 g 为 mod n 的原根，生成所有原根，复杂度 O(n) （若加上排序即为 O(nlogn)，可通过计数排序优化）
// 所有满足 gcd(i, phi(n)) 的 i 会生成 g^i
vector<int> generate(int g, int n) {
	int pn = phi[n];
	vector<int> res; res.reserve(phi[pn]);
	for (int i = 1; i <= pn; ++i) {
		if (gcd(pn, i) == 1) res.emplace_back(qpow(g, i, n));
	}
	sort(res.begin(), res.end());
	return res;
}

// 枚举求出 n 的最小原根
// 复杂度 O(n^0.25*logn) （由于原根密度较大，一个估计是最小原根出现在n^0.25的位置）
int get_root(int n) {
	if (!check_exist(n)) return -1;
	int g;
	for (int i = 1; i <= n; ++i) {
		if (check_isroot(i, n)) {
			g = i;
			break;
		}
	}
	return g;
}

// 求 n 的所有原根
vector<int> solve(int n) {
	if (!check_exist(n)) return {};
	// int cnt = phi[phi[n]];  // 原根个数
	int g = get_root(n);
    vector<int> res = generate(g, n);
    return res;
}