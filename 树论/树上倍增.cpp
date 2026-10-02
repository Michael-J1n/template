#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;

/**
 * 树上倍增 / LCA / 路径信息查询
 *
 * 输入：
 * - g[u] = {{v, w}}，0-based 带权树
 *
 * 功能：
 * - getKthAncestor(u, k)：u 向上跳 k 步
 * - getLCA(u, v)：求 LCA
 * - dist(u, v)：树上距离，按边数
 * - isAncestor(u, v)：u 是否是 v 的祖先
 * - kthOnPath(u, v, k)：u 到 v 路径上第 k 个点，k 从 0 开始
 * - queryMaxUp(u, len)：u 向上 len 条边的最大边权
 * - queryMinUp(u, len)：u 向上 len 条边的最小边权
 * - querySumUp(u, len)：u 向上 len 条边的边权和
 * - queryMinMax(u, v)：u-v 路径上的最小/最大边权
 * - querySum(u, v)：u-v 路径边权和
 * - jumpMaxLE(u, lim)：从 u 向上跳，要求经过边权最大值 <= lim
 * - jumpMinGE(u, lim)：从 u 向上跳，要求经过边权最小值 >= lim
 * - jumpSumLE(u, lim)：从 u 向上跳，要求经过边权和 <= lim，通常要求边权非负
 *
 * 复杂度：
 * - 预处理 O(nlogn)
 * - 单次查询 O(logn)
 */
struct TreeLift {
    int n, m;
    vector<int> dep;
    vector<vector<int>> pa, mx, mn, sum;
    TreeLift(vector<vector<pair<int, int>>>& g, int root = 0) {
        n = g.size(), m = __lg(max(1, n)) + 1;
        dep.assign(n, 0);
        pa.assign(n, vector<int>(m));
        mx.assign(n, vector<int>(m, -INF));
        mn.assign(n, vector<int>(m, INF));
        sum.assign(n, vector<int>(m, 0));
        dfs(g, root, root, 0);
    }
    void dfs(vector<vector<pair<int, int>>>& g, int u, int p, int w) {
        pa[u][0] = p;
        if (u != p) mx[u][0] = mn[u][0] = sum[u][0] = w;
        for (int j = 1; j < m; ++j) {
            int x = pa[u][j - 1];
            pa[u][j] = pa[x][j - 1];
            mx[u][j] = max(mx[u][j - 1], mx[x][j - 1]);
            mn[u][j] = min(mn[u][j - 1], mn[x][j - 1]);
            sum[u][j] = sum[u][j - 1] + sum[x][j - 1];
        }
        for (auto& [v, nw] : g[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            dfs(g, v, u, nw);
        }
    }
    int getKthAncestor(int u, int k) {
        if (k > dep[u]) return -1;
        for (int j = 0; j < m; ++j) {
            if (k >> j & 1) u = pa[u][j];
        }
        return u;
    }
    int getLCA(int u, int v) {
        if (dep[u] > dep[v]) swap(u, v);
        v = getKthAncestor(v, dep[v] - dep[u]);
        if (u == v) return u;
        for (int j = m - 1; j >= 0; --j) {
            if (pa[u][j] != pa[v][j]) {
                u = pa[u][j], v = pa[v][j];
            }
        }
        return pa[u][0];
    }
    int dist(int u, int v) {
        int z = getLCA(u, v);
        return dep[u] + dep[v] - 2 * dep[z];
    }
    bool isAncestor(int u, int v) {
        return getLCA(u, v) == u;
    }
    int kthOnPath(int u, int v, int k) {
        int z = getLCA(u, v);
        int d1 = dep[u] - dep[z], d2 = dep[v] - dep[z];
        int d = d1 + d2;
        if (k > d) return -1;
        if (k <= d1) return getKthAncestor(u, k);
        return getKthAncestor(v, d - k);
    }
    int queryMaxUp(int u, int len) {
        int res = -INF;
        for (int j = 0; j < m; ++j) {
            if (len >> j & 1) {
                res = max(res, mx[u][j]);
                u = pa[u][j];
            }
        }
        return res;
    }
    int queryMinUp(int u, int len) {
        int res = INF;
        for (int j = 0; j < m; ++j) {
            if (len >> j & 1) {
                res = min(res, mn[u][j]);
                u = pa[u][j];
            }
        }
        return res;
    }
    int querySumUp(int u, int len) {
        int res = 0;
        for (int j = 0; j < m; ++j) {
            if (len >> j & 1) {
                res += sum[u][j];
                u = pa[u][j];
            }
        }
        return res;
    }
    pair<int, int> queryMinMax(int u, int v) {
        int a = INF, b = -INF;
        if (dep[u] > dep[v]) swap(u, v);
        int d = dep[v] - dep[u];
        for (int j = 0; j < m; ++j) {
            if (d >> j & 1) {
                a = min(a, mn[v][j]);
                b = max(b, mx[v][j]);
                v = pa[v][j];
            }
        }
        if (u == v) return {a, b};
        for (int j = m - 1; j >= 0; --j) {
            if (pa[u][j] != pa[v][j]) {
                a = min({a, mn[u][j], mn[v][j]});
                b = max({b, mx[u][j], mx[v][j]});
                u = pa[u][j], v = pa[v][j];
            }
        }
        a = min({a, mn[u][0], mn[v][0]});
        b = max({b, mx[u][0], mx[v][0]});
        return {a, b};
    }
    int queryMin(int u, int v) {
        return queryMinMax(u, v).first;
    }
    int queryMax(int u, int v) {
        return queryMinMax(u, v).second;
    }
    int querySum(int u, int v) {
        int z = getLCA(u, v);
        return querySumUp(u, dep[u] - dep[z]) + querySumUp(v, dep[v] - dep[z]);
    }
    int jumpMaxLE(int u, int lim) {
        for (int j = m - 1; j >= 0; --j) {
            if (mx[u][j] <= lim) {
                u = pa[u][j];
            }
        }
        return u;
    }
    int jumpMinGE(int u, int lim) {
        for (int j = m - 1; j >= 0; --j) {
            if (mn[u][j] >= lim) {
                u = pa[u][j];
            }
        }
        return u;
    }
    int jumpSumLE(int u, int lim) {
        int cur = 0;
        for (int j = m - 1; j >= 0; --j) {
            if (cur + sum[u][j] <= lim) {
                cur += sum[u][j];
                u = pa[u][j];
            }
        }
        return u;
    }
};