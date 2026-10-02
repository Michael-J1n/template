#include <bits/stdc++.h>
using namespace std;

/*
MaxSubarraySegTree：静态 / 单点修改 + 区间最大子段和，0-based，闭区间 [l,r]。

约定：最大子段和为非空子段。
修改：
1. set(p, v)：单点赋值。
2. add(p, v)：单点加。

复杂度：build O(n)，修改/查询 O(logn)。
*/

struct MaxSubarraySegTree {
    using ll = long long;
    static constexpr ll NEG = LLONG_MIN / 4;
    struct Node {
        ll sum, pref, suff, best;
        int len;
    };
    int n, sz;
    vector<ll> a;
    vector<Node> tr;
    MaxSubarraySegTree() {}
    MaxSubarraySegTree(const vector<ll>& a) {
        init(a);
    }
    Node empty() const {
        return {0, NEG, NEG, NEG, 0};
    }
    Node single(ll v) const {
        return {v, v, v, v, 1};
    }
    Node merge(const Node& a, const Node& b) const {
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
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        sz = 1;
        while (sz < max(1, n)) sz <<= 1;
        tr.assign(sz << 1, empty());
        for (int i = 0; i < n; ++i) tr[sz + i] = single(a[i]);
        for (int i = sz - 1; i; --i) tr[i] = merge(tr[i << 1], tr[i << 1 | 1]);
    }
    void set(int p, ll v) {
        if (p < 0 || p >= n) return;
        a[p] = v;
        int u = sz + p;
        tr[u] = single(v);
        for (u >>= 1; u; u >>= 1) tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    Node query(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return empty();
        Node L = empty(), R = empty();
        for (l += sz, r += sz; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) L = merge(L, tr[l++]);
            if (!(r & 1)) R = merge(tr[r--], R);
        }
        return merge(L, R);
    }
    ll get(int p) const {
        return 0 <= p && p < n ? a[p] : 0;
    }
    void add(int p, ll v) {
        if (p < 0 || p >= n || !v) return;
        set(p, a[p] + v);
    }
    ll range_max_subarray(int l, int r) const {
        return query(l, r).best;
    }
};
