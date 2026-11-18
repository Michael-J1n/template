#include <bits/stdc++.h>
using namespace std;

/**
 * 势能线段树
 * 关键在于，维护区间的势能值并依此进行跳过
 * 
 * 常见势能操作：
 * 操作     	 skip(u, x)  	 op(v, x)
 * --------------------------------------------
 * 有效取模    tr[u].mx < x	       v % x
 * 整数开方	   tr[u].mx <= 1     floor(sqrt(v))
 * 数位和	   tr[u].mx < 10     digitSum(v)
 * 约数个数	   tr[u].mx <= 2	 divisorCount(v)
 * x /= k	  tr[u].mx == 0	      v / x
 * 
 * 实现细节：
 * 1. 有时候 sum 会维护取 mod 后的值，但势能操作要使用原值，这个时候可以把 modify 中的 op(tr[u].sum, x) 改成 op(tr[u].mx, x)
 * 2. 在 skip 的时候，要注意分析什么时候 tr[u] 代表的区间 (a[l],..a[r]) 在操作后每个值全都不变。如果剪枝条件较弱，复杂度会假。
 *    例如：区间每个数 x 改为 gcd(x, k)，不能简单的用 tr[u].mx <= 1 判断。分析若区间每个 a[i] 在 gcd(a[i],x) 后不变，说明 a[i] | x
 *    因此区间的 lcm 为 x 的因数，剪枝的条件应该用区间 lcm 来判断（实现时用上限截断 lcm，否则会溢出）
 * 
 * 复杂度：O((Q + n log A) log n)
 */
template<class T>
struct SegTree {
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
    bool skip(int u, T x) {  // 节点 u 是否可以被整段跳过（当前操作对节点 u 的所有元素均无效）
        return tr[u].mx < x;
    }
    T op(T v, T x) {         // 节点值为 v，对 v 进行势能操作（本例中为取 mod）
        return v % x;
    }
    // -----------------------------修改结束----------------------------------
    int n;
    vector<Node> tr;
    SegTree() : n(0) {}
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
    void modify(int u, int l, int r, int ql, int qr, T x) {
        if (skip(u, x)) return;
        if (l == r) return leaf(u, l, op(tr[u].sum, x));
        int mid = (l + r) >> 1;
        if (ql <= mid) modify(u << 1, l, mid, ql, qr, x);
        if (qr > mid) modify(u << 1 | 1, mid + 1, r, ql, qr, x);
        pushup(u);
    }
    void modify(int l, int r, T x) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        modify(1, 0, n - 1, l, r, x);
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