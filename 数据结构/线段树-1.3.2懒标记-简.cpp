#include <bits/stdc++.h>
using namespace std;

// 前置条件：构造数组非空；修改与查询区间均位于 [0,n-1] 且非空
template<class T>
struct LazySegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum, mx, mn;
    };
    int n;
    Node E = {0, -INF, INF};
    vector<Node> tr;
    vector<T> add_tag;
    LazySegTree() {}
    LazySegTree(int _n) : n(_n) {
        tr.assign(n * 4 + 5, {0, 0, 0});
        add_tag.assign(n * 4 + 5, 0);
    }
    LazySegTree(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        add_tag.assign(n * 4 + 5, 0);
        build(1, 0, n - 1, a);
    }
    Node merge(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);
        return res;
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void apply_add(int u, int l, int r, T v) {
        tr[u].sum += v * (r - l + 1);
        tr[u].mx += v;
        tr[u].mn += v;
        add_tag[u] += v;
    }
    void pushdown(int u, int l, int r) {
        int mid = (l + r) >> 1;
        if (add_tag[u]) {
            apply_add(u << 1, l, mid, add_tag[u]);
            apply_add(u << 1 | 1, mid + 1, r, add_tag[u]);
            add_tag[u] = 0;
        }
    }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) {
            tr[u] = {a[l], a[l], a[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void add(int u, int l, int r, int ql, int qr, T v) {
        if (ql <= l && r <= qr) {
            apply_add(u, l, r, v);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
        if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u);
    }
    void add(int l, int r, T v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        add(1, 0, n - 1, l, r, v);
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
