#include <bits/stdc++.h>
using namespace std;

/**
 * 扩展卢卡斯通过将mod数分解并进行p进分析从而得到递推式，最后使用exCRT求出解
 * 适用于求 nCm % mod, mod 不一定为素数的情景
 * 复杂度 O(logP + sumlog_p(n))
 * */
inline long long qpow(long long a, long long b, long long p) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p, b >>= 1;
    }
    return res;
}
vector<pair<long long, long long>> fact(long long n) {
    vector<pair<long long, long long>> res;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            long long c = 0;
            while (n % i == 0) n /= i, ++c;
            res.emplace_back(i, c);
        }
    }
    if (n > 1) res.emplace_back(n, 1);
    return res;
}
inline long long vpf(long long n, long long p) {  // 求 v_p(n!)，O(logn)
    long long res = 0;
    while (n)  n /= p, res += n;
    return res;
}
inline long long exgcd(long long a, long long b, long long& x, long long& y) {  // 求ax=1(modm)的逆元x-1，O(logm)
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    long long x1, y1;
    long long g = exgcd(b, a % b, x1, y1);
    x = y1, y = x1 - (a / b) * y1;
    return g;
}
inline long long inv(long long a, long long m) {
    long long x, y;
    long long g = exgcd(a, m, x, y);
    if (g != 1) return LLONG_MAX;
    x = (x % m + m) % m;
    return x;
}
pair<long long, long long> merge(long long r1, long long m1, long long r2, long long m2) {  // 扩展CRT合并同余方程组，O(nlogV)
    r1 = (r1 % m1 + m1) % m1, r2 = (r2 % m2 + m2) % m2;
    long long x, y, d = r2 - r1;
    long long g = exgcd(m1, m2, x, y);
    if (d % g != 0) return {0, -1};
    long long m = m2 / g, lcm = m1 / g * m2;
    long long k = (((d / g) % m * x )% m + m )% m, r = ((r1 + m1 * k % lcm) % lcm + lcm) % lcm;
    return {r, lcm};
}
pair<long long, long long> exCRT(vector<long long>& r, vector<long long>& m) {
    long long n= r.size(), R = r[0], M = m[0];
    for (long long i = 1; i < n; ++i) {
        auto [tr, tm] = merge(R, M, r[i], m[i]);
        if (tm == -1) return {0, -1};
        R = tr, M = tm;
    }
    return {R, M};
}
inline long long f(long long n, long long p, long long P, long long block_prod) {  // 扩展卢卡斯递推式 f(n)=f(n/p)*block^{n/P}*rem，周期项×余项
    if (n == 0) return 1;
    long long res = 1;
    long long block_sz = n / P;
    res *= qpow(block_prod, block_sz, P);
    for (long long i = P * block_sz; i <= n; ++i) {
        if (i % p != 0) (res *= (i % P)) %= P;
    }
    (res *= f(n / p, p, P, block_prod)) %= P;
    return (res % P + P) % P;
}
// 求 nCm mod P, P=p^k, O(logP)
inline long long solve(long long n, long long m, long long p, long long P, long long block_prod) {
    long long f1 = f(n, p, P, block_prod), f2 = f(m, p, P, block_prod), f3 = f(n - m, p, P, block_prod);
    long long res = f1;
    long long if2 = inv(f2, P), if3 = inv(f3, P);
    (res *= (if2 * if3 % P)) %= P;
    long long v1 = vpf(n, p), v2 = vpf(m, p), v3 = vpf(n - m, p);
    long long tmp = qpow(p, v1 - v2 - v3, P);
    return (res * tmp % P + P) % P;
}
inline long long exLucas(long long n, long long m, long long p) {  // 合并解
    if (m < 0 || m > n) return 0;
    vector<pair<long long, long long>> facts = fact(p);
    vector<long long> rs, ms;
    for (auto& [p, k] : facts) {
        long long P = 1;
        for (int i = 0; i < k; ++i) P *= p;
        long long block_prod = 1;  // 对于每个P=p^k都与处理product block
        for (long long i = 1; i < P; ++i) {
            if (i % p != 0) (block_prod *= i) %= P;
        }
        rs.emplace_back(solve(n, m, p, P, block_prod));
        ms.emplace_back(P);
    }
    auto [R, _] = exCRT(rs, ms);
    return R;
}