#include <bits/stdc++.h>
using namespace std;

/**
 * 二维静态区间可重复贡献操作查询
 * ST2D<T, Op>(a, op, identity) 可传入运算与单位元；默认维护区间最小值
 * 
 * 复杂度 O(nmlognlogm)
 * 前置条件：构造矩阵非空且各行等长；查询矩形非空且坐标位于矩阵范围内
 */
template<class T>
struct ST2DMinOp {
    T operator()(const T& x, const T& y) const { return min(x, y); }
};

template <class T, class Op = ST2DMinOp<T>>
struct ST2D {
    int n, m, K1, K2;
    vector<int> lg1, lg2;
    vector<T> st;
    Op op;
    T identity;
    ST2D() : identity(numeric_limits<T>::max()) {}
    T e() const {
        return identity;
    }
    T f(const T& x, const T& y) const {
        return op(x, y);
    }
    size_t id(int k1, int k2, int i, int j) const {
        return (((size_t(k1) * (K2 + 1) + k2) * n + i) * m + j);
    }
    T& at(int k1, int k2, int i, int j) {
        return st[id(k1, k2, i, j)];
    }
    const T& at(int k1, int k2, int i, int j) const {
        return st[id(k1, k2, i, j)];
    }
    ST2D(const vector<vector<T>>& a, Op _op = Op(), T _identity = numeric_limits<T>::max()) : op(move(_op)), identity(_identity) {
        n = a.size(), m = a[0].size();
        lg1.assign(n + 1, 0), lg2.assign(m + 1, 0);
        for (int i = 2; i <= n; ++i) lg1[i] = lg1[i >> 1] + 1;
        for (int i = 2; i <= m; ++i) lg2[i] = lg2[i >> 1] + 1;
        K1 = lg1[n], K2 = lg2[m];
        st.resize(1LL * (K1 + 1) * (K2 + 1) * n * m);
        for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) at(0, 0, i, j) = a[i][j];
        for (int k1 = 0; k1 <= K1; ++k1) {
            for (int k2 = 0; k2 <= K2; ++k2) {
                if (!k1 && !k2) continue;
                int len1 = 1 << k1, len2 = 1 << k2;
                if (k1) {
                    int half = len1 >> 1;
                    for (int i = 0; i + len1 <= n; ++i) {
                        for (int j = 0; j + len2 <= m; ++j) at(k1, k2, i, j) = f(at(k1 - 1, k2, i, j), at(k1 - 1, k2, i + half, j));
                    }
                } else {
                    int half = len2 >> 1;
                    for (int i = 0; i + len1 <= n; ++i) {
                        for (int j = 0; j + len2 <= m; ++j) at(k1, k2, i, j) = f(at(k1, k2 - 1, i, j), at(k1, k2 - 1, i, j + half));
                    }
                }
            }
        }
    }
    T query(int x1, int y1, int x2, int y2) const {
        x1 = max(x1, 0), y1 = max(y1, 0);
        x2 = min(x2, n - 1), y2 = min(y2, m - 1);
        if (x1 > x2 || y1 > y2) return e();
        int k1 = lg1[x2 - x1 + 1], k2 = lg2[y2 - y1 + 1];
        int nx = x2 - (1 << k1) + 1, ny = y2 - (1 << k2) + 1;
        T a = f(at(k1, k2, x1, y1), at(k1, k2, nx, y1));
        T b = f(at(k1, k2, x1, ny), at(k1, k2, nx, ny));
        return f(a, b);
    }
};
