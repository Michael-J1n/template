#include <bits/stdc++.h>
using namespace std;

/**
 * 线段树套树状数组【动态二维点修改与矩形计数】
 * 将数组元素表示为二维点 (i, a[i])，点坐标固定，动态维护每个点的权值。
 * 下标 0-based；查询区间均为闭区间。
 *
 * 支持操作：
 * add(p, v)：点 (p, a[p]) 的权值增加 v；
 * query(l, r, vl, vr)：查询 l <= i <= r 且 vl <= a[i] <= vr 的点权和。
 *
 * 适用：动态逆序对、动态二维偏序、区间值域计数、元素动态加入与删除。
 *
 * 复杂度：
 * 建树 O(n log n)，修改查询均 O(log^2 n)；空间 O(n log n)
 */
struct SegBIT {
    int n;
    vector<int> a;
    vector<vector<int>> xs, bit;
    SegBIT(const vector<int>& a) : n(a.size()), a(a), xs(n << 2), bit(n << 2) {
        build(1, 0, n - 1);
    }
    void build(int u, int l, int r) {
        if (l == r) {
            xs[u] = {a[l]};
            bit[u].assign(2, 0);
            return;
        }
        int mid = l + r >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);
        merge(xs[u << 1].begin(), xs[u << 1].end(), xs[u << 1 | 1].begin(), xs[u << 1 | 1].end(), back_inserter(xs[u]));
        xs[u].erase(unique(xs[u].begin(), xs[u].end()), xs[u].end());
        bit[u].assign(xs[u].size() + 1, 0);
    }
    void bit_add(int u, int x, int v) {
        int p = lower_bound(xs[u].begin(), xs[u].end(), x) - xs[u].begin() + 1;
        for (int i = p; i < bit[u].size(); i += i & -i) bit[u][i] += v;
    }
    int bit_query(int u, int x) const {
        int p = upper_bound(xs[u].begin(), xs[u].end(), x) - xs[u].begin();
        int res = 0;
        for (int i = p; i; i -= i & -i) res += bit[u][i];
        return res;
    }
    int bit_query(int u, int l, int r) const {
        if (l > r) return 0;
        return bit_query(u, r) - bit_query(u, l - 1);
    }
    void add(int u, int l, int r, int p, int v) {
        bit_add(u, a[p], v);
        if (l == r) return;
        int mid = l + r >> 1;
        if (p <= mid) add(u << 1, l, mid, p, v);
        else add(u << 1 | 1, mid + 1, r, p, v);
    }
    int query(int u, int l, int r, int ql, int qr, int vl, int vr) const {
        if (ql <= l && r <= qr) return bit_query(u, vl, vr);
        int mid = l + r >> 1;
        int res = 0;
        if (ql <= mid) res += query(u << 1, l, mid, ql, qr, vl, vr);
        if (qr > mid) res += query(u << 1 | 1, mid + 1, r, ql, qr, vl, vr);
        return res;
    }
    void add(int p, int v) { add(1, 0, n - 1, p, v); }
    int query(int l, int r, int vl, int vr) const {
        if (l > r || vl > vr) return 0;
        return query(1, 0, n - 1, l, r, vl, vr);
    }
};