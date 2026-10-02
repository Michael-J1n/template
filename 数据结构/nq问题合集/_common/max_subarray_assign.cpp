#include <bits/stdc++.h>
using namespace std;

/*
RangeAssignMaxSubarray：区间赋值 + 区间最大子段和，0-based，闭区间 [l,r]。

约定：最大子段和为非空子段。
复杂度：build O(n)，修改/查询 O(logn)。
*/

struct RangeAssignMaxSubarray {
    using ll = long long;
    static constexpr ll NEG = LLONG_MIN / 4;
    struct Node {
        ll sum, pref, suff, best;
        int len;
    };
    int n;
    vector<Node> tr;
    vector<ll> setTag;
    vector<char> hasSet;
    RangeAssignMaxSubarray() {}
    RangeAssignMaxSubarray(const vector<ll>& a) {
        init(a);
    }
    Node empty() {
        return {0, NEG, NEG, NEG, 0};
    }
    Node single(ll v) {
        return {v, v, v, v, 1};
    }
    Node same(ll v, int len) {
        ll sum = v * len;
        ll best = v >= 0 ? sum : v;
        return {sum, best, best, best, len};
    }
    Node merge(const Node& a, const Node& b) {
        if (!a.len) return b;
        if (!b.len) return a;
        Node c;
        c.len = a.len + b.len;
        c.sum = a.sum + b.sum;
        c.pref = max(a.pref, a.sum + b.pref);
        c.suff = max(b.suff, b.sum + a.suff);
        c.best = max(max(a.best, b.best), a.suff + b.pref);
        return c;
    }
    void init(const vector<ll>& a) {
        n = a.size();
        tr.assign(n * 4 + 5, empty());
        setTag.assign(n * 4 + 5, 0);
        hasSet.assign(n * 4 + 5, 0);
        if (!n) return;
        build(1, 0, n - 1, a);
    }
    void apply_set(int u, int l, int r, ll v) {
        tr[u] = same(v, r - l + 1);
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
            tr[u] = single(a[l]);
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
        if (l > r) return empty();
        return query(1, 0, n - 1, l, r);
    }
    ll range_max_subarray(int l, int r) {
        return query(l, r).best;
    }
};
