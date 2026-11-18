#include <bits/stdc++.h>
using namespace std;

struct tr_node {
    long long l, r;
    mutable long long v;
    tr_node(long long l, long long r, long long v) : l(l), r(r), v(v) {}
    bool operator<(const tr_node& o) const { return l < o.l; }
};
struct ODT {
    long long n = 1;
    set<tr_node> tree;
    ODT(set<tr_node> st) : tree(st) { n = tree.rbegin()->r + 1; }
    ODT(const vector<long long>& a) : n(a.size() - 1) {
        for (int l = 0; l <= n; ) {
            int r = l;
            while (r + 1 <= n && a[r + 1] == a[l]) ++r;
            tree.emplace(l, r, a[l]);
            l = r + 1;
        }
        ++n;
    }
    auto split(long long pos) {
        if (pos >= n) return tree.end();
        auto it = tree.lower_bound(tr_node(pos, 0, 0));
        if (it != tree.end() && it->l == pos) return it;
        --it;
        long long l = it->l, r = it->r, v = it->v;
        tree.erase(it);
        tree.emplace(l, pos - 1, v);
        return tree.emplace(pos, r, v).first;
    }
    void split(int l, int r) {
        split(l);
        if (r + 1 < n) split(r + 1);
    }
    void assign(long long l, long long r, long long v) {
        auto end = split(r + 1), begin = split(l);
        tree.erase(begin, end);
        // merge_around(tree.emplace(l, r, v).first);  // merge_around 可选，不影响均摊复杂度
        tree.emplace(l, r, v);
    }
    template<class F>
    void for_each(long long l, long long r, F&& f) const {
        auto it = prev(tree.upper_bound(tr_node(l, 0, 0)));
        for (; it != tree.end() && it->l <= r; ++it) {
            f(max(l, it->l), min(r, it->r), it->v);
        }
    }
};
template<class T>
struct Fenwick {
    int n;
    vector<T> tr;
    Fenwick(int n) : n(n), tr(n + 1, 0) {}
    Fenwick(const vector<T>& a) : n(a.size()), tr(n + 1, 0) {
        for (int i = 0; i < n; ++i) {
            tr[i + 1] += a[i] - (i ? a[i - 1] : 0);
            int j = i + 1 + ((i + 1) & -(i + 1));
            if (j <= n) tr[j] = tr[j] + tr[i + 1];
        }
    }
    void add(int x, T v) { for (++x; x <= n; x += x & -x) tr[x] += v; }
    void update(int l, int r, T v) { add(l, v), add(r + 1, -v); }
    T query(int x) {
        T res = 0;
        for (++x; x > 0; x -= x & -x) res += tr[x];
        return res;
    }
};

void answer(const vector<long long>& a, vector<vector<int>>& queries) {
    long long stamp = ranges::max(a);
    ODT tr(a);                  // ODT 只维护连续颜色段边界
    Fenwick<long long> bit(a);  // 树状数组维护真实值
    for (auto& q : queries) {
        int op = q[0], l = q[1], r = q[2]; long long x = q[3];
        if (op == 1) {          // 区间 [l,r] 加 x；维护连续段边界并更新树状数组上的真实值
            tr.split(l, r);
            bit.update(l, r, x);
        } else {                // 遍历在 [l,r] 中的连续颜色段，并用 x 推平
            tr.for_each(l, r, [&](long long L, long long R, long long _) {
                long long v = bit.query(L);
                // 当前连续段 [L, R] 的值为 v，做统计
                cout << L << " " << R << " " << v << endl;
            });
            // 维护推平后的真实值和连续段边界
            tr.for_each(l, r, [&](long long L, long long R, long long _) {
                long long v = bit.query(L);
                bit.update(L, R, x - v);
            });
            tr.assign(l, r, ++stamp);
        }
    }
}