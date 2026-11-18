#include <bits/stdc++.h>
using namespace std;

template<class T>
struct Treap {
    struct Node {
        int l, r;
        T v;
        uint32_t pri;
        int cnt, sz;
    };
    inline static uint64_t seed = 88172645463325252ull;
    vector<Node> t;
    int rt, bin;
    Treap(int cap = 0) : rt(0), bin(0) {
        if (cap) t.reserve(cap + 1);
    }
    uint32_t rng() {
        seed += 0x9e3779b97f4a7c15ull;
        uint64_t z = seed;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
        z ^= z >> 31;
        return z;
    }
    int sz(int u) const { return u ? t[u].sz : 0; }
    void pull(int u) { t[u].sz = sz(t[u].l) + sz(t[u].r) + t[u].cnt; }
    int new_node(T x) {
        if (t.empty()) t.push_back({});
        if (bin) {
            int u = bin;
            bin = t[u].l;
            t[u] = {0, 0, x, rng(), 1, 1};
            return u;
        }
        t.push_back({0, 0, x, rng(), 1, 1});
        return t.size() - 1;
    }
    void recycle(int u) {
        t[u].l = bin;
        bin = u;
    }
    int rot_l(int u) {
        int p = t[u].r;
        t[u].r = t[p].l, t[p].l = u;
        pull(u), pull(p);
        return p;
    }
    int rot_r(int u) {
        int p = t[u].l;
        t[u].l = t[p].r, t[p].r = u;
        pull(u), pull(p);
        return p;
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pri < t[b].pri) {
            t[a].r = merge(t[a].r, b);
            pull(a);
            return a;
        }
        t[b].l = merge(a, t[b].l);
        pull(b);
        return b;
    }
    int _insert(int u, T x) {
        if (!u) return new_node(x);
        if (t[u].v == x) {
            ++t[u].cnt;
            pull(u);
            return u;
        }
        if (x < t[u].v) {
            t[u].l = _insert(t[u].l, x);
            if (t[t[u].l].pri < t[u].pri) u = rot_r(u);
        } else {
            t[u].r = _insert(t[u].r, x);
            if (t[t[u].r].pri < t[u].pri) u = rot_l(u);
        }
        pull(u);
        return u;
    }
    void _erase_one(int& u, T x) {
        if (!u) return;
        if (t[u].v == x) {
            if (t[u].cnt > 1) {
                --t[u].cnt;
                pull(u);
                return;
            }
            if (!t[u].l || !t[u].r) {
                int old = u;
                u = t[u].l ? t[u].l : t[u].r;
                recycle(old);
                return;
            }
            if (t[t[u].l].pri < t[t[u].r].pri) {
                u = rot_r(u);
                _erase_one(t[u].r, x);
            } else {
                u = rot_l(u);
                _erase_one(t[u].l, x);
            }
            pull(u);
            return;
        }
        if (x < t[u].v) _erase_one(t[u].l, x);
        else _erase_one(t[u].r, x);
        pull(u);
    }
    void _erase_all(int& u, T x) {
        if (!u) return;
        if (t[u].v == x) {
            int old = u;
            u = merge(t[u].l, t[u].r);
            recycle(old);
            return;
        }
        if (x < t[u].v) _erase_all(t[u].l, x);
        else _erase_all(t[u].r, x);
        pull(u);
    }
    int calc_sz(int u) {
        if (!u) return 0;
        t[u].sz = calc_sz(t[u].l) + calc_sz(t[u].r) + t[u].cnt;
        return t[u].sz;
    }
    void build_sorted(const vector<T>& a) {
        t.clear(); t.reserve(a.size() + 1); t.push_back({});
        rt = 0, bin = 0;
        int top = 0;
        for (int i = 0; i < a.size();) {
            int j = i + 1;
            while (j < a.size() && a[j] == a[i]) ++j;
            int u = new_node(a[i]);
            t[u].cnt = j - i;
            int last = 0;
            while (top && t[top].pri > t[u].pri) {
                last = top;
                top = t[top].sz;
            }
            t[u].l = last;
            if (top) t[top].r = u;
            t[u].sz = top;
            top = u;
            i = j;
        }
        if (!top) return;
        rt = top;
        while (t[rt].sz) rt = t[rt].sz;
        calc_sz(rt);
    }
    void insert(T x) { rt = _insert(rt, x); }
    void erase_one(T x) { _erase_one(rt, x); }
    void erase_all(T x) { _erase_all(rt, x); }
    void clear() {
        t.clear();
        rt = 0, bin = 0;
    }
    int size() const { return sz(rt); }
    int count(T x) const {
        int u = rt;
        while (u) {
            if (t[u].v == x) return t[u].cnt;
            if (x < t[u].v) u = t[u].l;
            else u = t[u].r;
        }
        return 0;
    }
    int lt_cnt(T x) const {
        int u = rt, res = 0;
        while (u) {
            if (x <= t[u].v) u = t[u].l;
            else {
                res += sz(t[u].l) + t[u].cnt;
                u = t[u].r;
            }
        }
        return res;
    }
    int le_cnt(T x) const {
        int u = rt, res = 0;
        while (u) {
            if (x < t[u].v) u = t[u].l;
            else {
                res += sz(t[u].l) + t[u].cnt;
                u = t[u].r;
            }
        }
        return res;
    }
    int cnt_range(T l, T r) const {
        if (r < l) return 0;
        return le_cnt(r) - lt_cnt(l);
    }
    int rank(T x) const { return lt_cnt(x) + 1; }
    T kth(int k) const {
        int u = rt;
        while (u) {
            int ls = sz(t[u].l);
            if (k <= ls) u = t[u].l;
            else if (k <= ls + t[u].cnt) return t[u].v;
            else {
                k -= ls + t[u].cnt;
                u = t[u].r;
            }
        }
        return T();
    }
    T min_val() const {
        int u = rt;
        while (t[u].l) u = t[u].l;
        return t[u].v;
    }
    T max_val() const {
        int u = rt;
        while (t[u].r) u = t[u].r;
        return t[u].v;
    }
    T lower_bound(T x) const {
        int u = rt;
        T res = numeric_limits<T>::max();
        while (u) {
            if (x <= t[u].v) {
                res = t[u].v;
                u = t[u].l;
            } else {
                u = t[u].r;
            }
        }
        return res;
    }
    T upper_bound(T x) const {
        int u = rt;
        T res = numeric_limits<T>::max();
        while (u) {
            if (x < t[u].v) {
                res = t[u].v;
                u = t[u].l;
            } else {
                u = t[u].r;
            }
        }
        return res;
    }
    T pre_val(T x) const {
        int u = rt;
        T res = numeric_limits<T>::lowest();
        while (u) {
            if (t[u].v < x) {
                res = t[u].v;
                u = t[u].r;
            } else {
                u = t[u].l;
            }
        }
        return res;
    }
    T nxt_val(T x) const { return upper_bound(x); }
};

