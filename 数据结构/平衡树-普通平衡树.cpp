#include <bits/stdc++.h>
using namespace std;

/**
 * Treap 平衡树模板
 *
 * 功能：
 * 支持 multiset 语义的动态有序集合
 * insert(x)：插入一个 x
 * erase_one(x)：删除一个 x，若有多个只删一个
 * erase_all(x)：删除所有 x
 * clear()：清空整棵树
 * size()：返回元素总数，包含重复元素
 * countt(x)：返回 x 的出现次数
 * lt_cnt(x): 返回严格 <x 的元素出现次数  ==> ge_cnt = size - lt_cnt
 * le_cnt(x): 返回 <=x 的元素出现次数    ==> gt_cnt = size - le_cnt
 * cnt_range(l, r)：返回 [l, r] 内元素个数
 * rank(x)：返回 x 的排名，1-based，等于 1 + #(< x)
 * kth(k)：返回第 k 小，1-based，要求 1 <= k <= size()
 * min_val()：返回最小值，要求非空
 * max_val()：返回最大值，要求非空
 * lower_bound(x)：返回最小 >= x 的值，不存在返回 numeric_limits<T>::max()
 * upper_bound(x)：返回最小 > x 的值，不存在返回 numeric_limits<T>::max()
 * pre_val(x)：返回最大 < x 的值，不存在返回 numeric_limits<T>::lowest()
 * nxt_val(x)：返回最小 > x 的值，不存在返回 numeric_limits<T>::max()
 *
 * 复杂度：
 * 所有查询和修改期望 O(log n)
 * size O(1)，clear O(1)
 */ 
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
    Treap(int cap = 0) : rt(0), bin(0) { if (cap) t.reserve(cap + 1); }
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