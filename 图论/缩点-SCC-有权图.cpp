#include <bits/stdc++.h>
using namespace std;

/**
 * Tarjan 求有向带权图强连通分量 SCC，并返回带权缩点 DAG
 * SCC：连通块内部任意两个点 u, v 都满足 u 能到 v 且 v 能到 u
 * low[u] = u 这个 DFS 子树能绕回到的最早的栈中节点
 *
 * （！！！）返回：
 * 返回 SCC 缩点后的带权有向无环图 dag；dag 中节点是 comp 的编号，从 0 到 cnt - 1
 * dag[a] 中的边 (b, w) 表示原图中存在一条跨 SCC 的带权边 u -> v, w，其中 comp[u] = a, comp[v] = b
 * 同时 comp[u] 会被修改为 u 所属的 SCC 编号，i.e. 缩点 DAG 节点编号
 *
 * 注意：
 * 这个模板不会对带权边去重，因为同一对 SCC 之间的多条边可能需要按题意取 max / min / sum / count / 原样保留
 * Tarjan 弹栈得到 SCC 的顺序通常是反拓扑序
 * 
 * 复杂度：O(n + m)
 */
vector<vector<pair<int, int>>> SCC(vector<vector<pair<int, int>>>& g, vector<int>& comp) {
    int n = g.size(), timer = 0, cnt = 0;
    vector<int> dfn(n, -1), low(n), in(n), stk;
    comp.assign(n, -1);
    auto dfs = [&](this auto&& dfs, int u) -> void {
        dfn[u] = low[u] = timer++;
        stk.emplace_back(u), in[u] = 1;
        for (auto& [v, w] : g[u]) {
            if (dfn[v] == -1) {                 // u -> v is a tree edge in the DFS tree
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in[v]) {                 // u -> v points to a node still in the stack, so it can form a cycle
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {                 // u is the root of one SCC (the highest point that can loop back)
            while (true) {
                int x = stk.back(); stk.pop_back();
                in[x] = 0, comp[x] = cnt;
                if (x == u) break;
            }
            ++cnt;
        }
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i);
    }
    vector<vector<pair<int, int>>> dag(cnt);
    for (int u = 0; u < n; ++u) {
        int a = comp[u];
        for (auto& [v, w] : g[u]) {
            int b = comp[v];
            if (a == b) continue;
            dag[a].push_back({b, w});
        }
    }
    return dag;
}


// 注：如果 dag 之间边权要带权去重，可以参考下面这个保留最大边权的模板
vector<vector<pair<int, int>>> dedup(vector<vector<pair<int, int>>>& dag) {
    int n = dag.size();
    vector<vector<pair<int, int>>> res(n);
    vector<int> vis(n, -1), val(n);
    for (int u = 0; u < n; ++u) {
        for (auto& [v, w] : dag[u]) {
            if (vis[v] != u) {
                vis[v] = u;
                val[v] = w;
                res[u].push_back({v, w});
            } else {
                val[v] = max(val[v], w);
            }
        }
        for (auto& [v, w] : res[u]) w = val[v];
    }
    return res;
}