/**
 * 线段树套 Treap / 二逼平衡树
 * 维护数组 a，支持单点修改和**区间值域查询**（可同时见动态小波矩阵，有更省空间的做法）
 * 
 * 功能：
 * get_node(i)：数组第 i 个位置对应的 Treap 在线段树中的下标
 * get_nodes(l, r): 返回 [l, r] 区间对应的 logn 个 Treap 在线段树中的下标
 * set(p, x)：令 a[p] = x
 * lt_cnt(l, r, x)：返回 [l, r] 中严格 <x 的元素个数
 * le_cnt(l, r, x)：返回 [l, r] 中 <=x 的元素个数
 * cnt_range(l, r, x, y)：返回 [l, r] 中值属于 [x, y] 的元素个数
 * rank(l, r, x)：返回 x 在 [l, r] 中的排名，1-based
 * pre_val(l, r, x)：返回 [l, r] 中最大 <x 的值
 * lower_bound(l, r, x)：返回 [l, r] 中最小 >=x 的值
 * upper_bound(l, r, x)：返回 [l, r] 中最小 >x 的值
 * nxt_val(l, r, x)：等价于 upper_bound
 * kth(l, r, k)：返回 [l, r] 中第 k 小，1-based
 *
 * 复杂度：
 * 建树 O(n log n)，空间 O(n log n)
 * set / 普通查询期望 O(log^2 n)；kth 期望 O(log^3 n)
 * 前置条件：构造数组非空；修改下标与查询区间合法且区间非空；kth 的 k 位于 [1,r-l+1]
 */
