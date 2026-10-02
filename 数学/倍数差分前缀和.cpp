#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;

/**
 * 对于形如 F(x) = sum_{i=1..m} g[i]*floor(x/i)，需要求 F(1),F(2),...,F(n) 的问题：
 * 数论分块做到 O(nsqrtn) 但常数太大，慢；可以用倍数差分前缀和快速得到。
 * 
 * 复杂度：O(nlogn)
 */
vector<long long> calc(int n, int m, const vector<long long>& g) {
    vector<long long> f(n + 1);
    for (int i = 1; i <= min(n, m); ++i) {
        for (int j = i; j <= n; j += i) {
            f[j] += g[i];
            if (f[j] >= mod) f[j] -= mod;
        }
    }
    for (int i = 1; i <= n; ++i) {
        f[i] += f[i - 1];
        if (f[i] >= mod) f[i] -= mod;
    }
    return f;
}