#include <bits/stdc++.h>
using namespace std;

const long long LINF = 4e18;

/**
 * 最短路图模板
 * 
 * 返回最短路图得 DAG
 * 
 * 返回：
 * d[v]：s 到 v 的最短路长度
 * dag[u]：所有满足 d[v] = d[u] + w(u, v) 的边 u -> v
 * 
 * 性质：
 * 保留所有紧边 u->v，意为 dis[u] + w(u, v) = dis[v]；每条紧边有 dis[u] < dis[v]，构成一个唯一的 DAG。
 * s -> v 的路径是最短路径，当且仅当它完全由紧边组成。
 * 按照 dis 升序排列就是合法拓扑序。每个点至少有一条入边，除了根节点 s 入度为 0.
 * 
 * 要求：边权非负，点编号 0-based
 * 注意：若所有边权 > 0，则 dag 一定是 DAG；若存在 0 边，dag 可能有环
 * 
 * 复杂度：O((n + m) log n)
 */
vector<long long> SPG(vector<vector<pair<int, int>>>& g, int s, vector<vector<int>>& dag) {
    int n = g.size();
    vector<long long> d(n, LINF); d[s] = 0;
    vector<char> vis(n);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq; pq.emplace(0, s);
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        for (auto& [v, w] : g[u]) {
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.emplace(d[v], v);
            }
        }
    }
    dag.assign(n, {});
    for (int u = 0; u < n; ++u) {
        if (d[u] == LINF) continue;
        for (auto& [v, w] : g[u]) {
            if (d[v] == d[u] + w) {
                dag[u].emplace_back(v);
            }
        }
    }
    return d;
}

/**
 * 应用：在最短路 DAG 上，可以进行 DAG DP。
 * 例1：最短路计数，计算源点 s 到任一点 v 的最短路径数量。
 */
vector<long long> countP(vector<vector<pair<int, int>>>& g, int s = 0) {
    int n = g.size();
    vector<vector<int>> dag;
    vector<int> ind(n);
    auto dis = SPG(g, s, dag);
    for (int u = 0; u < n; ++u) {
        for (int v : dag[u]) ++ind[v];
    }
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        if (!ind[u]) q.emplace(u);
    }
    vector<long long> dp(n); dp[s] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : dag[u]) {
            dp[v] += dp[u];
            --ind[v];
            if (!ind[v]) q.emplace(v);
        }
    }
    return dp;
}

/**
 * 例2：最短路树计数，计算以 s 为根的最短路树的数量。
 * 每个点可以独立选择一条入边作为自己的父边（这样一路追溯到祖先一定是s），因此答案是 PROD ind[v]
 */
long long countT(vector<vector<pair<int, int>>>& g, int s = 0) {
    int n = g.size();
    vector<vector<int>> dag;
    auto dis = SPG(g, s, dag);
    vector<int> ind(n);
    for (int u = 0; u < n; ++u) {
        for (int v : dag[u]) ++ind[v];
    }
    long long res = 1;
    for (int u = 0; u < n; ++u) {
        if (u != s && dis[u] != LINF) res *= ind[u];
    }
    return res;
}