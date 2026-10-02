#include <bits/stdc++.h>
using namespace std;

/**
 * O(n+m) 图上 BFS
 * 图编号 0~n-1，g 为邻接表
 * 返回每个点所属连通块编号
 */

vector<int> bfs(vector<vector<int>>& g) {
    int n = g.size(), id = 0;
    vector<int> comp(n, -1);
    auto bfs = [&](int s) -> void {
        queue<int> q; q.emplace(s);
        comp[s] = id;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& v : g[u]) {
                if (comp[v] != -1) continue;
                comp[v] = id;
                q.emplace(v);
            }
        }
    };
    for (int i = 0; i < n; ++i) {
        if (comp[i] != -1) continue;
        bfs(i), ++id;
    }
    return comp;
}