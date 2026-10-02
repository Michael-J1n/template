#include <bits/stdc++.h>
using namespace std;

/*
RangeAssignBasic：区间赋值 + 区间 sum/max/min/gcd，0-based，闭区间 [l,r]。

修改：
1. set(l, r, v)：区间赋值。
2. set(p, v)：单点赋值。

复杂度：build O(n)，修改/查询 O(logn)。
*/

struct RangeAssignBasic {
    using ll = long long;
    static constexpr ll INF = LLONG_MAX / 2;
    struct Node {
        ll sum, mx, mn, g;
    };
    int n;
    vector<Node> tr;
    vector<ll> setTag;
    vector<char> hasSet;
    RangeAssignBasic() {}
    RangeAssignBasic(const vector<ll>& a) {
        init(a);
    }
    Node merge(const Node& a, const Node& b) {
        return {a.sum + b.sum, max(a.mx, b.mx), min(a.mn, b.mn), std::gcd(a.g, b.g)};
    }
    void init(const vector<ll>& a) {
        n = a.size();
        tr.assign(n * 4 + 5, {0, -INF, INF, 0});
        setTag.assign(n * 4 + 5, 0);
        hasSet.assign(n * 4 + 5, 0);
        if (!n) return;
        build(1, 0, n - 1, a);
    }
    void apply_set(int u, int l, int r, ll v) {
        tr[u] = {v * (r - l + 1), v, v, llabs(v)};
        setTag[u] = v;
        hasSet[u] = 1;
    }
    void pushdown(int u, int l, int r) {
        if (!hasSet[u] || l == r) return;
        int mid = (l + r) >> 1;
        apply_set(u << 1, l, mid, setTag[u]);
        apply_set(u << 1 | 1, mid + 1, r, setTag[u]);
        hasSet[u] = 0;
    }
    void build(int u, int l, int r, const vector<ll>& a) {
        if (l == r) {
            apply_set(u, l, r, a[l]);
            hasSet[u] = 0;
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void set(int u, int l, int r, int ql, int qr, ll v) {
        if (ql <= l && r <= qr) {
            apply_set(u, l, r, v);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) set(u << 1, l, mid, ql, qr, v);
        if (qr > mid) set(u << 1 | 1, mid + 1, r, ql, qr, v);
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void set(int l, int r, ll v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        set(1, 0, n - 1, l, r, v);
    }
    void set(int p, ll v) {
        set(p, p, v);
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
        if (l > r) return {0, -INF, INF, 0};
        return query(1, 0, n - 1, l, r);
    }
    ll get(int p) {
        if (p < 0 || p >= n) return 0;
        return query(p, p).sum;
    }
    ll range_sum(int l, int r) { return query(l, r).sum; }
    ll range_max(int l, int r) { return query(l, r).mx; }
    ll range_min(int l, int r) { return query(l, r).mn; }
    ll range_gcd(int l, int r) { return query(l, r).g; }
};
