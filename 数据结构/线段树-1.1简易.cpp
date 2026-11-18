#include <bits/stdc++.h>
using namespace std;

/**
 * 支持具有结合律的操作，例如 +/max/min/gcd/lcm/&/|/^/矩阵乘
 * 前置条件：构造规模为正或构造数组非空；修改下标与查询区间均合法，查询区间非空
 */
template<class T>
struct SegTree {
    int n;
    T e;
    vector<T> tr;
    // -----------------------------修改开始----------------------------------
    T f(const T& a, const T& b) {
        return max(a, b);
    }
    // -----------------------------修改结束----------------------------------
    SegTree() {}
    SegTree(int _n, T _e) : n(_n), e(_e) { tr.assign(n * 4 + 5, e); }
    SegTree(const vector<T>& a, T _e): n(a.size()), e(_e) {
        tr.assign(n * 4 + 5, e);
        build(1, 0, n - 1, a);
    }
    void pushup(int u) { tr[u] = f(tr[u << 1], tr[u << 1 | 1]); }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) {
            tr[u] = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void modify(int u, int l, int r, int p, const T& v, int flag) {
        if (l == r) {
            if (flag == 0) tr[u] = v;
            else if (flag == 1) tr[u] = f(tr[u], v);
            return;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) modify(u << 1, l, mid, p, v, flag);
        else modify(u << 1 | 1, mid + 1, r, p, v, flag);
        pushup(u);
    }
    void set(int p, const T& v) { modify(1, 0, n - 1, p, v, 0); }
    void apply(int p, const T& v) { modify(1, 0, n - 1, p, v, 1); }
    T query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return f(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    T query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return e;
        return query(1, 0, n - 1, l, r);
    }
    T get(int p) { return query(p, p); }
};

/**
 * 附注：若想要建立一个 vector<SegTree<T>>，大小 nxm，这样初始化：
 * vector<SegTree<T>> tr; tr.reserve(n);
 * for (int i = 0; i < n; ++i) tr.emplace_back(m, e);
 * 
 * 或者：
 * vector<SegTree<T>> trs(27);
 * for (int k = 1; k <= 26; ++k) {
 *     vi a(n);
 *     trs[k] = SegTree<T>(a, INF);
 * }
 */
