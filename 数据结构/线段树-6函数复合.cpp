#include <bits/stdc++.h>
using namespace std;

/**
 * 函数复合线段树【维护区间仿射函数复合】
 *
 * 每个位置 i 维护函数 f_i(x) = a_i * x + b_i (mod mod)，区间 [l, r] 表示从左到右依次执行 f_l, f_(l+1), ..., f_r。
 * 合并时先执行左区间，再执行右区间，即 merge(L, R) = R(L(x))。函数复合满足结合律，但不满足交换律，因此合并顺序不能颠倒。要求所有函数系数和输入 x 均在 [0, mod - 1]。
 *
 * 支持操作：
 * set(p, {a, b})：单点修改函数。
 * query(l, r)：查询区间复合函数，返回 {a, b}，表示 f(x) = ax + b。
 * calc(l, r, x)：计算初始值 x 依次经过区间所有函数后的结果。
 *
 * 适用：从左到右执行的操作可以表示为仿射函数。
 * 复杂度：建树 O(n)，单点修改 O(log n)，区间查询 O(log n)。
 */
template<class T>
struct SegTree {
    static constexpr T mod = 1e9 + 7;
    // -----------------------------修改开始----------------------------------
    struct Node {
        T a, b;
    };
    int n;
    vector<Node> tr;
    Node E = {1, 0};
    Node merge(const Node& a, const Node& b) {
        Node res;
        // 先执行 a，再执行 b，即 b(a(x))
        res.a = 1LL * b.a * a.a % mod;
        res.b = (1LL * b.a * a.b + b.b) % mod;
        return res;
    }
    void leaf(int u, int p, Node v) {
        tr[u] = v;
    }
    // -----------------------------修改结束----------------------------------
    SegTree() {}
    SegTree(int _n) : n(_n) { tr.assign(n * 4 + 5, E); }
    SegTree(const vector<Node>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        build(1, 0, n - 1, a);
    }
    void pushup(int u) { tr[u] = merge(tr[u << 1], tr[u << 1 | 1]); }
    void build(int u, int l, int r, const vector<Node>& a) {
        if (l == r) return leaf(u, l, a[l]);
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void set(int u, int l, int r, int p, Node v) {
        if (l == r) return leaf(u, p, v);
        int mid = (l + r) >> 1;
        if (p <= mid) set(u << 1, l, mid, p, v);
        else set(u << 1 | 1, mid + 1, r, p, v);
        pushup(u);
    } void set(int p, Node v) { set(1, 0, n - 1, p, v); }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    } Node query(int l, int r) { return query(1, 0, n - 1, l, r); }
    T calc(int l, int r, T x) {
        Node f = query(l, r);
        return (1LL * f.a * x + f.b) % mod;
    }
};