template<class T>
struct SegTreap {
    static constexpr T INF = numeric_limits<T>::max();
    static constexpr T NINF = numeric_limits<T>::lowest();
    int n;
    vector<T> a;
    vector<Treap<T>> tr;
    SegTreap() {}
    SegTreap(int _n) : n(_n), a(n, T()) {
        tr.resize(n * 4 + 5);
        build(1, 0, n - 1);
    }
    SegTreap(const vector<T>& _a) : n(_a.size()), a(_a) {
        tr.resize(n * 4 + 5);
        build(1, 0, n - 1);
    }
    vector<T> build(int u, int l, int r) {
        if (l == r) {
            vector<T> res = {a[l]};
            tr[u].build_sorted(res);
            return res;
        }
        int mid = l + r >> 1;
        vector<T> x = build(u << 1, l, mid), y = build(u << 1 | 1, mid + 1, r);
        vector<T> res;
        res.reserve(x.size() + y.size());
        merge(x.begin(), x.end(), y.begin(), y.end(), back_inserter(res));
        tr[u].build_sorted(res);
        return res;
    }
    void set(int u, int l, int r, int p, T old, T x) {
        tr[u].erase_one(old);
        tr[u].insert(x);
        if (l == r) return;
        int mid = l + r >> 1;
        if (p <= mid) set(u << 1, l, mid, p, old, x);
        else set(u << 1 | 1, mid + 1, r, p, old, x);
    }
    void set(int p, T x) {
        if (a[p] == x) return;
        T old = a[p];
        a[p] = x;
        set(1, 0, n - 1, p, old, x);
    }
    int get_node(int u, int l, int r, int p) const {
        if (l == r) return u;
        int mid = l + r >> 1;
        if (p <= mid) return get_node(u << 1, l, mid, p);
        return get_node(u << 1 | 1, mid + 1, r, p);
    }
    int get_node(int p) const { return get_node(1, 0, n - 1, p); }
    void get_nodes(int u, int l, int r, int ql, int qr, array<int, 64>& ids, int& m) const {
        if (ql <= l && r <= qr) {
            ids[m++] = u;
            return;
        }
        int mid = l + r >> 1;
        if (ql <= mid) get_nodes(u << 1, l, mid, ql, qr, ids, m);
        if (qr > mid) get_nodes(u << 1 | 1, mid + 1, r, ql, qr, ids, m);
    }
    /** 使用：auto nodes = get_nodes(l, r); for (auto& u : nodes) tr[u] */
    vector<int> get_nodes(int l, int r) const {
        array<int, 64> ids;
        int m = 0;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        return vector<int>(ids.begin(), ids.begin() + m);
    }
    int lt_cnt(int l, int r, T x) const {
        array<int, 64> ids;
        int m = 0, res = 0;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        for (int i = 0; i < m; ++i) res += tr[ids[i]].lt_cnt(x);
        return res;
    }
    int le_cnt(int l, int r, T x) const {
        array<int, 64> ids;
        int m = 0, res = 0;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        for (int i = 0; i < m; ++i) res += tr[ids[i]].le_cnt(x);
        return res;
    }
    int cnt_range(int l, int r, T x, T y) const {
        if (y < x) return 0;
        array<int, 64> ids;
        int m = 0, res = 0;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        for (int i = 0; i < m; ++i) res += tr[ids[i]].cnt_range(x, y);
        return res;
    }
    int rank(int l, int r, T x) const { return lt_cnt(l, r, x) + 1; }
    T pre_val(int l, int r, T x) const {
        array<int, 64> ids;
        int m = 0;
        T res = NINF;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        for (int i = 0; i < m; ++i) res = max(res, tr[ids[i]].pre_val(x));
        return res;
    }
    T lower_bound(int l, int r, T x) const {
        array<int, 64> ids;
        int m = 0;
        T res = INF;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        for (int i = 0; i < m; ++i) res = min(res, tr[ids[i]].lower_bound(x));
        return res;
    }
    T upper_bound(int l, int r, T x) const {
        array<int, 64> ids;
        int m = 0;
        T res = INF;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        for (int i = 0; i < m; ++i) res = min(res, tr[ids[i]].upper_bound(x));
        return res;
    }
    T nxt_val(int l, int r, T x) const { return upper_bound(l, r, x); }
    T kth(int l, int r, int k) const {
        array<int, 64> ids;
        int m = 0;
        get_nodes(1, 0, n - 1, l, r, ids, m);
        int lo = 1, hi = n;
        while (lo < hi) {
            int mid = (lo + hi) >> 1;
            T x = tr[1].kth(mid);
            int cnt = 0;
            for (int i = 0; i < m; ++i) cnt += tr[ids[i]].le_cnt(x);
            if (cnt >= k) hi = mid;
            else lo = mid + 1;
        }
        return tr[1].kth(lo);
    }
};
