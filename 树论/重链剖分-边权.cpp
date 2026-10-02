#include <bits/stdc++.h>
using namespace std;

/**
 * Heavy-Light Decomposition 树链剖分
 * 边权路径拆区间版本: 边权路径 u -> v 可以拆成 O(log n) 段 DFS 序连续区间
 *
 * 状态定义：
 * fa[u] 表示 u 的父亲; dep[u] 表示 u 的深度; sz[u] 表示 u 的子树大小
 * son[u] 表示 u 的重儿子，不存在则为 -1; top[u] 表示 u 所在重链的链顶
 * dfn[u] 表示 u 在线性 DFS 序中的位置; rev[i] 表示 DFS 序位置 i 对应的原树节点
 *
 * 操作：
 * path(u,v)：返回路径 u -> v 上所有边拆成的若干个 DFS 序区间 [l,r]
 * subtree(u)：返回子树 u 内部所有边对应的 DFS 序区间 [l,r]
 * toSeq(a)：把原树边权 a[u] 转成 DFS 序边权 b[dfn[u]]，其中 a[u] 表示边 fa[u] -> u 的权值，root 无意义
 *
 * 复杂度：
 * 建立树链剖分 O(n)，其余查询 O(logn)
 *
 * 注意：
 * 边权默认存在儿子节点上，即边 fa[u] -> u 的权值存在 dfn[u]
 * path(u,u) 没有边，会返回空区间集合
 * subtree(u) 不包含 u 连向父亲的边，叶子节点会返回 l > r 的空区间
 * 如果路径信息不满足交换律，需要额外维护方向，不能直接随便合并这些区间
 */
struct HLD {
    int n, timer;
    vector<int> fa, dep, sz, son, top, dfn, rev;
    HLD() {}
    HLD(vector<vector<int>>& g, int root = 0) {
        n = g.size(), timer = 0;
        fa.assign(n, -1);
        dep.assign(n, 0);
        sz.assign(n, 1);
        son.assign(n, -1);
        top.assign(n, 0);
        dfn.assign(n, 0);
        rev.assign(n, 0);
        dfs1(g, root, -1);
        dfs2(g, root, root);
    }
    void dfs1(vector<vector<int>>& g, int u, int p) {
        fa[u] = p;
        sz[u] = 1;
        for (auto& v : g[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            dfs1(g, v, u);
            sz[u] += sz[v];
            // heavy son: one with the largest subtree size
            if (son[u] == -1 || sz[v] > sz[son[u]]) son[u] = v;
        }
    }
    void dfs2(vector<vector<int>>& g, int u, int t) {
        top[u] = t;
        dfn[u] = timer;
        rev[timer] = u;
        ++timer;
        // heavy son
        if (son[u] != -1) dfs2(g, son[u], t);
        for (auto& v : g[u]) {
            if (v == fa[u] || v == son[u]) continue;
            // light son
            dfs2(g, v, v);
        }
    }
    vector<pair<int, int>> path(int u, int v) {
        vector<pair<int, int>> res;
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            res.push_back({dfn[top[u]], dfn[u]});  // edges fa[top[u]] -> top[u] ... fa[u] -> u
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        if (dfn[u] + 1 <= dfn[v]) res.push_back({dfn[u] + 1, dfn[v]});  // exclude lca node
        return res;
    }
    pair<int, int> subtree(int u) {
        return {dfn[u] + 1, dfn[u] + sz[u] - 1};
    }
    vector<int> toSeq(vector<int>& a) {
        vector<int> b(n);
        for (int i = 0; i < n; ++i) b[dfn[i]] = a[i];
        return b;
    }
};