#include <bits/stdc++.h>
using namespace std;

/**
 * ST 表
 * 区间静态且可重复贡献二元操作的 RMQ（可重复贡献：min/max/gcd/AND/OR）
 * 
 * 支持：
 * ST(a)：使用 a 数组初始化
 * arg(l,r)：返回区间 [l,r] 最小/最大值的最左侧下标
 * query(l,r)：返回区间 [l,r] 的最小/最大值
 * 
 * 复杂度：建表 O(nlogn)，查询 O(1)
 */
template<class T>
struct ST {
    int n, K;
    vector<T> a;
    vector<int> lg;
    vector<vector<int>> st;
    int f(int x, int y) const {
        if (a[x] == a[y]) return min(x, y);
        return a[x] < a[y] ? x : y;
        // if (a[x] == a[y]) return max(x, y);
        // return a[x] < a[y] ? x : y;
    }
    ST(const vector<T>& _a) : a(_a) {
        n = a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
        K = lg[n];
        st.assign(K + 1, vector<int>(n));
        for (int i = 0; i < n; ++i) st[0][i] = i;
        for (int k = 1; k <= K; ++k) {
            int len = 1 << k, half = len >> 1;
            for (int i = 0; i + len <= n; ++i) st[k][i] = f(st[k - 1][i], st[k - 1][i + half]);
        }
    }
    int arg(int l, int r) const {
        int k = lg[r - l + 1];
        return f(st[k][l], st[k][r - (1 << k) + 1]);
    }
    T query(int l, int r) const { return a[arg(l, r)]; }
};
