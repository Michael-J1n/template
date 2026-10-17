#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
/**
 * 斐波那契数 / Fast Doubling【单次求第 n 项模 mod】
 * F[0] = 0，F[1] = 1；0 <= n <= LLONG_MAX，1 <= mod <= INT_MAX。
 *
 * 支持操作：fib(n) 返回 int，使用全局常量 mod。
 * 适用：大下标斐波那契数查询；时间 O(log(n + 1))，额外空间 O(1)。
 */
int fib(long long n) {
    int a = 0, b = 1 % mod;
    for (int i = 63 - __builtin_clzll(n | 1LL); ~i; --i) {
        long long t = 2LL * b - a;
        if (t < 0) t += mod;
        if (t >= mod) t -= mod;
        int c = a * t % mod, d = (1LL * a * a + 1LL * b * b) % mod;
        if (n >> i & 1) {
            a = d;
            b = c >= mod - d ? c - (mod - d) : c + d;
        } else a = c, b = d;
    }
    return a;
}
