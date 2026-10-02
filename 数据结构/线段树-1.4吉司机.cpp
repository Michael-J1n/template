#include <bits/stdc++.h>
using namespace std;

/**
 * 吉司机线段树 / Segment Tree Beats
 *
 * 状态定义：
 * sum：区间和
 * mx, se, cmx：最大值、严格次大值、最大值个数
 * mn, se2, cmn：最小值、严格次小值、最小值个数
 * add：区间加懒标记
 *
 * 支持操作：
 * range_chmin(l, r, x)：a[i] = min(a[i], x)
 * range_chmax(l, r, x)：a[i] = max(a[i], x)
 * range_add(l, r, x)：a[i] += x
 *
 * 复杂度：
 * 单次操作均摊 O(logn)，常数较大
 * 前置条件：构造数组非空；修改与查询区间均位于 [0,n-1] 且非空
 */
template<class T>
struct SegBeats {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    static constexpr T NINF = numeric_limits<T>::lowest() / 2;
    struct Node {
        T sum, mx, se, mn, se2, add;
        int cmx, cmn;
    };
    int n;
    vector<Node> tr;
    SegBeats() {}
    SegBeats(const vector<T>& a) : n(a.size()) {
        tr.resize(n * 4 + 5);
        build(1, 0, n - 1, a);
    }
    Node merge(const Node& a, const Node& b) {
        Node c;
        c.sum = a.sum + b.sum;
        if (a.mx == b.mx) {
            c.mx = a.mx;
            c.cmx = a.cmx + b.cmx;
            c.se = max(a.se, b.se);
        } else if (a.mx > b.mx) {
            c.mx = a.mx;
            c.cmx = a.cmx;
            c.se = max(a.se, b.mx);
        } else {
            c.mx = b.mx;
            c.cmx = b.cmx;
            c.se = max(a.mx, b.se);
        }
        if (a.mn == b.mn) {
            c.mn = a.mn;
            c.cmn = a.cmn + b.cmn;
            c.se2 = min(a.se2, b.se2);
        } else if (a.mn < b.mn) {
            c.mn = a.mn;
            c.cmn = a.cmn;
            c.se2 = min(a.se2, b.mn);
        } else {
            c.mn = b.mn;
            c.cmn = b.cmn;
            c.se2 = min(a.mn, b.se2);
        }
        c.add = 0;
        return c;
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) {
            T x = a[l];
            tr[u] = {x, x, NINF, x, INF, 0, 1, 1};
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void apply_add(int u, int l, int r, T x) {
        tr[u].sum += (r - l + 1) * x;
        tr[u].mx += x;
        tr[u].mn += x;
        if (tr[u].se != NINF) tr[u].se += x;
        if (tr[u].se2 != INF) tr[u].se2 += x;
        tr[u].add += x;
    }
    void apply_chmin(int u, T x) {
        if (tr[u].mx <= x) return;
        tr[u].sum += (x - tr[u].mx) * tr[u].cmx;
        if (tr[u].mn == tr[u].mx) tr[u].mn = x;
        else if (tr[u].se2 == tr[u].mx) tr[u].se2 = x;
        tr[u].mx = x;
    }
    void apply_chmax(int u, T x) {
        if (tr[u].mn >= x) return;
        tr[u].sum += (x - tr[u].mn) * tr[u].cmn;
        if (tr[u].mx == tr[u].mn) tr[u].mx = x;
        else if (tr[u].se == tr[u].mn) tr[u].se = x;
        tr[u].mn = x;
    }
    void pushdown(int u, int l, int r) {
        int mid = (l + r) >> 1;
        if (tr[u].add) {
            apply_add(u << 1, l, mid, tr[u].add);
            apply_add(u << 1 | 1, mid + 1, r, tr[u].add);
            tr[u].add = 0;
        }
        if (tr[u << 1].mx > tr[u].mx) apply_chmin(u << 1, tr[u].mx);
        if (tr[u << 1 | 1].mx > tr[u].mx) apply_chmin(u << 1 | 1, tr[u].mx);
        if (tr[u << 1].mn < tr[u].mn) apply_chmax(u << 1, tr[u].mn);
        if (tr[u << 1 | 1].mn < tr[u].mn) apply_chmax(u << 1 | 1, tr[u].mn);
    }
    void range_add(int l, int r, T x) {
        range_add(1, 0, n - 1, l, r, x);
    }
    void range_add(int u, int l, int r, int ql, int qr, T x) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply_add(u, l, r, x);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        range_add(u << 1, l, mid, ql, qr, x);
        range_add(u << 1 | 1, mid + 1, r, ql, qr, x);
        pushup(u);
    }
    void range_chmin(int l, int r, T x) {
        range_chmin(1, 0, n - 1, l, r, x);
    }
    void range_chmin(int u, int l, int r, int ql, int qr, T x) {
        if (qr < l || r < ql || tr[u].mx <= x) return;
        if (ql <= l && r <= qr && tr[u].se < x) {
            apply_chmin(u, x);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        range_chmin(u << 1, l, mid, ql, qr, x);
        range_chmin(u << 1 | 1, mid + 1, r, ql, qr, x);
        pushup(u);
    }
    void range_chmax(int l, int r, T x) {
        range_chmax(1, 0, n - 1, l, r, x);
    }
    void range_chmax(int u, int l, int r, int ql, int qr, T x) {
        if (qr < l || r < ql || tr[u].mn >= x) return;
        if (ql <= l && r <= qr && tr[u].se2 > x) {
            apply_chmax(u, x);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        range_chmax(u << 1, l, mid, ql, qr, x);
        range_chmax(u << 1 | 1, mid + 1, r, ql, qr, x);
        pushup(u);
    }
    T query_sum(int l, int r) {
        return query_sum(1, 0, n - 1, l, r);
    }
    T query_sum(int u, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tr[u].sum;
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        return query_sum(u << 1, l, mid, ql, qr) + query_sum(u << 1 | 1, mid + 1, r, ql, qr);
    }
    T query_mx(int l, int r) {
        return query_mx(1, 0, n - 1, l, r);
    }
    T query_mx(int u, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return NINF;
        if (ql <= l && r <= qr) return tr[u].mx;
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        return max(query_mx(u << 1, l, mid, ql, qr), query_mx(u << 1 | 1, mid + 1, r, ql, qr));
    }
    T query_mn(int l, int r) {
        return query_mn(1, 0, n - 1, l, r);
    }
    T query_mn(int u, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return INF;
        if (ql <= l && r <= qr) return tr[u].mn;
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        return min(query_mn(u << 1, l, mid, ql, qr), query_mn(u << 1 | 1, mid + 1, r, ql, qr));
    }
};
