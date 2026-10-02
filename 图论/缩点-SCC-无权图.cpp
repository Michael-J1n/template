#include <bits/stdc++.h>
using namespace std;

/**
 * Tarjan 求有向图强连通分量 SCC，并返回缩点 DAG
 * SCC 块内性质：
 * 1. 连通块内部任意两个点 u, v 都满足 u 能到 v 且 v 能到 u
 * 2. 任一点都能作为起点走闭合路线覆盖整个 SCC
 * 3. 如果有环，环一定完整的在某 SCC 内部
 * 
 * dfn[u] = u 在 DFS 下时间戳顺序
 * low[u] = u 这个 DFS 子树能绕回到的最早的栈中节点
 *
 * （！！！）返回：
 * 返回 SCC 缩点后的有向无环图 dag；dag 中节点是 comp 的编号，从 0 到 cnt - 1
 * 同时 comp[u] 会被修改为 u 所属的 SCC 编号，i.e. 缩点 DAG 节点编号
 *
 * 复杂度：O(n + m)
 * 
 * 注意：
 * Tarjan 弹栈得到 SCC 的顺序通常是反拓扑序，所以要按照在 cnt 维度的倒序进行刷表更新，才相当于按照 queue 进行更新的顺序
 */
vector<vector<int>> SCC(vector<vector<int>>& g, vector<int>& comp) {
    int n = g.size(), timer = 0, cnt = 0;
    vector<int> dfn(n, -1), low(n), in(n), stk;
    vector<vector<int>> nodes;
    comp.assign(n, -1);
    auto dfs = [&](this auto&& dfs, int u) -> void {
        dfn[u] = low[u] = timer++;
        stk.emplace_back(u), in[u] = 1;
        for (auto& v : g[u]) {
            if (dfn[v] == -1) {                 // u -> v is a tree edge in the DFS tree
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in[v]) {                 // u -> v points to a node still in the stack, so it can form a cycle
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {                 // u is the root of one SCC (the highest point that can loop back)
            nodes.emplace_back();
            while (true) {
                int x = stk.back(); stk.pop_back();
                in[x] = 0, comp[x] = cnt;
                nodes.back().emplace_back(x);
                if (x == u) break;
            }
            ++cnt;
        }
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i);
    }
    vector<vector<int>> dag(cnt);
    vector<int> vis(cnt, -1);
    for (int a = 0; a < cnt; ++a) {
        for (int u : nodes[a]) {
            for (auto& v : g[u]) {
                int b = comp[v];
                if (a == b || vis[b] == a) continue;
                vis[b] = a;
                dag[a].emplace_back(b);
            }
        }
    }
    return dag;
}