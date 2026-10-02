#include <bits/stdc++.h>
using namespace std;

/**
 * 适合维护：
 * 区间静态且可重复贡献二元操作的 RMQ
 * 静态：无修改操作
 * 可重复贡献：min/max/gcd/AND/OR
 * 
 * 修改：f 函数，表示需要维护的可重复贡献操作
 * 也可通过 ST<T, Op>(a, op, identity) 传入运算与单位元；默认仍为区间最小值
 * 
 * 复杂度：建表 O(nlogn)，查询 O(1)
 * 前置条件：构造数组非空；查询区间 [l,r] 非空且位于 [0,n-1]
 */
template<class T>
struct STMinOp {
    T operator()(const T& x, const T& y) const { return min(x, y); }
};

template <class T, class Op = STMinOp<T>>
struct ST {
    int n, K;
    vector<int> lg;
    vector<vector<T>> st;
    Op op;
    T identity;
    ST() : identity(numeric_limits<T>::max()) {}
    T e() const {
        return identity;
    }
    T f(const T& x, const T& y) const {
        return op(x, y);
    }
    ST(const vector<T>& a, Op _op = Op(), T _identity = numeric_limits<T>::max()) : op(move(_op)), identity(_identity) {
        n = a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
        K = lg[n];
        st.assign(K + 1, vector<T>(n));
        for (int i = 0; i < n; ++i) st[0][i] = a[i];
        for (int k = 1; k <= K; ++k) {
            int len = 1 << k, half = len >> 1;
            for (int i = 0; i + len <= n; ++i) st[k][i] = f(st[k - 1][i], st[k - 1][i + half]);
        }
    }
    T query(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return e();
        int k = lg[r - l + 1];
        return f(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
