#include <bits/stdc++.h>
using namespace std;

long long qpow(long long a, long long b, long long mod) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

/**
 * 矩阵行列式 / Determinant【高斯消元求模意义下行列式】
 * 矩阵下标为 0-based，要求矩阵为 n × n，mod 为质数。
 *
 * 支持操作：det(a, mod)：返回矩阵 a 的行列式对 mod 取模的结果。
 * 
 * 适用：矩阵树定理、模质数意义下的行列式计算。
 * 复杂度：O(n^3)。
 */
long long det(vector<vector<long long>> a, long long mod) {
    int n = a.size();
    for (auto& row : a) {
        for (auto& x : row) x = (x % mod + mod) % mod;
    }
    long long res = 1;
    for (int i = 0; i < n; ++i) {
        int p = i;
        while (p < n && a[p][i] == 0) ++p;
        if (p == n) return 0;
        if (p != i) {
            swap(a[p], a[i]);
            res = (mod - res) % mod;
        }
        res = res * a[i][i] % mod;
        long long inv = qpow(a[i][i], mod - 2, mod);
        for (int j = i + 1; j < n; ++j) {
            long long k = a[j][i] * inv % mod;
            for (int t = i; t < n; ++t) a[j][t] = (a[j][t] - k * a[i][t] % mod + mod) % mod;
        }
    }
    return res;
}

/**
 * 矩阵行列式 / Determinant【任意模数下高斯消元】
 * 矩阵下标为 0-based，要求矩阵为 n × n。
 *
 * 支持操作：det(a, mod)：返回矩阵 a 的行列式对 mod 取模的结果。其中 mod 不要求为质数，不使用模逆元。
 *
 * 复杂度：O(n^3 log mod)。
 */
long long det(vector<vector<long long>> a, long long mod) {
    int n = a.size();
    for (auto& row : a) {
        for (auto& x : row) x = (x % mod + mod) % mod;
    }
    long long res = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (a[j][i]) {
                long long k = a[i][i] / a[j][i];
                for (int t = i; t < n; ++t) a[i][t] = (a[i][t] - k * a[j][t] % mod + mod) % mod;
                swap(a[i], a[j]);
                res = (mod - res) % mod;
            }
        }
        if (a[i][i] == 0) return 0;
        res = res * a[i][i] % mod;
    }
    return res;
}