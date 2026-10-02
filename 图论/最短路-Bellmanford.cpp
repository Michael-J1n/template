#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;

/**
 * Bellman-Ford 单源最短路，支持负权边，可检测负环
 * 算法不变量：第 i 轮全图松弛考虑的是得到最多 i 条边的简单路径最短路；若没有负环，简单路径最多 n - 1 条边，只需要更新 n - 1 轮
 * 
 * 传入图 g 与起点 source
 * neg[t]=1 代表 s 到 t 的距离可以由负环无限缩小；否则 dis[t] 代表 s 到 t 的最小距离，INF 代表不可达
 * 
 * 复杂度：O(nm)
 */
vector<int> bellmanFord(vector<vector<pair<int, int>>>& g, int source, vector<char>& neg) {
    int n = g.size();
    vector<int> dis(n, INF); dis[source] = 0;
    neg.assign(n, 0);
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        bool upd = 0;
        for (int u = 0; u < n; ++u) {
            if (dis[u] == INF) continue;
            for (auto& [v, w] : g[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    upd = 1;
                    if (i == n - 1 && !neg[v]) neg[v] = 1, q.emplace(v);  // 第 n 轮可以被松弛到的点 v，一定是被某个 s 可达负环影响的点（因此 s->v 的距离可以不断减小）
                }
            }
        }
        if (!upd) break;
    }
    // 搜索所有距离可以无限下降的点
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto& [v, w] : g[u]) {
            if (!neg[v]) neg[v] = 1, q.emplace(v);
        }
    }
    return dis;
}