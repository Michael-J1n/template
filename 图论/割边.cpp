#include <bits/stdc++.h>
using namespace std;

/**
 * Tarjan 求无向图割边
 * 割边：如果删掉这条边之后，图的连通块数量增加
 *
 * 状态定义：
 * dfn[u] 表示 u 的 DFS 时间戳
 * low[u] 表示 u 的整颗子树通过树边和最多一条返祖边能到达的最小 dfn
 *        注意 dfs 树中树边 u->v 不会拿来更新 low[v]
 * 
 * 判定：
 * 对 DFS 树边 u-v，若 low[v] > dfn[u]，则 u-v 是割边
 * 在 dfs 到 u 时，如果发现儿子 v 有 dfn[v]=-1，说明 u--v 是一条搜索树边
 *
 * 复杂度：O(n + m)
 */
vector<pair<int, int>> bridges(vector<vector<int>>& g) {
    int n = g.size(), timer = 0;
    vector<int> dfn(n, -1), low(n);
    vector<pair<int, int>> res;
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        dfn[u] = low[u] = timer++;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            if (dfn[v] == -1) {                 // u--v is a tree edge in the DFS tree
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {          // u--v is a bridge; subtree of v cannot reach any ancestor of u
                    res.emplace_back(u, v);
                }
            } else {                            // u--v is a back edge
                low[u] = min(low[u], dfn[v]);
            }
        }
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i, -1);
    }
    return res;
}

// 若有重边，改用这个（需要给边编号）：
vector<pair<int, int>> bridges(vector<vector<int>>& g) {
    int n = g.size(), m = 0, timer = 0;
    vector<vector<array<int, 2>>> ng(n);
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) {
            if (u >= v) continue;
            ng[u].push_back({v, m});
            ng[v].push_back({u, m});
            ++m;
        }
    }
    vector<int> dfn(n, -1), low(n);
    vector<pair<int, int>> res;
    auto dfs = [&](this auto&& dfs, int u, int eid) -> void {
        dfn[u] = low[u] = timer++;
        for (auto& [v, id] : ng[u]) {
            if (id == eid) continue;
            if (dfn[v] == -1) {                 // u--v is a tree edge in the DFS tree
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {          // u--v is a bridge; subtree of v cannot reach any proper ancestor of u
                    res.emplace_back(u, v);
                }
            } else {                            // u--v is a back edge
                low[u] = min(low[u], dfn[v]);
            }
        }
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i, -1);
    }
    return res;
}