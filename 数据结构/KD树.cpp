#include <bits/stdc++.h>
using namespace std;

/**
 * 二维 K-D Tree / 2D K-D Tree【二维静态点集空间查询】
 * 点编号为 0-based；矩形范围为闭区间 [x1, x2] × [y1, y2]。
 * 距离使用欧氏距离平方；每个节点维护子树最小包围矩形与点数用于剪枝。
 *
 * 支持操作：
 * range_count(x1, y1, x2, y2)：查询矩形内点数；
 * range_query(x1, y1, x2, y2)：查询矩形内所有点的原编号；
 * nearest_dist2(x, y, banned_id)：查询最近点距离平方；
 * nearest_id(x, y, banned_id)：查询最近点原编号；
 * kth_dist2(x, y, k, banned_id)：查询第 k 近点距离平方；
 * nearest(x, y)：返回 {最近邻下标，最近距离平方}。
 * 
 * 适用：二维静态点集的矩形范围查询、最近邻与 k 近邻查询。
 *
 * 复杂度：建树平均 O(n log n)；范围查询与最近邻平均远快于 O(n)，最坏 O(n)；
 * 第 k 近邻最坏 O(n log k)。
 *
 * banned_id = -1 表示不排除任何点；不存在答案时编号返回 -1
 */
