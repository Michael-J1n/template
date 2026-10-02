#include <bits/stdc++.h>
using namespace std;

/**
 * 求一颗树的重心
 * 
 * 重心：u 为重心即为 删去 u 后最大的连通块大小最小，即 每个连通块大小<=n/2
 * 性质：最多有两个重心，且如果有两个则他们一定相邻
 * 求法：对于每个节点看 max(n-sz[u], sz[v1],...sz[vk]) 什么时候到达最小值
 */
vector<int> treeCentroid(vector<vector<int>>& g) {
    int n = g.size(), best = n;
    vector<int> sz(n);
    vector<int> res;
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        sz[u] = 1;
        int mx = 0;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            sz[u] += sz[v];
            mx = max(mx, sz[v]);
        }
        mx = max(mx, n - sz[u]);
        if (mx < best) best = mx, res = {u};
        else if (mx == best) res.emplace_back(u);
    };
    dfs(0, -1);
    return res;
}