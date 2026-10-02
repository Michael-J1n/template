#include <bits/stdc++.h>
using namespace std;

/**
 * 连续段合并并查集
 * 适合：数组位置动态激活，每次激活 x 后和左右 active 位置合并，从而维护 active 连续段信息
 *
 * 状态定义：
 * vis[x] 表示下标 x 是否已经激活
 * pa[x] 表示 x 所在连续段的根，只有 active 点可以 find
 * sz[x] 表示以 x 为根的连续段长度，只有根有意义
 * sum[x] 表示以 x 为根的连续段权值和，只有根有意义
 * L[x], R[x] 表示以 x 为根的连续段左右端点，只有根有意义
 * cc 表示当前 active 连续段数量
 *
 * 复杂度：
 * 单次 activate 均摊 O(alpha(n))
 * 前置条件：每个下标只 activate 一次；find/unite 及连续段信息查询只用于已激活且合法的下标
 */
struct SegUFS {
    int n, cc;
    vector<int> pa, sz, vis, sum, L, R;
    SegUFS(int _n) : n(_n), cc(0) {
        pa.resize(n), sz.resize(n), vis.resize(n), sum.resize(n), L.resize(n), R.resize(n);
    }
    int find(int x) {
        if (pa[x] != x) pa[x] = find(pa[x]);
        return pa[x];
    }
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (sz[px] < sz[py]) swap(px, py);
        pa[py] = px;
        sz[px] += sz[py];
        sum[px] += sum[py];
        L[px] = min(L[px], L[py]);
        R[px] = max(R[px], R[py]);
        --cc;
        return true;
    }
    int activate(int x, int w = 1) {
        vis[x] = 1;
        pa[x] = x, sz[x] = 1, sum[x] = w;
        L[x] = R[x] = x;
        ++cc;
        if (x > 0 && vis[x - 1]) unite(x, x - 1);
        if (x + 1 < n && vis[x + 1]) unite(x, x + 1);
        return find(x);
    }
    int size(int x) {
        return sz[find(x)];
    }
    int query_sum(int x) {
        return sum[find(x)];
    }
    pair<int, int> range(int x) {
        int p = find(x);
        return {L[p], R[p]};
    }
};