template <typename T = long long>
struct KDTree {
    static constexpr T LINF = LLONG_MAX;
    struct Point {
        T x, y;
        int id;
        T operator[](int d) const { return d ? y : x; }
    };
    struct Node {
        Point p;
        int ls, rs, sz;
        T mn[2], mx[2];
    };
    int n, root;
    vector<Point> a;
    vector<Node> tr;
    KDTree() : n(0), root(-1) {}
    KDTree(const vector<pair<T, T>>& points) { init(points); }
    T dist2(const Point& a, const Point& b) const {
        T dx = a.x - b.x;
        T dy = a.y - b.y;
        return dx * dx + dy * dy;
    }
    T box_dist2(int u, const Point& p) const {
        T res = 0;
        for (int d = 0; d < 2; ++d) {
            if (p[d] < tr[u].mn[d]) {
                T delta = tr[u].mn[d] - p[d];
                res += delta * delta;
            } else if (p[d] > tr[u].mx[d]) {
                T delta = p[d] - tr[u].mx[d];
                res += delta * delta;
            }
        }
        return res;
    }
    bool outside(int u, T x1, T y1, T x2, T y2) const { return tr[u].mx[0] < x1 || tr[u].mn[0] > x2 || tr[u].mx[1] < y1 || tr[u].mn[1] > y2; }
    bool inside(int u, T x1, T y1, T x2, T y2) const { return x1 <= tr[u].mn[0] && tr[u].mx[0] <= x2 && y1 <= tr[u].mn[1] && tr[u].mx[1] <= y2; }
    bool inside(const Point& p, T x1, T y1, T x2, T y2) const { return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2; }
    void pushup(int u) {
        tr[u].sz = 1;
        tr[u].mn[0] = tr[u].mx[0] = tr[u].p.x;
        tr[u].mn[1] = tr[u].mx[1] = tr[u].p.y;
        for (int v : {tr[u].ls, tr[u].rs}) {
            if (v == -1) continue;
            tr[u].sz += tr[v].sz;
            for (int d = 0; d < 2; ++d) {
                tr[u].mn[d] = min(tr[u].mn[d], tr[v].mn[d]);
                tr[u].mx[d] = max(tr[u].mx[d], tr[v].mx[d]);
            }
        }
    }
    int build(int l, int r, int dep) {
        if (l > r) return -1;
        int mid = (l + r) >> 1;
        int d = dep & 1;
        nth_element(a.begin() + l, a.begin() + mid, a.begin() + r + 1, [d](const Point& a, const Point& b) {
            if (a[d] != b[d]) return a[d] < b[d];
            if (a[d ^ 1] != b[d ^ 1]) return a[d ^ 1] < b[d ^ 1];
            return a.id < b.id;
        });
        int u = mid;
        tr[u].p = a[mid];
        tr[u].ls = build(l, mid - 1, dep + 1);
        tr[u].rs = build(mid + 1, r, dep + 1);
        pushup(u);
        return u;
    }
    void init(const vector<pair<T, T>>& points) {
        n = points.size();
        a.resize(n);
        tr.resize(n);
        for (int i = 0; i < n; ++i) a[i] = {points[i].first, points[i].second, i};
        root = build(0, n - 1, 0);
    }
    int range_count(int u, T x1, T y1, T x2, T y2) const {
        if (u == -1 || outside(u, x1, y1, x2, y2)) return 0;
        if (inside(u, x1, y1, x2, y2)) return tr[u].sz;
        int res = inside(tr[u].p, x1, y1, x2, y2);
        res += range_count(tr[u].ls, x1, y1, x2, y2);
        res += range_count(tr[u].rs, x1, y1, x2, y2);
        return res;
    }
    int range_count(T x1, T y1, T x2, T y2) const { return range_count(root, x1, y1, x2, y2); }
    void collect(int u, vector<int>& res) const {
        if (u == -1) return;
        res.emplace_back(tr[u].p.id);
        collect(tr[u].ls, res);
        collect(tr[u].rs, res);
    }
    void range_query(int u, T x1, T y1, T x2, T y2, vector<int>& res) const {
        if (u == -1 || outside(u, x1, y1, x2, y2)) return;
        if (inside(u, x1, y1, x2, y2)) {
            collect(u, res);
            return;
        }
        if (inside(tr[u].p, x1, y1, x2, y2)) res.emplace_back(tr[u].p.id);
        range_query(tr[u].ls, x1, y1, x2, y2, res);
        range_query(tr[u].rs, x1, y1, x2, y2, res);
    }
    vector<int> range_query(T x1, T y1, T x2, T y2) const {
        vector<int> res;
        range_query(root, x1, y1, x2, y2, res);
        return res;
    }
    void nearest(int u, const Point& p, int banned_id, T& ans, int& id) const {
        if (u == -1) return;
        if (tr[u].p.id != banned_id) {
            T d = dist2(tr[u].p, p);
            if (d < ans || (d == ans && tr[u].p.id < id)) {
                ans = d;
                id = tr[u].p.id;
            }
        }
        int l = tr[u].ls, r = tr[u].rs;
        T dl = l == -1 ? LINF : box_dist2(l, p);
        T dr = r == -1 ? LINF : box_dist2(r, p);
        if (dl > dr) {
            swap(dl, dr);
            swap(l, r);
        }
        if (dl <= ans) nearest(l, p, banned_id, ans, id);
        if (dr <= ans) nearest(r, p, banned_id, ans, id);
    }
    T nearest_dist2(T x, T y, int banned_id = -1) const {
        Point p = {x, y, -1};
        T ans = LINF;
        int id = -1;
        nearest(root, p, banned_id, ans, id);
        return ans;
    }
    int nearest_id(T x, T y, int banned_id = -1) const {
        Point p = {x, y, -1};
        T ans = LINF;
        int id = -1;
        nearest(root, p, banned_id, ans, id);
        return id;
    }
    void kth(int u, const Point& p, int k, int banned_id, int& cnt, priority_queue<T>& heap) const {
        if (u == -1) return;
        if (tr[u].p.id != banned_id) {
            T d = dist2(tr[u].p, p);
            if (cnt < k) {
                heap.push(d);
                ++cnt;
            } else if (d < heap.top()) {
                heap.pop();
                heap.push(d);
            }
        }
        int l = tr[u].ls, r = tr[u].rs;
        T dl = l == -1 ? LINF : box_dist2(l, p);
        T dr = r == -1 ? LINF : box_dist2(r, p);
        if (dl > dr) {
            swap(dl, dr);
            swap(l, r);
        }
        if (cnt < k || dl < heap.top()) kth(l, p, k, banned_id, cnt, heap);
        if (cnt < k || dr < heap.top()) kth(r, p, k, banned_id, cnt, heap);
    }
    T kth_dist2(T x, T y, int k, int banned_id = -1) const {
        Point p = {x, y, -1};
        priority_queue<T> heap;
        int cnt = 0;
        kth(root, p, k, banned_id, cnt, heap);
        return cnt == k ? heap.top() : LINF;
    }
    pair<int, T> nearest(T x, T y, int banned_id = -1) const {
        Point p = {x, y, -1};
        T ans = LINF;
        int id = -1;
        nearest(root, p, banned_id, ans, id);
        return {id, ans};
    }
};
