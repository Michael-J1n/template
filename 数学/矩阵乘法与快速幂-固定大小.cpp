#include <bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;

/**
 * 定长矩阵与向量
 *
 * 支持操作：
 * mul(a, b)：矩阵乘法，支持 N×M 与 M×K 相乘，返回 N×K 矩阵
 * qpow(a, b)：方阵快速幂，计算 a^b，要求 b >= 0（若 b=0 返回的是 I）
 *
 * 复杂度：
 * mul： O(NMK)
 * qpow：O(N^3 log b)
 * 
 * 示例用法（注意初始化时多一层大括号）：
    using Mat = Matrix<3, 3>;
    using Vec = Vector<3>;
    Mat A = { {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}} };
    Vec v = { {{1}, {2}, {3}} };
 */

template<size_t N, size_t M> using Matrix = array<array<long long, M>, N>;
template<size_t N> using Vector = Matrix<N, 1>;

template<size_t N, size_t M, size_t K>
Matrix<N, K> mul(const Matrix<N, M>& a, const Matrix<M, K>& b) {
    Matrix<N, K> c{};
    for (int i = 0; i < N; ++i) {
        for (int t = 0; t < M; ++t) {
            if (!a[i][t]) continue;
            for (int j = 0; j < K; ++j) {
                (c[i][j] += a[i][t] * b[t][j] % mod) %= mod;
            }
        }
    }
    return c;
}

template<size_t N>
Matrix<N, N> qpow(Matrix<N, N> a, long long b) {
    Matrix<N, N> res{};
    for (int i = 0; i < N; ++i) res[i][i] = 1;
    while (b) {
        if (b & 1) res = mul(res, a);
        b >>= 1;
        if (b) a = mul(a, a);
    }
    return res;
}