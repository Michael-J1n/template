#include <bits/stdc++.h>
using namespace std;

/**
 * O(n+m) 图上 DFS
 * 图编号 0~n-1，g 为邻接表
 * 返回每个点所属连通块编号
 */

vector<int> dfs(vector<vector<int>>& g) {
    int n = g.size(), id = 0;
    vector<int> comp(n, -1);
    auto dfs = [&](this auto&& dfs, int u) -> void {
        comp[u] = id;
        for (auto& v : g[u]) if (comp[v] == -1) {
            dfs(v);
        }
    };
    for (int i = 0; i < n; ++i) {
        if (comp[v] != -1) continue;
        dfs(i), ++id;
    }
    return comp;
}