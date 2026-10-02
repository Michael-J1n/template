#include <bits/stdc++.h>
using namespace std;

/**
 * 矩阵二维前缀和
 * 注意输入 a 是 0-based 数组
 * 前置条件：构造矩阵非空且各行等长；查询矩形非空且坐标位于矩阵范围内
 */
template <class T>
struct Pre2D {
    int n, m;
    vector<vector<T>> sum;
    Pre2D() {}
    T e() const {
        return 0;
    }
    template <class U>
    Pre2D(const vector<vector<U>>& a) {
        n = a.size(), m = a[0].size();
        sum.assign(n + 1, vector<T>(m + 1, e()));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                sum[i + 1][j + 1] = sum[i][j + 1] + sum[i + 1][j] - sum[i][j] + a[i][j];
            }
        }
    }
    T get(int x1, int y1, int x2, int y2) const {
        x1 = max(x1, 0), y1 = max(y1, 0);
        x2 = min(x2, n - 1), y2 = min(y2, m - 1);
        if (x1 > x2 || y1 > y2) return e();
        return sum[x2 + 1][y2 + 1] - sum[x1][y2 + 1] - sum[x2 + 1][y1] + sum[x1][y1];
    }
};
