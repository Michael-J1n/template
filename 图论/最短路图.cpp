#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

/**
 * 最短路图模板
 * 
 * 返回最短路图得 DAG
 * 
 * d[v]：s 到 v 的最短路长度
 * dag[u]：所有满足 d[v] = d[u] + w(u, v) 的边 u -> v
 * rdag[v]：最短路图的反图，即所有可以作为 v 的最短路前驱的点
 * 
 * 要求：边权非负，点编号 0-based
 * 注意：若所有边权 > 0，则 dag 一定是 DAG；若存在 0 边，dag 可能有环
 * 
 * 复杂度：O((n + m) log n)
 */

vector<int> shortestPathGraph(int n, vector<vector<pair<int, int>>>& g, int s, vector<vector<int>>& dag, vector<vector<int>>& rdag) {
    vector<int> d(n, INF), vis(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    d[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        for (auto& [v, w] : g[u]) {
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
        }
    }
    dag.assign(n, {});
    rdag.assign(n, {});
    for (int u = 0; u < n; ++u) {
        if (d[u] == INF) continue;
        for (auto& [v, w] : g[u]) {
            if (d[v] == d[u] + w) {
                dag[u].emplace_back(v);
                rdag[v].emplace_back(u);
            }
        }
    }
    return d;
}