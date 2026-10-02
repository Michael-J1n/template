#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;

/**
 * 可以使用矩阵加速的状态转移，快速求作用 k 次后的结果
 * 例如 dp[t+1][j] = sum_i(dp[t][i] * w[i][j])；具体需要自己写出矩阵递推式
 * 
 * 复杂度：O(n^3 logk)
 */
using Matrix = vector<vector<long long>>;
Matrix mul(const Matrix& a, const Matrix& b) {
    int n = a.size(), m = b.size(), k = b[0].size();
    Matrix c(n, vector<long long>(k));
    for (int i = 0; i < n; ++i) {
        for (int t = 0; t < m; ++t) {
            if (!a[i][t]) continue;
            for (int j = 0; j < k; ++j) {
                (c[i][j] += a[i][t] * b[t][j] % mod) %= mod;
            }
        }
    }
    return c;
}
Matrix qpow(Matrix a, long long b) {
    int n = a.size();
    Matrix res(n, vector<long long>(n));
    for (int i = 0; i < n; ++i) res[i][i] = 1;
    while (b) {
        if (b & 1) res = mul(res, a);
        b >>= 1;
        if (b) a = mul(a, a);
    }
    return res;
}