#include <bits/stdc++.h>
using namespace std;

/**
 * 双模随机动态字符串哈希【单点修改 + 子串判等】
 * DynamicStringHash(s) 用线段树维护字符串 s，set(p, c) 将 s[p] 修改为字符 c，get(l, r) 返回闭区间 [l, r] 的双哈希值。
 * 所有实例共享同一个随机 base，可直接比较不同字符串中等长子串的哈希值。
 *
 * 适用：字符串存在单点修改，同时需要频繁进行子串判等、回文判断、LCP 等哈希查询。
 *
 * 复杂度：预处理 O(n)，单点修改 O(log n)，单次 get O(log n)
 */
struct DynamicStringHash {
    static const int mod1 = 1000000007;
    static const int mod2 = 1000000009;
    inline static vector<int> p1 = {1};
    inline static vector<int> p2 = {1};
    static int _get() {
        static const int base = []() {
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            return uniform_int_distribution<int>(256, mod1 - 1)(rng);
        }();
        return base;
    }
    static void ensure(int n) {
        int base = _get();
        while (p1.size() <= n) {
            p1.emplace_back(1LL * p1.back() * base % mod1);
            p2.emplace_back(1LL * p2.back() * base % mod2);
        }
    }
    struct Node {
        int h1, h2;
        int len;
    };
    int n;
    vector<Node> tr;
    Node merge(const Node& a, const Node& b) const {
        Node res;
        res.h1 = (1LL * a.h1 * p1[b.len] + b.h1) % mod1;
        res.h2 = (1LL * a.h2 * p2[b.len] + b.h2) % mod2;
        res.len = a.len + b.len;
        return res;
    }
    DynamicStringHash(const string& s) {
        n = s.size();
        ensure(n);
        tr.resize(n * 4 + 5);
        build(1, 0, n - 1, s);
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void build(int u, int l, int r, const string& s) {
        if (l == r) {
            int x = (unsigned char)s[l] + 1;
            tr[u] = {x, x, 1};
            return;
        }
        int mid = l + r >> 1;
        build(u << 1, l, mid, s);
        build(u << 1 | 1, mid + 1, r, s);
        pushup(u);
    }
    void set(int u, int l, int r, int p, char c) {
        if (l == r) {
            int x = (unsigned char)c + 1;
            tr[u] = {x, x, 1};
            return;
        }
        int mid = l + r >> 1;
        if (p <= mid) set(u << 1, l, mid, p, c);
        else set(u << 1 | 1, mid + 1, r, p, c);
        pushup(u);
    }
    void set(int p, char c) { set(1, 0, n - 1, p, c); }
    Node query(int u, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tr[u];
        int mid = l + r >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    pair<int, int> get(int l, int r) const {
        Node res = query(1, 0, n - 1, l, r);
        return {res.h1, res.h2};
    }
};