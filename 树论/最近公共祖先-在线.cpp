#include <bits/stdc++.h>
using namespace std;

/**
 * 在线倍增 LCA，支持维护整个森林
 *
 * 状态定义：
 * fa[u][j] 表示 u 的 2^j 级祖先；dep[u] 表示 u 的深度；rt[u] 表示 u 所属树的根。
 * 根节点的所有祖先均设为自己。
 *
 * 操作：
 * add_root(u)：加入一个新的根节点 u。
 * add(u, p)：加入新节点 u，并令 p 为 u 的父亲，要求 p 已经存在。
 * jump(u, k)：返回 u 的 k 级祖先；若 k > dep[u]，返回所在树的根。
 * lca(u, v)：返回 u、v 的最近公共祖先；不连通返回 -1。
 * dis(u, v)：返回 u、v 之间的距离；不连通返回 -1。
 *
 * 复杂度：
 * 加点、查询均为 O(log n)，空间复杂度 O(n log n)。
 */
const int LOG = 31;
struct forestLCA {
    int n;
    vector<array<int, LOG>> fa;
    vector<int> dep, rt;
    forestLCA(int _n) : n(_n), fa(n), dep(n), rt(n, -1) {}
    void add_root(int u) {
        dep[u] = 0, rt[u] = u;
        for (int j = 0; j < LOG; ++j) fa[u][j] = u;
    }
    void add(int u, int p) {
        dep[u] = dep[p] + 1, rt[u] = rt[p], fa[u][0] = p;
        for (int j = 1; j < LOG; ++j) fa[u][j] = fa[fa[u][j - 1]][j - 1];
    }
    int jump(int u, int k) {
        for (int j = 0; j < LOG; ++j) {
            if (k >> j & 1) u = fa[u][j];
        }
        return u;
    }
    bool connected(int u, int v) { return rt[u] != -1 && rt[u] == rt[v]; }
    int lca(int u, int v) {
        if (!connected(u, v)) return -1;
        if (dep[u] < dep[v]) swap(u, v);
        u = jump(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; --j) {
            if (fa[u][j] == fa[v][j]) continue;
            u = fa[u][j], v = fa[v][j];
        }
        return fa[u][0];
    }
    int dis(int u, int v) {
        int p = lca(u, v);
        if (p == -1) return -1;
        return dep[u] + dep[v] - 2 * dep[p];
    }
};


/**
 * 还有一种 LCA，支持 O(n) 预处理和 O(log n) 查询，在 Euler 序被空间卡掉时可以换成这个
 */
struct LCA {
    int n;
    vector<int> depth, father, sz, son, top;
    void dfs1(vector<vector<int>>& g, int u, int fa) {
        father[u] = fa;
        depth[u] = fa == -1 ? 0 : depth[fa] + 1;
        sz[u] = 1;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs1(g, v, u);
            sz[u] += sz[v];
            if (son[u] == -1 || sz[v] > sz[son[u]]) son[u] = v;
        }
    }
    void dfs2(vector<vector<int>>& g, int u, int tp) {
        top[u] = tp;
        if (son[u] != -1) dfs2(g, son[u], tp);
        for (auto& v : g[u]) {
            if (v == father[u] || v == son[u]) continue;
            dfs2(g, v, v);
        }
    }
    LCA(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        depth.resize(n);
        father.resize(n);
        sz.resize(n);
        son.assign(n, -1);
        top.resize(n);
        dfs1(g, root, -1);
        dfs2(g, root, root);
    }
    int get_lca(int x, int y) {
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]]) swap(x, y);
            x = father[top[x]];
        }
        return depth[x] < depth[y] ? x : y;
    }
    int dist(int x, int y) { return depth[x] + depth[y] - 2 * depth[get_lca(x, y)]; }
    bool on_path(int x, int u, int v) { return dist(u, x) + dist(x, v) == dist(u, v); }
};