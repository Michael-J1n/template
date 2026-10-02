#include <bits/stdc++.h>
using namespace std;

/**
 * 求单点 k 级祖先与两点最近公共祖先
 * 适用倍增表卡 MLE 的情况
 * 
 * 预处理 O(n)，查询 O(logn)
 */
struct LCA {
    int n, timer;
    vector<int> depth, father, sz, son, top, dfn, seq;
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
        dfn[u] = timer;
        seq[timer++] = u;
        if (son[u] != -1) dfs2(g, son[u], tp);
        for (auto& v : g[u]) {
            if (v == father[u] || v == son[u]) continue;
            dfs2(g, v, v);
        }
    }
    LCA(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        timer = 0;
        depth.resize(n);
        father.resize(n);
        sz.resize(n);
        son.assign(n, -1);
        top.resize(n);
        dfn.resize(n);
        seq.resize(n);
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
    int get_kth_ancestor(int u, int k) {
        if (k > depth[u]) return -1;
        while (depth[u] - depth[top[u]] < k) k -= depth[u] - depth[top[u]] + 1, u = father[top[u]];
        return seq[dfn[u] - k];
    }
    int dist(int x, int y) { return depth[x] + depth[y] - 2 * depth[get_lca(x, y)]; }
    bool on_path(int x, int u, int v) { return dist(u, x) + dist(x, v) == dist(u, v);}
};