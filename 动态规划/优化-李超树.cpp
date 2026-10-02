#include <bits/stdc++.h>
using namespace std;

/**
 * 李超树优化 DP：求形如 dp[i] = min {k(j) * f(i) + b(j)} 的 DP 优化
 * 
 * 初始化要传入所有查询的 f(i) 值 xs；查询时 x 必须在 xs 中
 * 如果要求 max，构造器的 _b 改成 -LINF，_add 中把比较改成 >，查询时把 min 改成 max
 *
 * 单次添加查询复杂度 O(logn)
 */
const long long LINF = 4e18;
struct LiChaoTree {
    struct Line {
        long long k, b;
        Line(long long _k = 0, long long _b = LINF) : k(_k), b(_b) {}
        long long get(long long x) {
            return k * x + b;
        }
    };
    int n;
    vector<long long> xs;
    vector<Line> tr;
    LiChaoTree(const vector<long long>& _xs) {
        xs = _xs;
        ranges::sort(xs);
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        tr.assign(n << 2, Line());
    }
    void _add(Line nw, int p, int l, int r) {
        int mid = (l + r) >> 1;
        bool lef = nw.get(xs[l]) < tr[p].get(xs[l]);
        bool md = nw.get(xs[mid]) < tr[p].get(xs[mid]);
        if (md) swap(nw, tr[p]);
        if (l == r) return;
        if (lef != md) _add(nw, p << 1, l, mid);
        else _add(nw, p << 1 | 1, mid + 1, r);
    }
    void add(long long k, long long b) { _add(Line(k, b), 1, 0, n - 1); }
    long long _query(int pos, int p, int l, int r) {
        long long res = tr[p].get(xs[pos]);
        if (l == r) return res;
        int mid = (l + r) >> 1;
        if (pos <= mid) res = min(res, _query(pos, p << 1, l, mid));
        else res = min(res, _query(pos, p << 1 | 1, mid + 1, r));
        return res;
    }
    long long query(long long x) {
        int pos = ranges::lower_bound(xs, x) - xs.begin();
        return _query(pos, 1, 0, n - 1);
    }
};


/**
 * 示例用法：
 * 
 * LiChaoTree tr(x);
 * tr.add(k[0], b[0]);
 * for (int i = 1; i < n; ++i) {
 *     dp[i] = tr.query(x[i]) + c[i];
 *     tr.add(k[i], dp[i] + d[i]);
 * }
 */