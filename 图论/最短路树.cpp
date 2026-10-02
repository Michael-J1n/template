#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

/**
 * 最短路树
 * 
 * 图上的从起点开始最短路树上的点满足，每个点的离起点的树上距离等于每个点在图中离起点的最短路距离
 * 
 * 状态定义：
 * d[v] 表示 s 到 v 的最短路距离，pre[v] 表示最短路树中 v 的父亲，tree[u] 存最短路树上 u 的儿子
 * 
 * 核心性质：若 pre[v] = u，则 d[v] = d[u] + w(u, v)
 * 
 * 复杂度：
 * O((n + m) log n)
 */

vector<int> shortestPathTree(int n, vector<vector<pair<int, int>>>& g, int s, vector<int>& pre, vector<vector<int>>& tree) {
    vector<int> d(n, INF), vis(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pre.assign(n, -1);
    tree.assign(n, {});
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
                pre[v] = u;
                pq.push({d[v], v});
            }
        }
    }
    for (int v = 0; v < n; ++v) {
        if (v != s && pre[v] != -1) tree[pre[v]].emplace_back(v);
    }
    return d;
}
// 恢复 s 到 t 的一条最短路
vector<int> getPath(int s, int t, vector<int>& pre) {
    vector<int> path;
    for (int x = t; x; x = pre[x]) {
        path.emplace_back(x);
        if (x == s) break;
    }
    if (path.back() != s) return {};
    reverse(path.begin(), path.end());
    return path;
}