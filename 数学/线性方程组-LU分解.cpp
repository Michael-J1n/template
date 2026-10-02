#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-10;

/**
 * PLU 分解；对任意 m*n 矩阵预处理 PA=LU
 * U 为消元后的行阶梯矩阵，L[i][r] 保存第 r 次 pivot 消第 i 行时的 multiplier
 *
 * solve(b) 解 Ax=b，返回 {状态, 一个解}：0=无解，1=唯一解，2=无穷多解；无穷多解时自由变量取 0
 *
 * 复杂度：预处理 O(N^3)，查询 O(N^2)
 */
struct PLU {
    int m, n, rank;
    vector<vector<double>> l, u;
    vector<int> p, piv;
    PLU(vector<vector<double>> a) {
        m = a.size(), n = a[0].size(), rank = 0;
        l.assign(m, vector<double>(m));
        u = a;
        p.resize(m);
        iota(p.begin(), p.end(), 0);
        for (int i = 0; i < m; ++i) l[i][i] = 1;
        for (int c = 0; c < n && rank < m; ++c) {
            int q = rank;
            for (int i = rank + 1; i < m; ++i) {
                if (abs(u[i][c]) > abs(u[q][c])) q = i;
            }
            if (abs(u[q][c]) < EPS) continue;
            swap(u[rank], u[q]), swap(p[rank], p[q]);
            for (int j = 0; j < rank; ++j) swap(l[rank][j], l[q][j]);
            piv.emplace_back(c);
            for (int i = rank + 1; i < m; ++i) {
                double f = u[i][c] / u[rank][c];
                l[i][rank] = f, u[i][c] = 0;
                for (int j = c + 1; j < n; ++j) u[i][j] -= f * u[rank][j];
            }
            ++rank;
        }
    }
    pair<int, vector<double>> solve(const vector<double>& b) const {
        vector<double> y(m);
        for (int i = 0; i < m; ++i) {
            y[i] = b[p[i]];
            for (int j = 0; j < min(i, rank); ++j) y[i] -= l[i][j] * y[j];
        }
        for (int i = rank; i < m; ++i) {
            if (abs(y[i]) >= EPS) return {0, {}};
        }
        vector<double> x(n);
        for (int i = rank - 1; i >= 0; --i) {
            int c = piv[i];
            double s = y[i];
            for (int j = c + 1; j < n; ++j) s -= u[i][j] * x[j];
            x[c] = s / u[i][c];
        }
        return {rank == n ? 1 : 2, x};
    }
};