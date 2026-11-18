#include <bits/stdc++.h>
using namespace std;

/**
 * 线段树；可修改：节点信息，合并函数，递归基情况
 */
template<class T>
struct SegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    // -----------------------------修改开始----------------------------------
    struct Node {
        T sum, mx, mn;
    };
    Node E = {0, -INF, INF};  // 单位元
    Node Z = {0, 0, 0};       // 空数组初始化
    Node merge(const Node& a, const Node& b) {  // 节点统计量根据左右孩子统计量合并更新
        Node res;
        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);
        return res;
    }
    void leaf(int u, int p, T v) {  // 递归到单点的 base case
        Node& cur = tr[u];
        cur.sum = v;
        cur.mn = v;
        cur.mx = v;
    }
    // -----------------------------修改结束----------------------------------
    int n;
    vector<Node> tr;
    SegTree() {}
    SegTree(int _n) : n(_n) { tr.assign(n * 4 + 5, Z); }
    SegTree(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        build(1, 0, n - 1, a);
    }
    void pushup(int u) { tr[u] = merge(tr[u << 1], tr[u << 1 | 1]); }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) return leaf(u, l, a[l]);
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void set(int u, int l, int r, int p, T v) {
        if (l == r) return leaf(u, p, v);
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
        if (l > r) return E;
        return query(1, 0, n - 1, l, r); 
    }
};