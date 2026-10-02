#include <bits/stdc++.h>
using namespace std;

/**
 * Tarjan 求无向图边双连通分量 eDCC，并返回桥树
 * eDCC：连通块内部不存在割边；将图中不经过桥边还可以互相到达的点缩为一个连通块
 * low[u] = u 这个 DFS 子树，在不走父边的情况下，能通过返祖边绕回到的最早祖先的 dfn
 *
 * （！！！）返回：
 * 返回边双缩点后的桥树 tree；tree 中节点是 comp 的编号，从 0 到 cnt - 1
 * 同时 comp[u] 会被修改为 u 所属的边双编号，i.e. 桥树节点编号
 *
 * 复杂度：O(n + m)
 */
vector<vector<int>> eDCC(vector<vector<int>>& g, vector<int>& comp) {
    int n = g.size(), m = 0, timer = 0;
    vector<array<int, 2>> edges;
    vector<vector<array<int, 2>>> ng(n);
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) {
            if (u > v) continue;
            edges.push_back({u, v});
            ng[u].push_back({v, m});
            ng[v].push_back({u, m});
            ++m;
        }
    }
    vector<int> dfn(n, -1), low(n);
    vector<char> is_bridge(m);
    auto dfs = [&](this auto&& dfs, int u, int eid) -> void {
        dfn[u] = low[u] = timer++;
        for (auto& [v, id] : ng[u]) {
            if (id == eid) continue;
            if (dfn[v] == -1) {                 // u--v is a tree edge in the DFS tree
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {          // u--v is a bridge; subtree of v cannot reach u or any ancestor of u
                    is_bridge[id] = 1;
                }
            } else {                            // u--v is a back edge
                low[u] = min(low[u], dfn[v]);
            }
        }
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i, -1);
    }
    int cnt = 0;
    comp.assign(n, -1);
    auto paint = [&](this auto&& paint, int u) -> void {
        comp[u] = cnt;
        for (auto& [v, id] : ng[u]) {
            if (comp[v] != -1 || is_bridge[id]) continue;
            paint(v);
        }
    };
    for (int i = 0; i < n; ++i) {
        if (comp[i] == -1) paint(i), ++cnt;
    }
    vector<vector<int>> tree(cnt);
    for (int i = 0; i < m; ++i) {
        if (!is_bridge[i]) continue;
        auto [u, v] = edges[i];
        u = comp[u], v = comp[v];
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
    return tree;
}

// 如果需要每个 bcc 节点内部的原图点列表；原图节点 u 所对应的 bcc id 为 comp[u]
vector<vector<int>> getBCC(vector<int>& comp, int cnt) {
    int n = comp.size();
    vector<vector<int>> bcc(cnt);
    for (int i = 0; i < n; ++i) {
        bcc[comp[i]].push_back(i);
    }
    return bcc;
}