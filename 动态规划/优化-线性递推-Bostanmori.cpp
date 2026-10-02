#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 4e6;
const int mod = 998244353, g = 3, gi = 332748118;

int qpow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % mod;
		a = 1LL * a * a % mod, b >>= 1;
	}
	return res;
}
int inv(int a) {
	return qpow(a, mod - 2);
}
void NTT(vector<ll>& A, int n, int op) {
	for (int i = 1, j = 0; i < n; ++i) {
		int b = n >> 1;
		for (; j & b; b >>= 1) j ^= b;
		j ^= b;
		if (i < j) swap(A[i], A[j]);
	}
	for (int i = 2; i <= n; i <<= 1) {
		ll g1 = qpow(op == 1 ? g : gi, (mod - 1) / i);
		for (int j = 0; j < n; j += i) {
			ll gk = 1;
			for (int k = j; k < j + i / 2; ++k) {
				ll x = A[k], y = A[k + i / 2] * gk % mod;
				A[k] = (x + y) % mod, A[k + i / 2] = (x - y + mod) % mod;
				gk = gk * g1 % mod;
			}
		}
	}
}

/**
 * Bostan-Mori 求线性递推第 n 项
 *
 * 状态定义：
 * 已知 k 阶线性递推：
 * f[i] = c[0] * f[i - 1] + c[1] * f[i - 2] + ... + c[k - 1] * f[i - k]
 * a[i] 表示初始值 f[i]，传入 f[0..k-1]
 * c[i] 表示 f[x - i - 1] 的系数，顺序从近到远
 *
 * 转移：
 * 递推式对应生成函数：
 * F(x) = P(x) / Q(x)
 * Q(x) = 1 - c[0] * x - c[1] * x^2 - ... - c[k - 1] * x^k
 * P(x) = F(x) * Q(x) 的前 k 项
 * 每次乘 Q(-x)，按 n 的奇偶性保留 P * Q(-x) 的奇数项或偶数项
 * Q 始终保留 Q(x) * Q(-x) 的偶数项
 *
 * 复杂度：
 * O(k log k log n)
 *
 * 注意：
 * a 和 c 长度必须相同
 * a 从 f[0] 开始给
 * 缺项系数补 0
 * 如果线性递推有常数项，无法使用，见矩阵快速幂
 */
int bostanMori(vector<ll> a, vector<ll> c, long long n) {
    int k = a.size();
    auto norm = [&](long long x) {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    };
    for (auto& x : a) x = norm(x);
    for (auto& x : c) x = norm(x);
    vector<ll> p(k), q(k + 1);
    q[0] = 1;
    for (int i = 0; i < k; ++i) q[i + 1] = c[i] ? mod - c[i] : 0;
    for (int i = 0; i < k; ++i) {
        p[i] = a[i];
        for (int j = 1; j <= i; ++j) {
            p[i] = (p[i] + a[i - j] * q[j]) % mod;
        }
    }
    while (n) {
        vector<ll> nq = q;
        for (int i = 1; i < nq.size(); i += 2) {
            if (nq[i]) nq[i] = mod - nq[i];
        }
        int sp = p.size() + nq.size() - 1;
        int sq = q.size() + nq.size() - 1;
        int len = 1;
        while (len < max(sp, sq)) len <<= 1;
        p.resize(len), q.resize(len), nq.resize(len);
        NTT(p, len, 1), NTT(q, len, 1), NTT(nq, len, 1);
        for (int i = 0; i < len; ++i) {
            p[i] = p[i] * nq[i] % mod;
            q[i] = q[i] * nq[i] % mod;
        }
        NTT(p, len, -1), NTT(q, len, -1);
        int iv = inv(len);
        for (int i = 0; i < sp; ++i) p[i] = p[i] * iv % mod;
        for (int i = 0; i < sq; ++i) q[i] = q[i] * iv % mod;
        vector<ll> np, qq;
        for (int i = n & 1; i < sp; i += 2) np.push_back(p[i]);
        for (int i = 0; i < sq; i += 2) qq.push_back(q[i]);
        p.swap(np), q.swap(qq);
        n >>= 1;
    }
    return p[0] % mod;
}