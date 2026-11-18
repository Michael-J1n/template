#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

/**
 * 单点求斐波那契数；定义：F(0) = 0, F(1) = 1
 * 求 fib(n)：返回 F(n) % mod
 *
 * 复杂度：O(log n)
 */
int fib(long long n) {
    int a = 0, b = 1 % mod;
    for (int i = 63 - __builtin_clzll(n | 1LL); ~i; --i) {
        long long t = 2LL * b - a;
        if (t < 0) t += mod; if (t >= mod) t -= mod;
        int c = a * t % mod, d = (1LL * a * a + 1LL * b * b) % mod;
        if (n >> i & 1) {
            a = d, b = c + d;
            if (b >= mod) b -= mod;
        } else a = c, b = d;
    }
    return a;
}

/**
 * 斐波那契数多次大下标查询
 *
 * 复杂度：预处理 O(63)，查询 O(popcount(n))
 */
struct Fibonacci {
    array<pair<int, int>, 63> pw;
    Fibonacci() {
        pw[0] = {1 % mod, 1 % mod};
        for (int i = 1; i < 63; ++i) {
            auto [a, b] = pw[i - 1];
            long long t = 2LL * b - a; if (t < 0) t += mod; if (t >= mod) t -= mod;
            int c = a * t % mod, d = (1LL * a * a + 1LL * b * b) % mod;
            pw[i] = {c, d};
        }
    }
    int fib(long long n) const {
        int a = 0, b = 1 % mod;
        while (n) {
            int i = __builtin_ctzll(n);
            auto [c, d] = pw[i];
            int x = b - a; if (x < 0) x += mod;
            int na = (1LL * a * d + 1LL * x * c) % mod, nb = (1LL * a * c + 1LL * b * d) % mod;
            a = na, b = nb, n &= n - 1;
        }
        return a;
    }
};