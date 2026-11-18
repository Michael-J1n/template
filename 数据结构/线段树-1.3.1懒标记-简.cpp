#include <bits/stdc++.h>
using namespace std;

// 前置条件：构造数组非空；修改与查询区间均位于 [0,n-1] 且非空
// 以区间加为例，节点维护区间和与最值
template<class T>
struct LazySegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    // -----------------------------修改开始----------------------------------
    struct Node {
        T sum, mx, mn;
    };
    Node E = {0, -INF, INF};
    Node Z = {0, 0, 0};
    Node merge(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);
        return res;
    }
    void leaf(int u, int p, T v) {
        Node& cur = tr[u];
        cur.sum = v;
        cur.mx = v;
        cur.mn = v;
    }
    // 按需要修改懒标记；记得需要同时更新 节点统计量 与 节点上存放的标记
    vector<T> tag;
    T tag_e = 0;
    void apply(int u, int l, int r, T v) {
        Node& cur = tr[u];
        cur.sum += v * (r - l + 1);
        cur.mx += v;
        cur.mn += v;
        tag[u] += v;
    }
    // -----------------------------修改结束----------------------------------
    int n;
    vector<Node> tr;
    LazySegTree() {}
    LazySegTree(int _n) : n(_n) {
        tr.assign(n * 4 + 5, Z);
        tag.assign(n * 4 + 5, tag_e);
    }
    LazySegTree(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        tag.assign(n * 4 + 5, tag_e);
        build(1, 0, n - 1, a);
    }
    void pushup(int u) { tr[u] = merge(tr[u << 1], tr[u << 1 | 1]); }
    void pushdown(int u, int l, int r) {
        int mid = (l + r) >> 1;
        if (tag[u]) {
            apply(u << 1, l, mid, tag[u]);
            apply(u << 1 | 1, mid + 1, r, tag[u]);
            tag[u] = tag_e;
        }
    }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) return leaf(u, l, a[l]);
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void modify(int u, int l, int r, int ql, int qr, T v) {
        if (ql <= l && r <= qr) return apply(u, l, r, v);
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) modify(u << 1, l, mid, ql, qr, v);
        if (qr > mid) modify(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u);
    } 
    void modify(int l, int r, T v) { 
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        modify(1, 0, n - 1, l, r, v); 
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    } 
    Node query(int l, int r) { 
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return E;
        return query(1, 0, n - 1, l, r); 
    }
};
