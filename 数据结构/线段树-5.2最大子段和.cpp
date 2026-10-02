#include <bits/stdc++.h>
using namespace std;

/**
 * 维护区间最大子段之和
 * 前置条件：构造数组非空；修改下标与查询区间合法，查询区间非空
 */
template<class T>
struct SegTree {
    struct Node {
        T sum, lsum, rsum, mx;
    };
    int n;
    vector<Node> tr;
    SegTree(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, {0, 0, 0, INT_MIN});
        build(1, 0, n - 1, a);
    }
    Node merge(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.lsum = max(a.lsum, a.sum + b.lsum);
        res.rsum = max(b.rsum, a.rsum + b.sum);
        res.mx = max({a.mx, b.mx, a.rsum + b.lsum});
        return res;
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) {
            tr[u] = {a[l], a[l], a[l], a[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void set(int u, int l, int r, int p, T v) {
        if (l == r) {
            tr[u] = {v, v, v, v};
            return;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) set(u << 1, l, mid, p, v);
        else set(u << 1 | 1, mid + 1, r, p, v);
        pushup(u);
    }
    void set(int p, T v) {
        if (p < 0 || p >= n) return;
        set(1, 0, n - 1, p, v);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    Node query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        return query(1, 0, n - 1, l, r);
    }
};
