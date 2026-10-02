#include <bits/stdc++.h>
using namespace std;

/**
 * 拓扑排序
 * 处理有向无环图 DAG 的拓扑顺序
 * 
 * 复杂度 O(N+M) 
 * 
 * 注意：
 * 1. 若 topo.size() < n，说明图中有环，不是 DAG
 * 2. 如果需要按照深度分组处理，在循环内先记录 sz=q.size()，这 sz 个点即为同一深度的
 */
vector<int> topoSort(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> ind(n), topo;
    for (int i = 0; i < n; ++i) {
        for (auto& j : g[i]) ++ind[j];
    }
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (!ind[i]) q.emplace(i);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo.emplace_back(u);
        for (auto& v : g[u]) {
            --ind[v];
            // dep[v] = max(dep[v], dep[u] + 1);  // 若需要深度信息
            if (!ind[v]) q.emplace(v);
        }
    }
    return topo;
}