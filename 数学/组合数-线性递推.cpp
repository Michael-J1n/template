#include <bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;

/**
 * 组合数线性递推：固定 n，求 C(n, 0)...C(n, n)
 *
 * 复杂度：O(n)
 */
vector<int> binom_row(int n) {
    vector<int> inv(n + 1), c(n + 1);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; ++i) inv[i] = 1LL * (mod - mod / i) * inv[mod % i] % mod;
    c[0] = 1;
    for (int k = 1; k <= n; ++k) c[k] = 1LL * c[k - 1] * (n - k + 1) % mod * inv[k] % mod;
    return c;
}