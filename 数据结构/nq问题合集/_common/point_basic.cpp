#include <bits/stdc++.h>
using namespace std;

/*
PointRangeBasic：单点修改 + 区间 sum/max/min/gcd，0-based，闭区间 [l,r]。

修改：
1. set(p, v)：a[p] = v。
2. add(p, v)：a[p] += v。

查询：
1. query(l, r)：返回 {sum,mx,mn,g}。
2. range_sum/range_max/range_min/range_gcd。

复杂度：build O(n)，修改/查询 O(logn)。
*/

struct PointRangeBasic {
    using ll = long long;
    static constexpr ll INF = LLONG_MAX / 2;
    struct Node {
        ll sum, mx, mn, g;
    };
    int n, sz;
    vector<ll> a;
    vector<Node> tr;
    PointRangeBasic() {}
    PointRangeBasic(const vector<ll>& a) {
        init(a);
    }
    Node empty() const {
        return {0, -INF, INF, 0};
    }
    Node single(ll v) const {
        return {v, v, v, llabs(v)};
    }
    Node merge(const Node& a, const Node& b) const {
        return {a.sum + b.sum, max(a.mx, b.mx), min(a.mn, b.mn), std::gcd(a.g, b.g)};
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
    ll range_sum(int l, int r) const { return query(l, r).sum; }
    ll range_max(int l, int r) const { return query(l, r).mx; }
    ll range_min(int l, int r) const { return query(l, r).mn; }
    ll range_gcd(int l, int r) const { return query(l, r).g; }
};
