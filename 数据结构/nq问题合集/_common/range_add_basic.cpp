#include <bits/stdc++.h>
using namespace std;

/*
RangeAddBasic：区间加 + 区间 sum/max/min/gcd，0-based，闭区间 [l,r]。

sum/max/min：普通 lazy 线段树。
gcd：差分维护。若 d[0]=a[0], d[i]=a[i]-a[i-1]，
则 gcd(a[l..r]) = gcd(abs(a[l]), abs(d[l+1]), ..., abs(d[r]))。

复杂度：build O(n)，range_add/query O(logn)。
*/

struct RangeAddBasic {
    using ll = long long;
    static constexpr ll INF = LLONG_MAX / 2;
    struct Node {
        ll sum, mx, mn, g;
    };
    struct LazyInfo {
        struct Info {
            ll sum, mx, mn;
        };
        int n;
        vector<Info> tr;
        vector<ll> tag;
        LazyInfo() {}
        LazyInfo(const vector<ll>& a) { init(a); }
        Info merge(const Info& a, const Info& b) {
            return {a.sum + b.sum, max(a.mx, b.mx), min(a.mn, b.mn)};
        }
        void init(const vector<ll>& a) {
            n = a.size();
            tr.assign(n * 4 + 5, {0, -INF, INF});
            tag.assign(n * 4 + 5, 0);
            if (!n) return;
            build(1, 0, n - 1, a);
        }
        void apply(int u, int l, int r, ll v) {
            tr[u].sum += v * (r - l + 1);
            tr[u].mx += v;
            tr[u].mn += v;
            tag[u] += v;
        }
        void pushdown(int u, int l, int r) {
            if (!tag[u] || l == r) return;
            int mid = (l + r) >> 1;
            apply(u << 1, l, mid, tag[u]);
            apply(u << 1 | 1, mid + 1, r, tag[u]);
            tag[u] = 0;
        }
        void build(int u, int l, int r, const vector<ll>& a) {
            if (l == r) {
                tr[u] = {a[l], a[l], a[l]};
                return;
            }
            int mid = (l + r) >> 1;
            build(u << 1, l, mid, a);
            build(u << 1 | 1, mid + 1, r, a);
            tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
        }
        void add(int u, int l, int r, int ql, int qr, ll v) {
            if (ql <= l && r <= qr) {
                apply(u, l, r, v);
                return;
            }
            pushdown(u, l, r);
            int mid = (l + r) >> 1;
            if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
            if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
            tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
        }
        void add(int l, int r, ll v) {
            l = max(l, 0), r = min(r, n - 1);
            if (l > r) return;
            add(1, 0, n - 1, l, r, v);
        }
        Info query(int u, int l, int r, int ql, int qr) {
            if (ql <= l && r <= qr) return tr[u];
            pushdown(u, l, r);
            int mid = (l + r) >> 1;
            if (qr <= mid) return query(u << 1, l, mid, ql, qr);
            if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
            return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
        }
        Info query(int l, int r) {
            l = max(l, 0), r = min(r, n - 1);
            if (l > r) return {0, -INF, INF};
            return query(1, 0, n - 1, l, r);
        }
    };

    struct BIT {
        int n;
        vector<ll> c;
        BIT() {}
        BIT(int _n) { init(_n); }
        void init(int _n) {
            n = _n;
            c.assign(n + 1, 0);
        }
        void build(const vector<ll>& a) {
            n = a.size();
            c.assign(n + 1, 0);
            for (int i = 1; i <= n; ++i) c[i] += a[i - 1];
            for (int i = 1; i <= n; ++i) {
                int j = i + (i & -i);
                if (j <= n) c[j] += c[i];
            }
        }
        void add(int p, ll v) {
            for (++p; p <= n; p += p & -p) c[p] += v;
        }
        ll pres(int p) const {
            ll s = 0;
            for (++p; p > 0; p &= p - 1) s += c[p];
            return s;
        }
    };

    struct SegGcd {
        int n, sz;
        vector<ll> tr;
        SegGcd() {}
        SegGcd(const vector<ll>& a) { init(a); }
        void init(const vector<ll>& a) {
            n = a.size();
            sz = 1;
            while (sz < n) sz <<= 1;
            tr.assign(sz << 1, 0);
            for (int i = 0; i < n; ++i) tr[sz + i] = llabs(a[i]);
            for (int i = sz - 1; i; --i) tr[i] = std::gcd(tr[i << 1], tr[i << 1 | 1]);
        }
        void set(int p, ll v) {
            p += sz;
            tr[p] = llabs(v);
            for (p >>= 1; p; p >>= 1) tr[p] = std::gcd(tr[p << 1], tr[p << 1 | 1]);
        }
        ll query(int l, int r) const {
            l = max(l, 0), r = min(r, n - 1);
            if (l > r) return 0;
            ll res = 0;
            for (l += sz, r += sz; l <= r; l >>= 1, r >>= 1) {
                if (l & 1) res = std::gcd(res, tr[l++]);
                if (!(r & 1)) res = std::gcd(res, tr[r--]);
            }
            return res;
        }
    };

    int n;
    LazyInfo info;
    BIT bit;
    SegGcd segGcd;
    vector<ll> d;

    RangeAddBasic() {}
    RangeAddBasic(const vector<ll>& a) {
        init(a);
    }
    void init(const vector<ll>& a) {
        n = a.size();
        info.init(a);
        d.assign(n, 0);
        for (int i = 0; i < n; ++i) d[i] = (i == 0 ? a[i] : a[i] - a[i - 1]);
        bit.build(d);
        segGcd.init(d);
    }
    void add_diff(int p, ll v) {
        if (p < 0 || p >= n) return;
        d[p] += v;
        bit.add(p, v);
        segGcd.set(p, d[p]);
    }
    void add(int l, int r, ll v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r || !v) return;
        info.add(l, r, v);
        add_diff(l, v);
        add_diff(r + 1, -v);
    }
    ll get(int p) const {
        if (p < 0 || p >= n) return 0;
        return bit.pres(p);
    }
    ll range_gcd(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        ll res = llabs(get(l));
        if (l + 1 <= r) res = std::gcd(res, segGcd.query(l + 1, r));
        return res;
    }
    Node query(int l, int r) {
        auto cur = info.query(l, r);
        return {cur.sum, cur.mx, cur.mn, range_gcd(l, r)};
    }
    ll range_sum(int l, int r) { return info.query(l, r).sum; }
    ll range_max(int l, int r) { return info.query(l, r).mx; }
    ll range_min(int l, int r) { return info.query(l, r).mn; }
};
