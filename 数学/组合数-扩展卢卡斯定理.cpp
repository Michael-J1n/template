#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

/**
 * 扩展卢卡斯通过将mod数分解并进行p进分析从而得到递推式，最后使用exCRT求出解
 * 适用于求 nCm % mod, mod 不一定为素数的情景
 * 复杂度 O(logP + sumlog_p(n))
 * */
#define int long long
inline int qpow(int a, int b, int p) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p, b >>= 1;
    }
    return res;
}
vector<pii> fact(int n) {
    vector<pii> res;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            int c = 0;
            while (n % i == 0) n /= i, ++c;
            res.emplace_back(i, c);
        }
    }
    if (n > 1) res.emplace_back(n, 1);
    return res;
}
// 求 v_p(n!)，O(logn)
inline int vpf(int n, int p) {
    int res = 0;
    while (n)  n /= p, res += n;
    return res;
}
// 求ax=1(modm)的逆元x-1，O(logm)
inline int exgcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    int x1, y1;
    int g = exgcd(b, a % b, x1, y1);
    x = y1, y = x1 - (a / b) * y1;
    return g;
}
inline int inv(int a, int m) {
    int x, y;
    int g = exgcd(a, m, x, y);
    if (g != 1) return INT_MAX;
    x = (x % m + m) % m;
    return x;
}
// 扩展CRT合并同余方程组，O(nlogV)
pii merge(int r1, int m1, int r2, int m2) {
    r1 = (r1 % m1 + m1) % m1, r2 = (r2 % m2 + m2) % m2;
    int x, y, d = r2 - r1;
    int g = exgcd(m1, m2, x, y);
    if (d % g != 0) return {0, -1};
    int m = m2 / g, lcm = m1 / g * m2;
    int k = (((d / g) % m * x )% m + m )% m, r = ((r1 + m1 * k % lcm) % lcm + lcm) % lcm;
    return {r, lcm};
}
pii exCRT(vector<int>& r, vector<int>& m) {
    int n= r.size(), R = r[0], M = m[0];
    for (int i = 1; i < n; ++i) {
        auto [tr, tm] = merge(R, M, r[i], m[i]);
        if (tm == -1) return {0, -1};
        R = tr, M = tm;
    }
    return {R, M};
}
// 扩展卢卡斯递推式 f(n)=f(n/p)*block^{n/P}*rem，周期项×余项
inline int f(int n, int p, int P, int block_prod) {
    if (n == 0) return 1;
    int res = 1;
    int block_sz = n / P;
    res *= qpow(block_prod, block_sz, P);
    for (int i = P * block_sz; i <= n; ++i) {
        if (i % p != 0) (res *= (i % P)) %= P;
    }
    (res *= f(n / p, p, P, block_prod)) %= P;
    return (res % P + P) % P;
}
// 求 nCm mod P, P=p^k, O(logP)
inline int solve(int n, int m, int p, int P, int block_prod) {
    int f1 = f(n, p, P, block_prod), f2 = f(m, p, P, block_prod), f3 = f(n - m, p, P, block_prod);
    int res = f1;
    int if2 = inv(f2, P), if3 = inv(f3, P);
    (res *= (if2 * if3 % P)) %= P;
    int v1 = vpf(n, p), v2 = vpf(m, p), v3 = vpf(n - m, p);
    int tmp = qpow(p, v1 - v2 - v3, P);
    return (res * tmp % P + P) % P;
}
// 扩展卢卡斯定理的合并解部分
inline int exLucas(int n, int m, int p) {
    if (m < 0 || m > n) return 0;
    vector<pii> facts = fact(p);
    vector<int> rs, ms;
    for (auto& [p, k] : facts) {
        int P = qpow(p, k, 1e9+7);
        int block_prod = 1;  // 对于每个P=p^k都与处理product block
        for (int i = 1; i < P; ++i) {
            if (i % p != 0) (block_prod *= i) %= P;
        }
        rs.emplace_back(solve(n, m, p, P, block_prod));
        ms.emplace_back(P);
    }
    auto [R, _] = exCRT(rs, ms);
    return R;
}