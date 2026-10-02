#include <bits/stdc++.h>
using namespace std;

/**
 * 树上差分
 * 
 * 离线的给树上一条路径的所有 点权 / 边权 共同加上一个树
 * 
 * 思想与数组差分类似，在路径端点做加法，贡献在 LCA 减去
 */
struct LCA {
    int n;
    vector<int> depth, first, lg, euler, father;
    vector<int> dfn;
    int timer;
    vector<vector<int>> st;
    void dfs(vector<vector<int>>& g, int u, int fa) {
        first[u] = euler.size();
        euler.emplace_back(u);
        dfn[u] = timer++;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            depth[v] = depth[u] + 1;
            father[v] = u;
            dfs(g, v, u);
            euler.emplace_back(u);
        }
    }
    LCA(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        timer = 0;
        depth.resize(n);
        dfn.resize(n);
        first.assign(n, -1);
        father.resize(n, -1);
        dfs(g, root, -1);
        int m = euler.size();
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg[i] = lg[i >> 1] + 1;
        int K = lg[m] + 1;
        st.assign(K, vector<int>(m));
        st[0] = euler;
        for (int k = 1; k < K; ++k) {
            for (int i = 0; i + (1 << k) <= m; ++i) {
                int x = st[k - 1][i], y = st[k - 1][i + (1 << (k - 1))];
                st[k][i] = depth[x] < depth[y] ? x : y;
            }
        }
    }
    int get_lca(int x, int y) {
        int l = first[x], r = first[y];
        if (l > r) swap(l, r);
        int k = lg[r - l + 1];
        int a = st[k][l], b = st[k][r - (1 << k) + 1];
        return depth[a] < depth[b] ? a : b;
    }
    int dist(int x, int y) {
        int z = get_lca(x, y);
        return depth[x] + depth[y] - 2 * depth[z];
    }
    bool on_path(int x, int u, int v) { return dist(u, x) + dist(x, v) == dist(u, v); }
};

/**
 * 点权路径差分
 * qs: {u, v, w} 表示路径 u-v 上所有点的点权加 w
 * 关键公式：d[u] += w, d[v] += w, d[p] -= w, d[fa[p]] -= w
 * fa[root] = -1
 * 返回 d[x]: 点 x 的最终权值
 */
vector<int> pointDiff(vector<vector<int>>& g, vector<int>& fa, vector<array<int, 3>>& qs, LCA& lca, int root = 0) {
    int n = g.size();
    vector<int> d(n);
    for (auto& [u, v, w] : qs) {
        int p = lca.get_lca(u, v);
        d[u] += w, d[v] += w;
        d[p] -= w;
        if (fa[p] != -1) d[fa[p]] -= w;
    }
    auto dfs = [&](this auto&& dfs, int u, int f) -> void {
        for (auto& v : g[u]) {
            if (v == f) continue;
            dfs(v, u);
            d[u] += d[v];
        }
    };
    dfs(root, -1);
    return d;
}

/**
 * 边权路径差分
 * qs: {u, v, w} 表示路径 u-v 上所有边的边权加 w
 * 关键公式：d[u] += w, d[v] += w, d[p] -= w * 2
 * fa[root] = -1
 * 返回 d[x]: 边 fa[x]-x 的最终权值；d[root] 无意义，不进行统计
 */
vector<int> edgeDiff(vector<vector<int>>& g, vector<int>& fa, vector<array<int, 3>>& qs, LCA& lca, int root = 0) {
    int n = g.size();
    vector<int> d(n);
    for (auto& [u, v, w] : qs) {
        int p = lca.get_lca(u, v);
        d[u] += w, d[v] += w;
        d[p] -= 2 * w;
    }
    auto dfs = [&](this auto&& dfs, int u, int f) -> void {
        for (auto& v : g[u]) {
            if (v == f) continue;
            dfs(v, u);
            d[u] += d[v];
        }
    };
    dfs(root, -1);
    return d;
}