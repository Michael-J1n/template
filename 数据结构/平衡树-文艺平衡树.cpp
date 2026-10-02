#include <bits/stdc++.h>
using namespace std;

/**
 * 文艺平衡树
 *
 * 功能：
 * 支持维护一个动态序列，按位置 split / merge
 * build(a)：用数组 a 建树
 * insert(p, x)：在位置 p 前插入 x
 * erase(p)：删除位置 p 的元素
 * range_reverse(l, r)：翻转区间 [l, r]
 * get(id)：返回下标为 id 的元素
 * to_vec()：返回整段序列
 *
 * 复杂度：
 * 所有修改和单点查询期望 O(log n)
 */
template<class T>
struct ImplicitTreap {
    struct Node {
        int l, r;
        T v;
        uint32_t pri;
        int sz;
        bool rev;
    };
    vector<Node> t;
    vector<int> free_nodes;
    int rt;
    uint64_t seed;
    ImplicitTreap(int cap = 0) : rt(0), seed(88172645463325252ull) {
        t.resize(1);
        if (cap) t.reserve(cap + 5);
    }
    ImplicitTreap(vector<T>& a) : rt(0), seed(88172645463325252ull) {
        t.resize(1);
        t.reserve(a.size() + 5);
        build(a);
    }
    uint32_t rng() {
        seed += 0x9e3779b97f4a7c15ull;
        uint64_t z = seed;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
        z ^= z >> 31;
        return z;
    }
    int sz(int u) {
        return u ? t[u].sz : 0;
    }
    void pull(int u) {
        t[u].sz = sz(t[u].l) + sz(t[u].r) + 1;
    }
    void tag_rev(int u) {
        if (!u) return;
        swap(t[u].l, t[u].r);
        t[u].rev ^= 1;
    }
    void push(int u) {
        if (!t[u].rev) return;
        tag_rev(t[u].l);
        tag_rev(t[u].r);
        t[u].rev = 0;
    }
    int new_node(T x) {
        if (!free_nodes.empty()) {
            int u = free_nodes.back();
            free_nodes.pop_back();
            t[u] = {0, 0, x, rng(), 1, 0};
            return u;
        }
        t.push_back({0, 0, x, rng(), 1, 0});
        return t.size() - 1;
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pri < t[b].pri) {
            push(a);
            t[a].r = merge(t[a].r, b);
            pull(a);
            return a;
        }
        push(b);
        t[b].l = merge(a, t[b].l);
        pull(b);
        return b;
    }
    void split(int u, int k, int& x, int& y) {
        if (!u) {
            x = y = 0;
            return;
        }
        push(u);
        if (sz(t[u].l) >= k) {
            y = u;
            split(t[u].l, k, x, t[y].l);
            pull(y);
        } else {
            x = u;
            split(t[u].r, k - sz(t[u].l) - 1, t[x].r, y);
            pull(x);
        }
    }
    void build(vector<T>& a) {
        clear();
        for (auto& x : a) rt = merge(rt, new_node(x));
    }
    void insert(int p, T x) {
        int a, b;
        split(rt, p, a, b);
        rt = merge(merge(a, new_node(x)), b);
    }
    void erase(int p) {
        int a, b, c;
        split(rt, p, a, b);
        split(b, 1, b, c);
        rt = merge(a, c);
        t[b] = {};
        free_nodes.emplace_back(b);
    }
    void range_reverse(int l, int r) {
        if (r < l) return;
        int a, b, c;
        split(rt, r + 1, a, c);
        split(a, l, a, b);
        tag_rev(b);
        rt = merge(merge(a, b), c);
    }
    int size() {
        return sz(rt);
    }
    void clear() {
        t.resize(1), free_nodes.clear(), rt = 0;
    }
    T get(int k) {
        int u = rt;
        while (u) {
            push(u);
            int ls = sz(t[u].l);
            if (k < ls) u = t[u].l;
            else if (k == ls) return t[u].v;
            else k -= ls + 1, u = t[u].r;
        }
        return T();
    }
    void dfs(int u, vector<T>& res) {
        if (!u) return;
        push(u);
        dfs(t[u].l, res);
        res.emplace_back(t[u].v);
        dfs(t[u].r, res);
    }
    vector<T> to_vec() {
        vector<T> res;
        res.reserve(size());
        dfs(rt, res);
        return res;
    }
};
