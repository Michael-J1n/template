#include <bits/stdc++.h>
using namespace std;
#define int long long
using pii = pair<int, int>;

/** 在 O(logn) 时间内求出 Fn；初始条件 F0=0, F1=1 */

pii fib2(int n, int mod) {
    if (mod == 1) return {0, 0};
    if (n == 0) return {0, 1};
    auto [a, b] = fib2(n >> 1, mod);
    int t = (2 * b % mod - a + mod) % mod, c = a * t % mod, d = (a * a % mod + b * b % mod) % mod;
    if ((n & 1) == 0) return {c, d};
    return {d, (c + d) % mod};
}

int fib(int n, int mod) { return fib2(n, mod).first; }