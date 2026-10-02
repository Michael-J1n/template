#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-10;

/**
 * 高斯消元；解任意线性方程组 Ax=b，其中 A 为 m*n 矩阵
 * 
 * 返回 {状态, 一个解}：0=无解，1=唯一解，2=无穷多解；无穷多解时自由变量取 0，返回其中一个解
 * 
 * 复杂度：O(N^3)
 */
pair<int, vector<double>> gauss(vector<vector<double>> a, vector<double> b) {
    int m = a.size(), n = a[0].size(), r = 0;
    vector<int> where(n, -1);
    for (int c = 0; c < n && r < m; ++c) {
        int p = r;
        for (int i = r + 1; i < m; ++i) {
            if (abs(a[i][c]) > abs(a[p][c])) p = i;
        }
        if (abs(a[p][c]) < EPS) continue;
        swap(a[r], a[p]), swap(b[r], b[p]);
        double d = a[r][c];
        for (int j = c; j < n; ++j) a[r][j] /= d;
        b[r] /= d;
        for (int i = 0; i < m; ++i) {
            if (i == r || abs(a[i][c]) < EPS) continue;
            double f = a[i][c];
            for (int j = c; j < n; ++j) a[i][j] -= f * a[r][j];
            b[i] -= f * b[r];
        }
        where[c] = r++;
    }
    for (int i = r; i < m; ++i) {
        if (abs(b[i]) >= EPS) return {0, {}};
    }
    vector<double> x(n);
    for (int c = 0; c < n; ++c) {
        if (where[c] != -1) x[c] = b[where[c]];
    }
    return {r == n ? 1 : 2, x};
}