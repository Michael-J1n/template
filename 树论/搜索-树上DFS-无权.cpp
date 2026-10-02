#include <bits/stdc++.h>
using namespace std;

/**
 * pa[u]：父节点
 * dep[u]：深度，根节点深度为0
 * sz[u]：子树大小（包含根）
 * tin[u]：dfs进入时间戳
 * tout[u]：dfs推出时间戳
 * order：树上dfs序
 */

struct Tree {
    int n, timer = 0;
    vector<int> pa, dep, sz, tin, tout, order;

    Tree(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        pa.assign(n, -1);
        dep.assign(n, 0);
        sz.assign(n, 0);
        tin.assign(n, 0), tout.assign(n, 0);
        order.reserve(n);
        auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
            pa[u] = fa;
            sz[u] = 1;
            tin[u] = timer++;
            order.emplace_back(u);
            for (auto& v : g[u]) {
                if (v == fa) continue;
                dep[v] = dep[u] + 1;
                dfs(v, u);
                sz[u] += sz[v];
            }
            tout[u] = timer;
        };
        dfs(root, -1);
    }
    
    bool isAncester(int x, int y) {
        return tin[x] <= tin[y] && tout[y] <= tout[x];
    }
    int getParent(int x) {
        return pa[x];
    }
    int getDepth(int x) {
        return dep[x];
    }
    int getSize(int x) {
        return sz[x];
    }
};