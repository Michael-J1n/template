#include <bits/stdc++.h>
using namespace std;

/**
 * 二维线段树 / 2D Segment Tree【矩形区间 chmax + 单点查询】
 * 考虑这样一个问题：矩形给子区间按照时间顺序覆盖染色；使用一个哈希表记录时间戳->颜色，每个点的最终颜色转化为求这个点上的最大时间戳；因此问题化归为：每次给子矩形 chmax 一个新时间戳。
 * 
 * 索引：0-based，矩形闭区间 [r1, r2] × [c1, c2]
 * 支持操作：
 * update(r1, c1, r2, c2, val)：矩形内所有元素取 chmax(val)
 * query(r, c)：查询单点的值
 *
 * 适用：矩形覆盖、时间戳染色；无需 pushdown
 * 复杂度：建树 O(nm)，修改 O(log n log m)，查询 O(log n log m)，空间 O(nm)
 */
struct SegTree2D {
    int n, m;
    vector<vector<int>> tr;
    SegTree2D(int n, int m) : n(n), m(m), tr(n << 2, vector<int>(m << 2)) {}
    void update_y(int p, int q, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) {
            tr[p][q] = max(tr[p][q], val);
            return;
        }
        int mid = l + r >> 1;
        if (ql <= mid) update_y(p, q << 1, l, mid, ql, qr, val);
        if (qr > mid) update_y(p, q << 1 | 1, mid + 1, r, ql, qr, val);
    }
    void update_x(int p, int l, int r, int r1, int r2, int c1, int c2, int val) {
        if (r1 <= l && r <= r2) {
            update_y(p, 1, 0, m - 1, c1, c2, val);
            return;
        }
        int mid = l + r >> 1;
        if (r1 <= mid) update_x(p << 1, l, mid, r1, r2, c1, c2, val);
        if (r2 > mid) update_x(p << 1 | 1, mid + 1, r, r1, r2, c1, c2, val);
    }
    void update(int r1, int c1, int r2, int c2, int val) {
        update_x(1, 0, n - 1, r1, r2, c1, c2, val);
    }
    int query_y(int p, int q, int l, int r, int c) {
        int res = tr[p][q];
        if (l == r) return res;
        int mid = l + r >> 1;
        if (c <= mid) return max(res, query_y(p, q << 1, l, mid, c));
        return max(res, query_y(p, q << 1 | 1, mid + 1, r, c));
    }
    int query_x(int p, int l, int r, int x, int y) {
        int res = query_y(p, 1, 0, m - 1, y);
        if (l == r) return res;
        int mid = l + r >> 1;
        if (x <= mid) return max(res, query_x(p << 1, l, mid, x, y));
        return max(res, query_x(p << 1 | 1, mid + 1, r, x, y));
    }
    int query(int r, int c) {
        return query_x(1, 0, n - 1, r, c);
    }
};