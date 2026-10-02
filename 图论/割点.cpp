#include <bits/stdc++.h>
using namespace std;

/**
 * Tarjan 求无向图割点
 * 割点：删除这个点后，连通块数量会增加
 *
 * 状态定义：
 * dfn[u] 表示 u 的 DFS 时间戳
 * low[u] 表示 u 的整颗子树通过树边和最多一条返祖边能到达的最小 dfn
 *        注意 dfs 树中树边 u->v 不会拿来更新 low[v]
 *
 * 判定：
 * 非根 u，若存在儿子 v 满足 low[v] >= dfn[u]，则 u 是割点
 * 根 u，若 DFS 儿子数 >= 2，则 u 是割点
 * 在 dfs 到 u 时，如果发现儿子 v 有 dfn[v]=-1，说明 u--v 是一条搜索树边
 *
 * 复杂度：O(n + m)
 */
vector<int> cutPoints(vector<vector<int>>& g) {
    int n = g.size(), timer = 0;
    vector<int> dfn(n, -1), low(n), cut(n), res;
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        dfn[u] = low[u] = timer++;
        int child = 0;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            if (dfn[v] == -1) {                 // u--v is a tree edge in the DFS tree
                ++child;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {         // subtree of v cannot reach any proper ancestor of u
                    if (fa != -1) cut[u] = 1;
                }
            } else {                            // u--v is a back edge
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (fa == -1 && child >= 2) cut[u] = 1;
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i, -1);
    }
    for (int i = 0; i < n; ++i) {
        if (cut[i]) res.emplace_back(i);
    }
    return res;
}