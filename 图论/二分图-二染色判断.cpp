#include <bits/stdc++.h>
using namespace std;

/**
 * 判断给定无向图是否为二分图
 * 
 * 定理：
 * 一张无向图是二分图，当且仅当图中不存在奇环
 * 
 * 复杂度：
 * O(n + m)
 */
bool bipartite(vector<vector<int>>& g, vector<int>& color) {
    int n = g.size();
    color.assign(n, -1);
    auto bfs = [&](int s) -> bool {
        queue<int> q; q.emplace(s);
        color[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& v : g[u]) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;
                    q.emplace(v);
                } else if (color[v] == color[u]) {    // u-v-u is an odd cycle
                    return 0;
                }
            }
        }
        return 1;
    };
    for (int i = 0; i < n; ++i) {
        if (color[i] != -1) continue;
        if (!bfs(i)) return 0;
    }
    return 1;
}